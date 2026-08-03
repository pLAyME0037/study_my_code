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

bool json_append_escaped(String_Builder *sb, const char *s, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        char c = s[i];
        if (c == '"')       sb_append_cstr(sb, "\\\"");
        else if (c == '\\') sb_append_cstr(sb, "\\\\");
        else if (c == '\n') sb_append_cstr(sb, "\\n");
        else if (c == '\t') sb_append_cstr(sb, "\\t");
        else                sb_append(sb, c);
    }
    return true;
}

bool generate_compile_commands(Cmd *cmd, const char *proj_dir) {
    String_Builder render = {0};
    cmd_render(*cmd, &render);
    sb_append_null(&render);

    String_Builder escaped = {0};
    json_append_escaped(&escaped, render.items, render.count - 1);

    String_Builder json = {0};
    sb_appendf(&json,
        "[\n"
        "  {\n"
        "    \"directory\": \"%s\",\n"
        "    \"command\": \"",
        proj_dir);
    json_append_escaped(&json, escaped.items, escaped.count);
    sb_appendf(&json,
        "\",\n"
        "    \"file\": \"main.c\"\n"
        "  }\n"
        "]\n");

    bool result = write_entire_file("compile_commands.json", json.items, json.count);

    sb_free(render);
    sb_free(escaped);
    sb_free(json);
    return result;
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
    cmd_append(&cmd, "./ui/table_tab.c");
    cmd_append(&cmd, "./ui/text_tab.c");
    cmd_append(&cmd, "./ui/review_tab.c");
    cmd_append(&cmd, "./coreui/window_core.c");
    cmd_append(&cmd, "./coreui/review_tab_core.c");
    cmd_append(&cmd, "./coreui/text_tab_core.c");
    cmd_append(&cmd, "./third_party/hash_table.c");
    cmd_append(&cmd, "./third_party/json_format.c");
    if (!add_pkg_config(&cmd, "--libs", "gtk4")) return 1;
    cmd_append(&cmd, "-lm", "-ljson-c");

    const char *proj_dir = get_current_dir_temp();
    generate_compile_commands(&cmd, proj_dir);

    if (!cmd_run(&cmd)) return 1;

    Cmd test_cmd = {0};
    cmd_append(&test_cmd, "cc", "-Wall", "-Wextra", "-ggdb", "-I.");
    cmd_append(&test_cmd, "-o", "./build/bin/test_hash_analysis");
    cmd_append(&test_cmd, "./tests/test_hash_analysis.c");
    cmd_append(&test_cmd, "./third_party/hash_table.c");
    cmd_append(&test_cmd, "-lm");
    if (!cmd_run(&test_cmd)) return 1;

    cmd_append(&test_cmd, "./build/bin/test_hash_analysis");
    if (!cmd_run(&test_cmd)) return 1;

    cmd_append(&cmd, "./build/bin/main");
    if (!cmd_run(&cmd)) return 1;

    return 0;
}

