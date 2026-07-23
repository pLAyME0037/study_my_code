#include "third_party/hash_table.h"
#include "third_party/json_format.h"
#include <stdio.h>

int main(void) {
    const char *file_path = "./data_2mb.json";
    String_Builder buffer = {0};

    if (!read_entire_file(file_path, &buffer)) return 1;
    String_View content = {
        .data  = buffer.items,
        .count = buffer.count,
    };

    find_sensetive_data(content, "./data.json");

    return 0;
}
