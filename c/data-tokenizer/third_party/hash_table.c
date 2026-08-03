#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash_table.h"
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "../build/nob.h"
#include "pattern_template.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static double delta_secs(struct timespec begin, struct timespec end) {
    double a = (double)begin.tv_sec + begin.tv_nsec * 1e-9;
    double b = (double)end.tv_sec + end.tv_nsec * 1e-9;
    return b - a;
}

uint32_t djb2(uint8_t *buf, size_t buf_size) {
    uint32_t hash = 5381;
    for (size_t i = 0; i < buf_size; ++i) {
        hash = ((hash << 5) + hash) + (uint32_t)buf[i]; /* hash * 33 + c */
    }
    return hash;
}

static bool is_prime(size_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

static size_t next_prime(size_t n) {
    if (n <= 3) return 3;
    if (n % 2 == 0) n++;
    while (!is_prime(n)) n += 2;
    return n;
}

static bool ht_init(FreqKeyVals *ht, size_t capacity) {
    ht->capacity = next_prime(capacity);
    ht->count = 0;
    ht->items = calloc(ht->capacity, sizeof(FreqKeyVal));
    return ht->items != NULL;
}

static bool ht_resize(FreqKeyVals *ht) {
    size_t new_cap = next_prime(ht->capacity * 2);
    FreqKeyVal *new_items = calloc(new_cap, sizeof(FreqKeyVal));
    if (!new_items) return false;

    FreqKeyVal *old_items = ht->items;
    size_t old_cap = ht->capacity;

    ht->items = new_items;
    ht->capacity = new_cap;
    ht->count = 0;

    for (size_t i = 0; i < old_cap; ++i) {
        if (old_items[i].occupied) {
            const char *word = old_items[i].word;
            size_t h = djb2((uint8_t *)word, strlen(word)) % ht->capacity;
            while (ht->items[h].occupied) h = (h + 1) % ht->capacity;
            ht->items[h] = old_items[i];
            ht->count++;
        }
    }

    free(old_items);
    return true;
}

static bool ht_insert(FreqKeyVals *ht, const char *word) {
    if (ht->count * 10 >= ht->capacity * 7) {
        if (!ht_resize(ht)) return false;
    }

    size_t h = djb2((uint8_t *)word, strlen(word)) % ht->capacity;
    while (ht->items[h].occupied && strcmp(ht->items[h].word, word) != 0) {
        h = (h + 1) % ht->capacity;
    }

    if (ht->items[h].occupied) {
        ht->items[h].value += 1;
        return true;
    }

    ht->items[h].word = strdup(word);
    if (!ht->items[h].word) return false;

    ht->items[h].value = 1;
    ht->items[h].occupied = true;
    ht->items[h].sensitive = false;
    ht->items[h].pattern = NULL;

    for (size_t p = 0; p < ARRAY_SIZE(patterns_l3); ++p) {
        if (strcmp(word, patterns_l3[p]) == 0) {
            ht->items[h].sensitive = true;
            ht->items[h].pattern = patterns_l3[p];
            break;
        }
    }

    ht->count++;
    return true;
}

static bool normalize_token(String_View token, char *out, size_t out_sz) {
    if (token.count == 0) return false;

    const char *start = token.data;
    const char *end = token.data + token.count;

    while (start < end && !isalnum((unsigned char)*start)) start++;
    while (end > start && !isalnum((unsigned char)end[-1])) end--;

    if (start >= end) return false;

    size_t len = (size_t)(end - start);
    if (len >= out_sz) len = out_sz - 1;

    for (size_t i = 0; i < len; ++i) {
        out[i] = (char)tolower((unsigned char)start[i]);
    }
    out[len] = '\0';
    return len > 0;
}

bool hash_analysis(String_View content, const char *file_path, FreqKeyVals *out) {
    out->items = NULL;
    out->count = 0;
    out->capacity = 0;
    if (!ht_init(out, 1024)) return false;

    nob_log(INFO, "Analying: %s", file_path);
    nob_log(INFO, "  Size: %zu bytes", content.count);

    struct timespec begin = {0};
    int ret = clock_gettime(CLOCK_MONOTONIC, &begin);
    assert(ret == 0);

    size_t total = 0;
    while (content.count > 0) {
        content = sv_trim_left(content);
        String_View token = sv_chop_by_space(&content);

        char norm[256];
        if (!normalize_token(token, norm, sizeof(norm))) continue;

        if (!ht_insert(out, norm)) return false;
        total++;
    }

    struct timespec end = {0};
    ret = clock_gettime(CLOCK_MONOTONIC, &end);
    assert(ret == 0);

    nob_log(INFO, "  Total tokens: %zu, unique words: %zu", total, out->count);

    print_top_hash_analysis(out);
    print_sensitive_patterns_from_hash_analysis(out);

    nob_log(INFO, "Elapsed time %.3lfs", delta_secs(begin, end));

    return true;
}

void freq_keyvals_free(FreqKeyVals *ht) {
    if (!ht) return;
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->items[i].occupied) {
            free(ht->items[i].word);
        }
    }
    free(ht->items);
    ht->items = NULL;
    ht->count = 0;
    ht->capacity = 0;
}

typedef struct {
    const FreqKeyVal **items;
    size_t count;
    size_t capacity;
} KVPtrArr;

static int compare_kv_ptr_count_desc(const void *a, const void *b) {
    const FreqKeyVal *ka = *(const FreqKeyVal *const *)a;
    const FreqKeyVal *kb = *(const FreqKeyVal *const *)b;
    if (kb->value > ka->value) return 1;
    if (kb->value < ka->value) return -1;
    return 0;
}

FreqKeyVal **freq_keyvals_sorted(const FreqKeyVals *ht, size_t *out_n) {
    KVPtrArr arr = {0};
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->items[i].occupied) {
            da_append(&arr, &ht->items[i]);
        }
    }
    qsort(arr.items, arr.count, sizeof(arr.items[0]), compare_kv_ptr_count_desc);
    if (out_n) *out_n = arr.count;
    return (FreqKeyVal **)arr.items;
}

void print_top_hash_analysis(const FreqKeyVals *ht) {
    size_t n = 0;
    FreqKeyVal **arr = freq_keyvals_sorted(ht, &n);
    nob_log(INFO, "  Top %zu most frequent tokens:", n < 10 ? n : 10);
    for (size_t i = 0; i < 10 && i < n; ++i) {
        nob_log(INFO, "    %zu: %s => %zu",
                i,
                arr[i]->word,
                arr[i]->value);
    }
    free(arr);
}

void print_sensitive_patterns_from_hash_analysis(const FreqKeyVals *ht) {
    KVPtrArr arr = {0};
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->items[i].occupied && ht->items[i].sensitive) {
            da_append(&arr, &ht->items[i]);
        }
    }
    qsort(arr.items, arr.count, sizeof(arr.items[0]), compare_kv_ptr_count_desc);
    nob_log(INFO, "  Banned words found (%zu):", arr.count);
    for (size_t i = 0; i < arr.count; ++i) {
        nob_log(INFO, "    %s (x%zu) pattern=%s",
                arr.items[i]->word,
                arr.items[i]->value,
                arr.items[i]->pattern);
    }
    free(arr.items);
}

