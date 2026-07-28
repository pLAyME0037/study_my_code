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
#include "../build/nob.h"

typedef struct {
    String_View key;
    size_t value;
    bool accupied;
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

static double delta_secs(struct timespec begin, struct timespec end) {
    double a = (double)begin.tv_sec + begin.tv_nsec * 1e-9;
    double b = (double)end.tv_sec + end.tv_nsec * 1e-9;
    return b - a;
}

int compare_freqkeyval_count(const void *a, const void *b) {
    const FreqKeyVal *akv = a;
    const FreqKeyVal *bkv = b;
    return (int)bkv->value - (int)akv->value;
}

uint32_t old_hash(uint8_t *buf, size_t buf_size) {
    uint32_t result = 5821;
    for (size_t i = 0; i < buf_size; ++i) {
        result = ((result << 5) + (uint32_t)buf[i]) + i;
    }
    return result;
}

uint32_t djb2(uint8_t *buf, size_t buf_size) {
    uint32_t hash = 5381;

    for (size_t i = 0; i < buf_size; ++i) {
        hash = ((hash << 5) + hash) + (uint32_t)buf[i]; /* hash * 33 + c */
    }

    return hash;
}

void native_analysis(String_View content, const char *file_path) {
    nob_log(INFO, "Analying: %s", file_path);
    nob_log(INFO, "  Size: %zu bytes", content.count);

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
        nob_log(INFO,
                "    %zu: "SV_Fmt" => %zu",
                i,
                SV_Arg(freq.items[i].key),
                freq.items[i].value);
    }
    nob_log(INFO, "Elapsed time %.3lfs", delta_secs(begin, end));
}

#define hash_init(ht, cap) \
    do { \
        (ht)->items = malloc(sizeof(*(ht)->items)*cap); \
        memset((ht)->items, 0, sizeof(*(ht)->items)*cap); \
        (ht)->count = 0; \
        (ht)->capacity = cap; \
    } while (0)

bool hash_analysis(String_View content, const char *file_path) {
    nob_log(INFO, "Analying: %s", file_path);
    nob_log(INFO, "  Size: %zu bytes", content.count);

    FreqKeyVals ht = {0};
    hash_init(&ht, 10000);

    struct timespec begin = {0};
    int ret = clock_gettime(CLOCK_MONOTONIC, &begin);
    assert(ret == 0);

    size_t count = 0;
    UNUSED(count);
    for (; content.count > 0; ++count) {
        content = sv_trim_left(content);
        Nob_String_View token = sv_chop_by_space(&content);

        uint32_t h = djb2((uint8_t*)token.data, token.count)%ht.capacity;
        // nob_log(INFO, "    %zu: 0x%08X = "SV_Fmt, count, h, SV_Arg(token));

        for (size_t i = 0;
             i < ht.capacity 
             && ht.items[h].accupied 
             && !sv_eq(ht.items[h].key, token);
             ++i)
        {
            h = (h + 1)%ht.capacity;
        }

        if (ht.items[h].accupied) {
            if (!sv_eq(ht.items[h].key, token)) {
                nob_log(ERROR, "Table Oversized");
                return false;
            }
            ht.items[h].value += 1;
        } else {
            ht.items[h].accupied = true;
            ht.items[h].key = token;
            ht.items[h].value = 1;
        }
    }

    struct timespec end = {0};
    ret = clock_gettime(CLOCK_MONOTONIC, &end);
    assert(ret == 0);

    FreqKeyVals freq = {0};
    for (size_t i = 0; i < ht.capacity; ++i) {
        if (ht.items[i].accupied) {
            da_append(&freq, ht.items[i]);
        }
    }
    qsort(freq.items, freq.count, sizeof(freq.items[0]), compare_freqkeyval_count);
    for (size_t i = 0; i < 10 && i < freq.count; ++i) {
        nob_log(INFO,
                "    %zu: "SV_Fmt" => %zu",
                i,
                SV_Arg(freq.items[i].key),
                freq.items[i].value);
    }

    nob_log(INFO, "Elapsed time %.3lfs", delta_secs(begin, end));

    return true;
}

// find_sensetive_data moved to json_format.c (json-c impl)
