#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

bool add_pkg_config(Cmd *cmd, const char *flag, const char *pkg) {
    Cmd pkg_cmd = {0};
    cmd_append(&pkg_cmd, "pkg-config", flag, pkg);

    const char *tmp_path = ".pkg-config.tmp";

    if (!cmd_run(&pkg_cmd, .stdout_path = tmp_path)) return false;

    String_Builder sb = {0};
    if (!read_entire_file(tmp_path, &sb)) return false;

    String_View sv = sb_to_sv(sb);
    while (sv.count > 0) {
        sv = sv_trim_left(sv);
        if (sv.count == 0) break;
        String_View arg = sv_chop_by_space(&sv);
        cmd_append(cmd, temp_sv_to_cstr(arg));
    }
    sb_free(sb);
    delete_file(tmp_path);

    return true;
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!mkdir_if_not_exists("build")) return 1;
    if (!mkdir_if_not_exists("build/bin")) return 1;

    Cmd cmd = {0};
    cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-ggdb", "-I.");
    if (!add_pkg_config(&cmd, "--cflags", "gtk4")) return 1;
    cmd_append(&cmd, "-o", "./build/bin/main", "./main.c");
    cmd_append(&cmd, "./ui/window.c");
    cmd_append(&cmd, "./third_party/hash_table.c");
    cmd_append(&cmd, "./third_party/json_format.c");
    if (!add_pkg_config(&cmd, "--libs", "gtk4")) return 1;
    cmd_append(&cmd, "-lm", "-ljson-c");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

    cmd_append(&cmd, "./build/bin/main");
    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}

