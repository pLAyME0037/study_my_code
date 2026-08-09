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

#include "core/serve.h"
#include "core/route.h"
#include "src/db.h"
#include "build/bundle.h"

#define STR(x) STR2_ELECTRIC_BOOGALOO(x)
#define STR2_ELECTRIC_BOOGALOO(x) #x
#define DEFAULT_SERVE_PORT 8000
#define DEFAULT_COMMAND "help"

// Computed at runtime in main()
static const char *HOME_PATH = NULL;

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
    return true;
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
