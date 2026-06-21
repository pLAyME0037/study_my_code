#include <assert.h>
#include <bits/time.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

double delta_secs(struct timespec begin, struct timespec end) {
    double a = (double)begin.tv_sec*1000*1000*1000 + begin.tv_nsec*1e-9;
    double b = (double)end.tv_sec*1000*1000*1000 + end.tv_nsec*1e-9;
    return b - a;
}

int compare_freqkeyval_count(const void *a, const void *b) {
    const FreqKeyVal *akv = a;
    const FreqKeyVal *bkv = b;
    return (int)bkv->value - (int)akv->value;
}

uint32_t hash(uint8_t *buf, size_t buf_size) {
    uint32_t result = 0;
    for (size_t i = 0; i < buf_size; ++i) {
        result += (uint32_t)buf[i];
    }
    return result;
}

void native_analysis(String_View content) {
    FreqKeyVals freq = {0};

    struct timespec begin = {0};
    int ret = clock_gettime(CLOCK_MONOTONIC, &begin);
    assert(ret == 0);

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

    struct timespec end = {0};
    ret = clock_gettime(CLOCK_MONOTONIC, &end);
    assert(ret == 0);

    nob_log(INFO, "  Top 10 tokens within %zu token", count);
    qsort(freq.items, freq.count, sizeof(freq.items[0]), compare_freqkeyval_count);
    for (size_t i = 0; i < 10 && i < freq.count; ++i) {
        nob_log(INFO, "    %zu: "SV_Fmt" => %zu", i, SV_Arg(freq.items[i].key), freq.items[i].value);
    }
    nob_log(INFO, "Elapsed time %.3lfs", delta_secs(begin, end));
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

    size_t n = 1000;
    bool *slots = malloc(sizeof(bool)*n);
    memset(slots, 0, sizeof(bool)*n);

    size_t count = 0;
    for (; count < 10 && content.count > 0; ++count) {
        content = sv_trim_left(content);
        Nob_String_View token = sv_chop_by_space(&content);

        uint32_t h = hash((uint8_t*)token.data, token.count);
        nob_log(INFO, "    %zu: %X = "SV_Fmt, count, h, SV_Arg(token));
    }
    native_analysis(content);

    return 0;
}
