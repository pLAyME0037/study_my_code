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

typedef struct {
    String_View pattern;
    char type[64];
    int level;
} SensitivePattern;

typedef struct {
    SensitivePattern *items;
    size_t count;
    size_t capacity;
} SensitivePatterns;

SensitivePatterns parse_sensitive_config(String_View json) {
    SensitivePatterns result = {0};

    const char *needle = "src {";
    size_t needle_len = strlen(needle);
    size_t pos = 0;
    for (size_t i = 0; i + needle_len <= json.count; i++) {
        if (memcmp(json.data + i, needle, needle_len) == 0) {
            pos = i + needle_len;
            break;
        }
    }
    if (pos == 0) return result;

    String_View rest = sv_from_parts(json.data + pos, json.count - pos);

    while (rest.count > 0) {
        rest = sv_trim_left(rest);
        if (rest.count == 0 || rest.data[0] == '}') break;

        if (rest.data[0] == '{') {
            sv_chop_left(&rest, 1);
            rest = sv_trim_left(rest);

            const char *close = memchr(rest.data, '}', rest.count);
            if (!close) break;

            size_t inner_len = close - rest.data;
            String_View inner = sv_from_parts(rest.data, inner_len);
            sv_chop_left(&rest, inner_len + 1);

            SensitivePattern pat = {0};

            String_View rem = inner;
            while (rem.count > 0) {
                rem = sv_trim_left(rem);
                if (rem.count == 0) break;

                const char *comma = memchr(rem.data, ',', rem.count);
                size_t pair_len = comma ? (size_t)(comma - rem.data) : rem.count;
                String_View pair = sv_trim(sv_from_parts(rem.data, pair_len));
                sv_chop_left(&rem, pair_len + (comma ? 1 : 0));

                const char *eq = memchr(pair.data, '=', pair.count);
                if (!eq) continue;

                size_t key_len = eq - pair.data;
                String_View key = sv_trim(sv_from_parts(pair.data, key_len));
                String_View val = sv_trim(sv_from_parts(eq + 1, pair.count - key_len - 1));

                if (val.count >= 2 && val.data[0] == '"' && val.data[val.count-1] == '"') {
                    val.data++; val.count -= 2;
                }

                if (sv_eq(key, sv_from_cstr("type"))) {
                    snprintf(pat.type, sizeof(pat.type), SV_Fmt, SV_Arg(val));
                } else if (sv_eq(key, sv_from_cstr("sesitivity"))) {
                    char num[16] = {0};
                    snprintf(num, sizeof(num), SV_Fmt, SV_Arg(val));
                    pat.level = atoi(num);
                } else {
                    pat.pattern = val;
                }
            }

            if (pat.pattern.data && pat.pattern.count > 0) {
                da_append(&result, pat);
            }
        } else {
            sv_chop_left(&rest, 1);
        }
    }

    return result;
}

bool find_sensetive_data(String_View content, const char *json_path) {
    Nob_String_Builder json_sb = {0};
    if (!read_entire_file(json_path, &json_sb)) {
        nob_log(ERROR, "Could not read config: %s", json_path);
        return false;
    }
    String_View json = sv_from_parts(json_sb.items, json_sb.count);

    SensitivePatterns patterns = parse_sensitive_config(json);
    nob_log(INFO, "Loaded %zu sensitive patterns from '%s'", patterns.count, json_path);

    if (patterns.count == 0) {
        nob_log(WARNING, "No patterns in '%s'", json_path);
        sb_free(json_sb);
        return true;
    }

    struct timespec begin = {0};
    clock_gettime(CLOCK_MONOTONIC, &begin);

    bool found_any = false;
    size_t total_matches = 0;
    String_View rest = content;

    while (rest.count > 0) {
        rest = sv_trim_left(rest);
        if (rest.count == 0) break;

        String_View token = sv_chop_by_space(&rest);

        for (size_t i = 0; i < patterns.count; i++) {
            if (sv_eq(token, patterns.items[i].pattern)) {
                nob_log(WARNING,
                    "Sensitive: "SV_Fmt" (type=%s, level=%d)",
                    SV_Arg(token),
                    patterns.items[i].type,
                    patterns.items[i].level);
                found_any = true;
                total_matches++;
                break;
            }
        }
    }

    struct timespec end = {0};
    clock_gettime(CLOCK_MONOTONIC, &end);

    if (found_any) {
        nob_log(INFO, "Matches: %zu (%.3lfs)", total_matches, delta_secs(begin, end));
    } else {
        nob_log(INFO, "No sensitive data detected (%.3lfs)", delta_secs(begin, end));
    }

    sb_free(json_sb);
    return true;
}
