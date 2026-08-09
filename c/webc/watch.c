#define _GNU_SOURCE
#include <sys/inotify.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define WATCH_PORT "8000"
#define SERVER_BIN "./bin/webc"

static int inotify_fd;
static Nob_Proc server_proc;

typedef struct {
    int wd;
    const char *path;
} WD_Map;

static WD_Map wd_map[2048];
static size_t wd_count;

static bool is_ignored_path(const char *path) {
    if (strstr(path, "/auto_ctrl") != NULL) return true;
    if (strstr(path, "/bin/") != NULL) return true;
    if (strstr(path, "/build/") != NULL) return true;
    if (strstr(path, "/src/sqlite-amalgamation-3460100") != NULL) return true;
    if (strstr(path, "/.git/") != NULL) return true;
    if (strstr(path, "/display/css/tailwindcss") != NULL) return true;
    if (strstr(path, "/css/output.css") != NULL) return true;
    if (strstr(path, "/index.html") != NULL) return true;
    if (strstr(path, "/index.h") != NULL) return true;
    return false;
}

static bool is_source_ext(const char *name) {
    if (sv_ends_with_cstr(sv_from_cstr(name), ".tt")) return true;
    if (sv_ends_with_cstr(sv_from_cstr(name), ".c")) return true;
    if (sv_ends_with_cstr(sv_from_cstr(name), ".h")) return true;
    if (sv_ends_with_cstr(sv_from_cstr(name), ".css")) return true;
    return false;
}

static const char *path_for_wd(int wd) {
    for (size_t i = 0; i < wd_count; ++i) {
        if (wd_map[i].wd == wd) return wd_map[i].path;
    }
    return ".";
}

static void register_wd(int wd, const char *path) {
    if (wd < 0) return;
    for (size_t i = 0; i < wd_count; ++i) {
        if (wd_map[i].wd == wd) return;
    }
    if (wd_count >= NOB_ARRAY_LEN(wd_map)) return;
    wd_map[wd_count].wd = wd;
    wd_map[wd_count].path = strdup(path);
    wd_count += 1;
}

static void add_dir_recursive(const char *path) {
    if (is_ignored_path(path)) return;

    struct stat st;
    if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) return;

    register_wd(inotify_add_watch(inotify_fd, path,
        IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO), path);

    DIR *d = opendir(path);
    if (!d) return;
    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (e->d_name[0] == '.') continue;
        if (e->d_type != DT_DIR) continue;
        char sub[PATH_MAX];
        snprintf(sub, sizeof sub, "%s/%s", path, e->d_name);
        add_dir_recursive(sub);
    }
    closedir(d);
}

static void strip_trailing_nl(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) s[--n] = '\0';
}

static int run_build(void) {
    Cmd cmd = {0};
    nob_log(INFO, "rebuilding...");
    cmd_append(&cmd, "./bin/nob");
    return cmd_run_sync_and_reset(&cmd) ? 0 : 1;
}

static void stop_server(void) {
    if (server_proc == 0) return;
    nob_log(INFO, "stopping webc server (pid %d)", server_proc);
    kill(server_proc, SIGTERM);
    nob_proc_wait(server_proc);
    server_proc = 0;
}

static void start_server(const char *port) {
    Cmd cmd = {0};
    cmd_append(&cmd, SERVER_BIN, "serve", port);
    Nob_Procs procs = {0};
    if (!cmd_run(&cmd, .async = &procs)) {
        nob_log(ERROR, "failed to spawn %s", SERVER_BIN);
        return;
    }
    cmd.count = 0;
    server_proc = procs.items[0];
    nob_log(INFO, "webc server started (pid %d) on :%s", server_proc, port);
    free(procs.items);
}

static void restart_server(const char *port) {
    stop_server();
    start_server(port);
}


int main(void) {
    nob_log(INFO, "watch: hot reload for webc pipeline");
    const char *port = getenv("PORT");
    if (port == NULL || port[0] == '\0') port = WATCH_PORT;

    inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        nob_log(ERROR, "inotify_init failed: %s", strerror(errno));
        return 1;
    }

    add_dir_recursive(".");
    nob_log(INFO, "watching source dirs (debounce 250ms, server on :%s)", port);

    if (run_build() == 0) start_server(port);
    else stop_server();

    bool dirty = false;
    const struct timeval debounce = {0, 250000};
    char buf[sizeof(struct inotify_event) + NAME_MAX + 1];

    for (;;) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(inotify_fd, &fds);
        struct timeval tv = debounce;
        int r = select(inotify_fd + 1, &fds, NULL, NULL, &tv);

        if (r < 0) return 1;
        ssize_t n = read(inotify_fd, buf, sizeof buf);
        for (ssize_t i = 0; i <= n - (ssize_t)sizeof(struct inotify_event);) {
            struct inotify_event *ev = (struct inotify_event *)(buf + i);
            if (ev->mask & (IN_IGNORED | IN_Q_OVERFLOW | IN_UNMOUNT)) {
                i += sizeof(struct inotify_event) + ev->len;
                continue;
            }

            const char *wd_path = path_for_wd(ev->wd);

            if ((ev->mask & IN_CREATE) && (ev->mask & IN_ISDIR) && ev->len > 0 && ev->name[0] != '.') {
                char full[PATH_MAX];
                snprintf(full, sizeof full, "%s/%s", wd_path, ev->name);
                if (!is_ignored_path(full)) add_dir_recursive(full);
            }

            if (ev->len > 0) {
                char full[PATH_MAX];
                snprintf(full, sizeof full, "%s/%s", wd_path, ev->name);
                if (!is_ignored_path(full) && is_source_ext(ev->name)) {
                    strip_trailing_nl(full);
                    nob_log(INFO, "change: %s", full);
                    dirty = true;
                }
            }

            i += sizeof(struct inotify_event) + ev->len;
        }

        if (r != 0 && !dirty) return 1;
        dirty = false;
        if (run_build() == 0) {
            restart_server(port);
        } else {
            nob_log(ERROR, "build failed, not reloading");
        }
    }

    return 0;
}
