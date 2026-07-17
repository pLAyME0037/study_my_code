#include "third_party/hash_table.h"

int main (int argc, char **argv) {
    const char *program = shift_args(&argc, &argv);
    Nob_String_Builder buffer = {0};

    if (argc <= 0) {
        nob_log(ERROR, "No Input was Provided");
        nob_log(INFO, "Usage: %s <input.txt>", program);
        return 1;
    }
    const char *file_path = shift_args(&argc, &argv);

    if (!read_entire_file(file_path, &buffer)) return 1;

    Nob_String_View content = {
        .data  = buffer.items,
        .count = buffer.count,
    };


    // native_analysis(content, file_path);
    // if (!hash_analysis(content, file_path)) return 1;
    find_sensetive_data(content, "data.json");

    return 0;
}
