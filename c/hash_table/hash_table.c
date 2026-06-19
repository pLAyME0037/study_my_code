#include <stdio.h>
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

typedef struct {
    String_View key;
    size_t value;
} FreqKeyVal;

typedef struct {
    FreqKeyVal *items;
    size_t count;
    size_t capacity;
} FreqKeyVals;

FreqKeyVal *find_key(FreqKeyVals haystack, String_View needle) {
    for (size_t i = 0; i < haystack.count; ++i) {
        if (sv_eq(haystack.items[i].key, needle)) {
            return &haystack.items[i];
        }
    }
    return NULL;
}

int compare_freqkeyval_count(const void *a, const void *b) {
    const FreqKeyVal *akv = a;
    const FreqKeyVal *bkv = b;
    return (int)bkv->value - (int)akv->value;
}

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

    nob_log(INFO, "Analying: %s", file_path);
    nob_log(INFO, "  Size: %zu bytes", buffer.count);

    Nob_String_View content = {
        .data  = buffer.items,
        .count = buffer.count,
    };

    FreqKeyVals freq = {0};

    size_t count = 0;
    for (; content.count > 0; ++count) {
        content = sv_trim_left(content);
        Nob_String_View token = sv_chop_by_space(&content);
        // nob_log(INFO, "  "SV_Fmt, SV_Arg(token));
        FreqKeyVal *kv = find_key(freq, token);
        if (kv) {
            kv->value += 1;
        } else {
            da_append(&freq, ((FreqKeyVal) {
                .key = token,
                .value = 1,
            }));
        }
    }

    qsort(freq.items, freq.count, sizeof(freq.items[0]), compare_freqkeyval_count);
    nob_log(INFO, "  Top 10 tokens within %zu token", count);
    for (size_t i = 0; i < 10 && i < freq.count; ++i) {
        nob_log(INFO, "    %zu: "SV_Fmt" => %zu", i, SV_Arg(freq.items[i].key), freq.items[i].value);
    }

    return 0;
}
