#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd = {0};

    cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-Wswitch-enum", "-ggdb", "-o", "./bin/tt", "tt.c");
    if (!cmd_run_sync_and_reset(&cmd)) return 1;

    Fd index_fd = fd_open_for_write("index.h");
    if (index_fd == INVALID_FD) return 1;

    cmd_append(&cmd, "./bin/tt", "./index.h.tt");
    if (!cmd_run_sync_redirect_and_reset(&cmd, (Nob_Cmd_Redirect) {
        .fdout = &index_fd
    })) return 1;

    cmd_append(&cmd, "cc", "-o", "index", "index.c");
    if (!nob_cmd_run(&cmd)) return 1;

    Fd html_fd = fd_open_for_write("index.html");
    if (html_fd == INVALID_FD) return 1;

    cmd_append(&cmd, "./index");
    if (!cmd_run_sync_redirect_and_reset(&cmd, (Nob_Cmd_Redirect) {
        .fdout = &html_fd
    })) return 1;

    return 0;
}

