#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int prepare_cttochtml(Cmd cmd) {
    mkdir_if_not_exists("./auto_ctrl");
    mkdir_if_not_exists("./auto_ctrl/cttochtml");

    Dir_Entry dir = {0};
    if (!dir_entry_open("./display", &dir)) return 1;

    while (dir_entry_next(&dir)) {
        String_View name = sv_from_cstr(dir.name);
        if (sv_eq(name, sv_from_cstr(".")) || sv_eq(name, sv_from_cstr(".."))) continue;
        if (!sv_ends_with_cstr(name, ".tt")) continue;

        String_Builder input = {0};
        sb_append_cstr(&input, "./display/");
        sb_append_sv(&input, name);
        sb_append_null(&input);

        String_View base = name;
        sv_chop_suffix(&base, sv_from_cstr(".tt"));
        String_Builder output = {0};
        sb_append_cstr(&output, "./auto_ctrl/cttochtml/");
        sb_append_sv(&output, base);
        sb_append_null(&output);

        Fd out_fd = fd_open_for_write(output.items);
        if (out_fd == INVALID_FD) return 1;

        cmd_append(&cmd, "./bin/tt", input.items);
        if (!cmd_run_sync_redirect_and_reset(&cmd, (Nob_Cmd_Redirect) {
            .fdout = &out_fd }))
        {
            return 1;
        } 
    }

    dir_entry_close(dir);
    return 0;
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd = {0};

    cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-Wswitch-enum", "-ggdb", "-o", "./bin/tt", "tt.c");
    if (!cmd_run_sync_and_reset(&cmd)) return 1;

    if (prepare_cttochtml(cmd)) return 1;

    cmd_append(&cmd, "./bin/tailwindcss-linux-x64", "-i", "css/input.css", "-o", "css/output.css", "--minify");
    if (!cmd_run_sync_and_reset(&cmd)) return 1;

    Fd index_fd = fd_open_for_write("./index.h");
    if (index_fd == INVALID_FD) return 1;

    cmd_append(&cmd, "./bin/tt", "./display/body.h.tt");
    if (!cmd_run_sync_redirect_and_reset(&cmd, (Nob_Cmd_Redirect) {
        .fdout = &index_fd
    })) return 1;

    cmd_append(&cmd, "cc", "-o", "./bin/index", "index.c");
    if (!nob_cmd_run(&cmd)) return 1;

    Fd html_fd = fd_open_for_write("index.html");
    if (html_fd == INVALID_FD) return 1;

    cmd_append(&cmd, "./bin/index");
    if (!cmd_run_sync_redirect_and_reset(&cmd, (Nob_Cmd_Redirect) {
        .fdout = &html_fd
    })) return 1;

    return 0;
}

