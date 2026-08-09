#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_NO_ECHO
#include "nob.h"

#include "sqlite3.h"
#include "bundle.h"

#define STR(x) STR2_ELECTRIC_BOOGALOO(x)
#define STR2_ELECTRIC_BOOGALOO(x) #x
#define DEFAULT_SERVE_PORT 8000
#define DEFAULT_COMMAND "help"

// Computed at runtime in main()
static const char *HOME_PATH = NULL;
static const char *WEBC_DIR_PATH = NULL;
static const char *WEBC_DB_PATH = NULL;
static bool WEBC_TRACE_MIGRATION_QUERIES = false;

#define LOG_SQLITE3_ERROR(db) fprintf(stderr, "%s:%d: SQLITE3 ERROR: %s\n", __FILE__, __LINE__, sqlite3_errmsg(db))

bool txn_begin(sqlite3 *db) {
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return false;
    }
    return true;
}

bool txn_commit(sqlite3 *db) {
    if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return false;
    }
    return true;
}

const char *migrations[] = {
    "CREATE TABLE IF NOT EXISTS Notes (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    title TEXT NOT NULL,\n"
    "    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,\n"
    "    body TEXT\n"
    ");\n",
};

// TODO: can we just extract webc_path from db somehow?
bool create_schema(sqlite3 *db, const char *webc_path) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;
    if (!txn_begin(db)) return_defer(false);

    if (sqlite3_exec(db,
            "CREATE TABLE IF NOT EXISTS Migrations (\n"
            "    applied_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,\n"
            "    query TEXT NOT NULL\n"
            ");\n",
            NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

    if (sqlite3_prepare_v2(db, "SELECT query FROM Migrations;", -1, &stmt, NULL)!= SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

    size_t index = 0;
    int ret = sqlite3_step(stmt);
    for (; ret == SQLITE_ROW; ++index) {
        if (index >= ARRAY_LEN(migrations)) {
            fprintf(stderr, "ERROR: %s: Database scheme is too new. Contains "
                    "more migrations applied than expected. Update your "
                    "application.\n", webc_path);
            return_defer(false);
        }
        const char *query = (const char *)sqlite3_column_text(stmt, 0);
        if (strcmp(query, migrations[index]) != 0) {
            fprintf(stderr, "ERROR: %s: Invalid database scheme. Mismatch in "
                    "migration %zu:\n", webc_path, index);
            fprintf(stderr, "EXPECTED: %s\n", migrations[index]);
            fprintf(stderr, "FOUND: %s\n", query);
            return_defer(false);
        }
        ret = sqlite3_step(stmt);
    }

    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_finalize(stmt);
    stmt = NULL;

    for (; index < ARRAY_LEN(migrations); ++index) {
        printf("INFO: %s: applying migration %zu\n", webc_path, index);
        if (WEBC_TRACE_MIGRATION_QUERIES) printf("%s\n", migrations[index]);
        if (sqlite3_exec(db, migrations[index], NULL, NULL, NULL) != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }

        int ret = sqlite3_prepare_v2(db, "INSERT INTO Migrations (query) VALUES (?)", -1, &stmt, NULL);
        if (ret != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }

        if (sqlite3_bind_text(stmt, 1, migrations[index], strlen(migrations[index]), NULL) != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }

        sqlite3_finalize(stmt);
        stmt = NULL;
    }

defer:
    if (stmt) sqlite3_finalize(stmt);
    if (result) result = txn_commit(db);
    return result;
}

sqlite3 *open_webc_db(void) {
    sqlite3 *result = NULL;

    int exists = file_exists(WEBC_DIR_PATH);
    if (exists < 0) return_defer(NULL);
    bool webc_dir_is_symlink = false;
    if (!exists) {
        if (!mkdir_if_not_exists(WEBC_DIR_PATH)) return_defer(NULL);
    } else {
        File_Type type = get_file_type(WEBC_DIR_PATH);
        if (type < 0) return_defer(NULL);
        switch (type) {
        case FILE_DIRECTORY: break;
        case FILE_REGULAR: {
            nob_log(INFO, "%s is a file! Migrating it to a directory...", WEBC_DIR_PATH);
            const char *webc_tmp_db_path = temp_sprintf("%s.tmp", WEBC_DIR_PATH);
            if (!nob_rename(WEBC_DIR_PATH, webc_tmp_db_path)) return_defer(NULL);
            if (!mkdir_if_not_exists(WEBC_DIR_PATH)) return_defer(NULL);
            if (!nob_rename(webc_tmp_db_path, WEBC_DB_PATH)) return_defer(NULL);
        } break;
        case FILE_SYMLINK: {
            webc_dir_is_symlink = true;
        } break;
        case FILE_OTHER: {
            fprintf(stderr, "ERROR: %s is a weird file! We expect it to be a "
                    "directory or a regular file in case of a legacy database...\n", WEBC_DIR_PATH);
            return_defer(NULL);
        } break;
        }
    }

    int ret = sqlite3_open(WEBC_DB_PATH, &result);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "ERROR: %s: %s\n", WEBC_DB_PATH, sqlite3_errstr(ret));
        if (webc_dir_is_symlink) {
            fprintf(stderr, "NOTE: Your %s is a symlink! We used to expect this "
                    "path to lead to an sqlite3 database file, but at some point "
                    "we changed it to a directory. And now the database file is "
                    "expected to be at %s. If you are using some clever symlink "
                    "setup, please update it accordingly so we could open %s as "
                    "the sqlite3 database.\n", WEBC_DIR_PATH, WEBC_DB_PATH, WEBC_DB_PATH);
        }
        return_defer(NULL);
    }

    if (!create_schema(result, WEBC_DB_PATH)) {
        sqlite3_close(result);
        return_defer(NULL);
    }

defer:
    return result;
}

typedef struct {
    int id;
    const char *title;
    const char *created_at;
    const char *body;
} Note;

typedef struct {
    Note *items;
    size_t count;
    size_t capacity;
} Notes;

bool load_notes(sqlite3 *db, Notes *notes) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    int ret = sqlite3_prepare_v2(db,
        "SELECT id, title, datetime(created_at, 'localtime'), body FROM Notes ORDER BY created_at DESC;",
        -1, &stmt, NULL);
    if (ret != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        int column = 0;
        int id = sqlite3_column_int(stmt, column++);
        const char *title = (const char *)sqlite3_column_text(stmt, column++);
        const char *created_at = (const char *)sqlite3_column_text(stmt, column++);
        const char *body = (const char *)sqlite3_column_text(stmt, column++);
        da_append(notes, ((Note) {
            .id         = id,
            .title      = title      ? temp_strdup(title)      : NULL,
            .created_at = created_at ? temp_strdup(created_at) : NULL,
            .body       = body       ? temp_strdup(body)       : NULL,
        }));
    }

    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    if (stmt) sqlite3_finalize(stmt);
    return result;
}

typedef struct Command {
    const char *name;
    const char *description;
    const char *signature;      // NULL means no signature
    bool (*run)(struct Command *self, const char *program_name, int argc, char **argv);
} Command;

typedef enum {
    DESCRIPTION_SHORT,
    DESCRIPTION_FULL,
} Description_Type;

void command_describe(struct Command command,
                      const char *program_name,
                      int pad,
                      Description_Type description_type) {
    printf("%*s%s %s", pad, "", program_name, command.name);
    if (command.signature) printf(" %s", command.signature);
    printf("\n");
    if (command.description) {
        switch (description_type) {
        case DESCRIPTION_SHORT: {
            String_View description = sv_from_cstr(command.description);
            String_View short_description = sv_chop_by_delim(&description, '\n');
            printf("%*s    "SV_Fmt"\n", pad, "", SV_Arg(short_description));
            String_View rest = sv_trim(description);
            if (rest.count != 0) printf("%*s    ...\n", pad, "");
        } break;
        case DESCRIPTION_FULL: {
            String_View description = sv_from_cstr(command.description);
            while (description.count > 0) {
                String_View line = sv_chop_by_delim(&description, '\n');
                printf("%*s    "SV_Fmt"\n", pad, "", SV_Arg(line));
            }
        } break;
        default: UNREACHABLE("description_type");
        }
    }
}

bool version_run(Command *self,
                 const char *program_name,
                 int argc, char **argv) {
    UNUSED(self);
    UNUSED(program_name);
    UNUSED(argc);
    UNUSED(argv);
    fprintf(stderr, "WEBC BUILD TIME:   "WEBC_BUILD_TIME"\n");
    fprintf(stderr, "SQLITE3 VERSION:   "SQLITE_VERSION"\n");
    return true;
}

typedef struct {
    int client_fd;
    String_Builder request;
    String_Builder response;
    String_Builder body;
} Serve_Context;

void sc_reset(Serve_Context *sc) {
    sc->body.count = 0;
    sc->response.count = 0;
    sc->request.count = 0;
}

Resource *find_resource(const char *file_path) {
    for (size_t i = 0; i < resources_count; ++i) {
        if (strcmp(file_path, resources[i].file_path) == 0) {
            return &resources[i];
        }
    }
    return NULL;
}

bool write_entire_sv(int fd, String_View sv) {
    String_View untransfered = sv;
    while (untransfered.count > 0) {
        ssize_t transfered = write(fd, untransfered.data, untransfered.count);
        if (transfered < 0) {
            fprintf(stderr, "ERROR: Could not write response: %s\n", strerror(errno));
            return false;
        }
        untransfered.data += transfered;
        untransfered.count -= transfered;
    }
    return true;
}

const char *http_reason_phrase_by_status_code(int status_code) {
    // Taken from https://gist.github.com/josantonius/0a889ab6f18db2fcefda15a039613293
    static const char *reason_phrases[] = {
        [100] = "Continue",
        [101] = "Switching Protocols",
        [200] = "OK",
        [201] = "Created",
        [202] = "Accepted",
        [204] = "No Content",
        [301] = "Moved Permanently",
        [302] = "Found",
        [304] = "Not Modified",
        [400] = "Bad Request",
        [401] = "Unauthorized",
        [403] = "Forbidden",
        [404] = "Not Found",
        [405] = "Method Not Allowed",
        [408] = "Request Time-out",
        [413] = "Payload Too Large",
        [500] = "Internal Server Error",
        [501] = "Not Implemented",
        [503] = "Service Unavailable",
    };

    if (!((size_t)status_code < ARRAY_LEN(reason_phrases)) || reason_phrases[status_code] == NULL) {
        return "Unknown";
    }
    return reason_phrases[status_code];
}

void http_render_response(String_Builder *response,
                          int status_code,
                          const char *content_type,
                          String_View body) {
    sb_append_cstr(response, temp_sprintf("HTTP/1.1 %d %s\r\n", status_code, http_reason_phrase_by_status_code(status_code)));
    sb_append_cstr(response, temp_sprintf("Content-Type: %s\r\n", content_type));
    sb_append_cstr(response, "Last-Modified: "WEBC_BUILD_TIME"\r\n");
    sb_append_cstr(response, temp_sprintf("Content-Length: %zu\r\n", body.count));
    sb_append_cstr(response, "Connection: close\r\n");
    sb_append_cstr(response, "\r\n");
    sb_append_buf(response, body.data, body.count);
}

void render_page_shell(String_Builder *sb,
                       String_View title,
                       String_View content) {
    sb_append_cstr(sb, "<!DOCTYPE html>\n");
    sb_append_cstr(sb, "<html lang=\"en\"><head>");
    sb_append_cstr(sb, "<meta charset=\"utf-8\">");
    sb_append_cstr(sb, "<title>");
    sb_append_sv(sb, title);
    sb_append_cstr(sb, "</title>");
    sb_append_cstr(sb, "<link rel=\"stylesheet\" href=\"/css/output.css\">");
    sb_append_cstr(sb, "</head><body>");
    sb_append_sv(sb, content);
    sb_append_cstr(sb, "</body></html>");
}

void serve_error(Serve_Context *sc, int status_code) {
    String_View title = sv_from_cstr(temp_sprintf("%d %s", status_code, http_reason_phrase_by_status_code(status_code)));
    String_Builder content = {0};
    sb_append_cstr(&content, "<div class=\"p-8\"><h1 class=\"text-3xl font-bold\">");
    sb_append_sv(&content, title);
    sb_append_cstr(&content, "</h1></div>");
    render_page_shell(&sc->body, title, sb_to_sv(content));
    http_render_response(&sc->response, status_code, "text/html", sb_to_sv(sc->body));
    UNUSED(write_entire_sv(sc->client_fd, sb_to_sv(sc->response)));
}

void serve_resource(Serve_Context *sc,
                    const char *resource_path,
                    const char *content_type) {
    Resource *resource = find_resource(resource_path);
    if (!resource) {
        serve_error(sc, 404);
        return;
    }

    String_View body = {
        .data  = (char *)&bundle[resource->offset],
        .count = resource->size,
    };
    http_render_response(&sc->response, 200, content_type, body);
    UNUSED(write_entire_sv(sc->client_fd, sb_to_sv(sc->response)));
}

void serve_version_page(Serve_Context *sc) {
    String_Builder content = {0};
    sb_append_cstr(&content, "<div class=\"p-8\"><h1>webc</h1>");
    sb_append_cstr(&content, "<p>Build time: "WEBC_BUILD_TIME"</p>");
    sb_append_cstr(&content, "<p>SQLite: "SQLITE_VERSION"</p>");
    sb_append_cstr(&content, "</div>");
    render_page_shell(&sc->body, sv_from_cstr("webc version"), sb_to_sv(content));
    http_render_response(&sc->response, 200, "text/html", sb_to_sv(sc->body));
    UNUSED(write_entire_sv(sc->client_fd, sb_to_sv(sc->response)));
}

void serve_notes(Serve_Context *sc) {
    bool result = true;
    Notes notes = {0};
    sqlite3 *db = open_webc_db();
    if (!db) return_defer(false);
    if (!txn_begin(db)) return_defer(false);
    if (!load_notes(db, &notes)) {
        serve_error(sc, 500);
        return_defer(false);
    }

    String_Builder content = {0};
    sb_append_cstr(&content, "<div class=\"p-8\"><h1>Notes</h1><ul>");
    for (size_t i = 0; i < notes.count; ++i) {
        Note *note = &notes.items[i];
        sb_append_cstr(&content, "<li>");
        sb_append_cstr(&content, note->title ? note->title : "");
        sb_append_cstr(&content, " (");
        sb_append_cstr(&content, note->created_at ? note->created_at : "");
        sb_append_cstr(&content, ")</li>");
    }
    sb_append_cstr(&content, "</ul></div>");
    render_page_shell(&sc->body, sv_from_cstr("Notes"), sb_to_sv(content));
    http_render_response(&sc->response, 200, "text/html", sb_to_sv(sc->body));
    UNUSED(write_entire_sv(sc->client_fd, sb_to_sv(sc->response)));

defer:
    if (db) {
        if (result) result = txn_commit(db);
        sqlite3_close(db);
    }
    free(notes.items);
}

void serve_request(Serve_Context *sc) {
    // TODO: log HTTP queries

    char buffer[1024];
    size_t cur = 0;
    String_View suffix = sv_from_parts("\r\n\r\n", 4);
    bool finish = false;
    ssize_t n = 0;
    do {
        n = read(sc->client_fd, buffer, sizeof(buffer));
        if (n <= 0) break;
        sb_append_buf(&sc->request, buffer, n);
        for (; cur < sc->request.count && !finish; cur += 1) {
            finish = sv_starts_with(sv_from_parts(sc->request.items + cur, sc->request.count - cur), suffix);
        }
    } while (!finish);

    if (n < 0) {
        fprintf(stderr, "ERROR: could not read request: %s", strerror(errno));
        return;
    }

    // NULL terminating the request buffer, just in case we need to use some stupid libc functions
    // that only work with NULL-terminated strings.
    sb_append_null(&sc->request);

    String_View request = sb_to_sv(sc->request);
    String_View status_line = sv_trim(sv_chop_by_delim(&request, '\n'));
    String_View method = sv_trim(sv_chop_by_delim(&status_line, ' '));
    UNUSED(method);
    String_View uri = sv_trim(sv_chop_by_delim(&status_line, ' '));

    // Drop the query part of the URI, if it exists
    for (size_t i = 0; i < uri.count; ++i) {
        if (uri.data[i] == '?') {
            uri.count = i;
            break;
        }
    }

    if (sv_eq(uri, sv_from_cstr("/"))) {
        serve_resource(sc, "./index.html", "text/html; charset=utf-8");
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/index.html"))) {
        serve_resource(sc, "./index.html", "text/html; charset=utf-8");
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/version"))) {
        serve_version_page(sc);
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/notes"))) {
        serve_notes(sc);
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/css/output.css"))) {
        serve_resource(sc, "./css/output.css", "text/css; charset=utf-8");
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/favicon.ico"))) {
        serve_resource(sc, "./resource/image/user1.png", "image/png");
        return;
    }
    if (sv_starts_with(uri, sv_from_cstr("/resource/"))) {
        String_View resource_prefix = sv_from_cstr("/resource/");
        String_View rest = {
            .data  = uri.data + resource_prefix.count,
            .count = uri.count - resource_prefix.count,
        };
        // Map "/resource/image/user1.png" -> "./resource/image/user1.png"
        String_Builder path = {0};
        sb_append_cstr(&path, "./resource/");
        sb_append_sv(&path, rest);
        sb_append_null(&path);
        Resource *r = find_resource(path.items);
        if (!r) {
            serve_error(sc, 404);
            return;
        }
        const char *content_type = "application/octet-stream";
        if (sv_ends_with(rest, sv_from_cstr(".png")))  content_type = "image/png";
        if (sv_ends_with(rest, sv_from_cstr(".jpg")))  content_type = "image/jpeg";
        if (sv_ends_with(rest, sv_from_cstr(".svg")))  content_type = "image/svg+xml";
        if (sv_ends_with(rest, sv_from_cstr(".css")))  content_type = "text/css; charset=utf-8";
        if (sv_ends_with(rest, sv_from_cstr(".js")))   content_type = "text/javascript; charset=utf-8";
        if (sv_ends_with(rest, sv_from_cstr(".html"))) content_type = "text/html; charset=utf-8";
        serve_resource(sc, path.items, content_type);
        return;
    }

    serve_error(sc, 404);
}

bool serve_run(Command *self, const char *program_name, int argc, char **argv) {
    UNUSED(self);
    UNUSED(program_name);

    bool result = true;
    // NOTE: We are intentionally not listening to the external addresses, because we are using a
    // custom scuffed implementation of HTTP protocol, which is incomplete and possibly insecure.
    // The `serve` command is meant to be used only locally by a single person. At least for now.
    // We are doing it for the sake of simplicity, 'cause we don't have to ship an entire proper
    // HTTP server. Though, if you really want to, you can always slap some reverse proxy like nginx
    // on top of the `serve`.
    const char *addr = "127.0.0.1";
    uint16_t port = DEFAULT_SERVE_PORT;
    if (argc > 0) port = atoi(shift(argv, argc));

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        fprintf(stderr, "ERROR: Could not create socket epicly: %s\n", strerror(errno));
        return_defer(false);
    }

    int option = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(addr);

    ssize_t err = bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (err != 0) {
        fprintf(stderr, "ERROR: Could not bind socket epicly: %s\n", strerror(errno));
        return_defer(false);
    }

    err = listen(server_fd, 69);
    if (err != 0) {
        fprintf(stderr, "ERROR: Could not listen to socket, it's too quiet: %s\n", strerror(errno));
        return_defer(false);
    }

    printf("Listening to http://%s:%d/\n", addr, port);

    Serve_Context sc = {0};
    for (;;) {
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = 0;
        sc.client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addrlen);
        if (sc.client_fd < 0) {
            fprintf(stderr, "ERROR: Could not accept connection. This is unacceptable! %s\n", strerror(errno));
            continue;
        }

        UNUSED(serve_request(&sc));

        shutdown(sc.client_fd, SHUT_WR);
        char buffer[4096];
        while (read(sc.client_fd, buffer, sizeof(buffer)) > 0);
        close(sc.client_fd);
        sc_reset(&sc);
        temp_reset();
    }

    // TODO: The only way to stop the server is by SIGINT, but that probably doesn't close the db correctly.
    // So we probably should add a SIGINT handler specifically for this.

    UNREACHABLE("serve");

defer:
    // TODO: properly close the sockets on defer
    return result;
}

bool help_run(Command *self, const char *program_name, int argc, char **argv);

static Command commands[] = {
    {
        .name = "serve",
        .signature = "[port]",
        .description = "Start up the Web Server. Default port is " STR(DEFAULT_SERVE_PORT) ".",
        .run = serve_run,
    },
    {
        .name = "help",
        .signature = "[command]",
        .description = "Show help messages for commands",
        .run = help_run,
    },
    {
        .name = "version",
        .signature = NULL,
        .description = "Show current version",
        .run = version_run,
    },
};

void usage(const char *program_name) {
    printf("Usage: %s [command] [command-arguments]\n", program_name);
    printf("\n");
    printf("Commands:\n");
    for (size_t i = 0; i < ARRAY_LEN(commands); ++i) {
        command_describe(commands[i], program_name, 2, DESCRIPTION_SHORT);
        printf("\n");
    }
    printf("The default command is `"DEFAULT_COMMAND"`.\n");
}

bool help_run(Command *self, const char *program_name, int argc, char **argv) {
    UNUSED(self);
    const char *command_name = NULL;
    if (argc > 0) command_name = shift(argv, argc);

    if (command_name) {
        size_t match_count = 0;
        Command *last_match = NULL;
        for (size_t i = 0; i < ARRAY_LEN(commands); ++i) {
            if (sv_starts_with(sv_from_cstr(commands[i].name), sv_from_cstr(command_name))) {
                last_match = &commands[i];
                match_count += 1;
            }
        }
        switch (match_count) {
        case 0:
            fprintf(stderr, "ERROR: unknown command `%s`\n", command_name);
            return false;
        case 1:
            command_describe(*last_match, program_name, 0, DESCRIPTION_FULL);
            return true;
        default:
            printf("Commands matching prefix `%s`:\n", command_name);
            for (size_t i = 0; i < ARRAY_LEN(commands); ++i) {
                if (sv_starts_with(sv_from_cstr(commands[i].name), sv_from_cstr(command_name))) {
                    command_describe(commands[i], program_name, 2, DESCRIPTION_SHORT);
                    printf("\n");
                }
            }
            return true;
        }
    }

    usage(program_name);
    return true;
}

int main(int argc, char **argv) {
    int result = 0;

    HOME_PATH = getenv("HOME");
    if (HOME_PATH == NULL) {
        fprintf(stderr, "ERROR: No $HOME environment variable is setup. We "
                "need it to find the location of ~/.webc/ directory.\n");
        return 1;
    }
    WEBC_DIR_PATH = strdup(temp_sprintf("%s/.webc", HOME_PATH));
    WEBC_DB_PATH = strdup(temp_sprintf("%s/db", WEBC_DIR_PATH));
    WEBC_TRACE_MIGRATION_QUERIES = getenv("WEBC_TRACE_MIGRATION_QUERIES") != NULL;

    const char *program_name = shift(argv, argc);
    const char *command_name = DEFAULT_COMMAND;
    if (argc > 0) command_name = shift(argv, argc);

    if (strcmp(command_name, "--help") == 0 || strcmp(command_name, "-h") == 0) {
        command_name = "help";
    }

    for (size_t i = 0; i < ARRAY_LEN(commands); ++i) {
        if (strcmp(commands[i].name, command_name) == 0) {
            if (!commands[i].run(&commands[i], program_name, argc, argv)) return_defer(1);
            return_defer(0);
        }
    }

    fprintf(stderr, "ERROR: unknown command `%s`\n", command_name);
    return_defer(1);

defer:
    return result;
}
