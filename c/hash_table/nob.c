#include <string.h>
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *program = shift_args(&argc, &argv);

    Cmd cmd = {0};
    cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-ggdb", "-pedantic", "-o", "./bin/hash_table", "hash_table.c");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

    if (argc > 0) {
        const char *subcmd = shift_args(&argc, &argv);

        if (strcmp(subcmd, "run") == 0) {
            cmd.count = 0;
            cmd_append(&cmd, "./bin/hash_table");
            da_append_many(&cmd, argv, argc);
            if (!nob_cmd_run_sync(cmd)) return 1;
        } else {
            nob_log(ERROR, "ERROR Unknown argv: %s", subcmd);
            return 1;
        }
    }

    return 0;
}
