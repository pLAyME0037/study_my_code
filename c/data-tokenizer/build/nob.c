#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd = {0};
    cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-ggdb", "-o", "./build/bin/main", 
               "./main.c", "./third_party/hash_table.c", "./third_party/json_format.c", "-ljson-c");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
    cmd_append(&cmd, "./build/bin/main");

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}

