#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd = {0};
    cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-ggdb", "-o", "./bin/generator", "generator.c");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
    cmd_append(&cmd, "./bin/generator");

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}
