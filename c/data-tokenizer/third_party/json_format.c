#include <json-c/json.h>
#include <string.h>
#include <time.h>
#define NOB_STRIP_PREFIX
#include "../build/nob.h"


static const char *patterns_l1[] = { "phone", "date_of_birth" };    // sensitivity 1
static const char *patterns_l2[] = { "status", "id", "password" };  // sensitivity 2
static const char *patterns_l3[] = { "kill", "rape", "shit", "jail" }; // sensitivity 3

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

typedef struct {
    char value[256];
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

    char *str = strndup(json.data, json.count);
    struct json_object *root = json_tokener_parse(str);
    free(str);

    if (!root) {
        nob_log(ERROR, "JSON parse failed");
        return result;
    }

    if (json_object_get_type(root) != json_type_array) {
        nob_log(ERROR, "Expected JSON array");
        json_object_put(root);
        return result;
    }

    size_t len = json_object_array_length(root);
    for (size_t i = 0; i < len; i++) {
        struct json_object *item = json_object_array_get_idx(root, i);
        struct json_object *val_obj, *type_obj, *lvl_obj;

        SensitivePattern pat = {0};

        if (json_object_object_get_ex(item, "value", &val_obj)) {
            snprintf(pat.value, sizeof(pat.value), "%s",
                     json_object_get_string(val_obj));
        }

        if (json_object_object_get_ex(item, "type", &type_obj)) {
            snprintf(pat.type, sizeof(pat.type), "%s",
                     json_object_get_string(type_obj));
        }

        if (json_object_object_get_ex(item, "sensitivity", &lvl_obj)) {
            pat.level = json_object_get_int(lvl_obj);
        }

        if (pat.value[0]) da_append(&result, pat);
    }

    json_object_put(root);
    return result;
}

static double delta_secs(struct timespec begin, struct timespec end) {
    double a = (double)begin.tv_sec + begin.tv_nsec * 1e-9;
    double b = (double)end.tv_sec + end.tv_nsec * 1e-9;
    return b - a;
}

static bool sv_eq_cstr(String_View sv, const char *cstr) {
    size_t len = strlen(cstr);
    return sv.count == len && memcmp(sv.data, cstr, len) == 0;
}

bool find_sensetive_data(String_View content, const char *json_path) {
    Nob_String_Builder json_sb = {0};
    if (!nob_read_entire_file(json_path, &json_sb)) {
        nob_log(ERROR, "Could not read config: %s", json_path);
        return false;
    }
    String_View json = sv_from_parts(json_sb.items, json_sb.count);

    SensitivePatterns patterns = parse_sensitive_config(json);
    nob_log(INFO, "Loaded %zu JSON blocks and sensitive patterns\narray 1 have "
            "%zu patterns\narray 2 have %zu patterns\narray 3 have %zu patterns"
            "\n> from '%s'",
            patterns.count,
            ARRAY_SIZE(patterns_l1),
            ARRAY_SIZE(patterns_l2),
            ARRAY_SIZE(patterns_l3),
            json_path);

    sb_free(json_sb);

    struct timespec begin = {0};
    clock_gettime(CLOCK_MONOTONIC, &begin);

    bool found_any = false;
    size_t total_matches = 0;
    String_View rest = content;

    while (rest.count > 0) {
        rest = sv_trim_left(rest);
        if (rest.count == 0) break;

        String_View token = sv_chop_by_space(&rest);

        // 1. Check loaded JSON patterns
        for (size_t i = 0; i < patterns.count; i++) {
            if (sv_eq_cstr(token, patterns.items[i].value)) {
                nob_log(WARNING,
                        "Sensitive: "SV_Fmt" (type=%s, level=%d)",
                        SV_Arg(token),
                        patterns.items[i].type,
                        patterns.items[i].level);
                found_any = true;
                total_matches++;
                goto next_token;
            }
        }

        // 2. Check type patterns l1
        for (size_t i = 0; i < ARRAY_SIZE(patterns_l1); i++) {
            if (sv_eq_cstr(token, patterns_l1[i])) {
                nob_log(WARNING,
                        "Sensitive: "SV_Fmt" (type=%s, level=%d)",
                        SV_Arg(token),
                        "type_l1", 1);
                found_any = true;
                total_matches++;
                goto next_token;
            }
        }

        // 3. Check type patterns l2
        for (size_t i = 0; i < ARRAY_SIZE(patterns_l2); i++) {
            if (sv_eq_cstr(token, patterns_l2[i])) {
                nob_log(WARNING,
                        "Sensitive: "SV_Fmt" (type=%s, level=%d)",
                        SV_Arg(token),
                        "type_l2", 2);
                found_any = true;
                total_matches++;
                goto next_token;
            }
        }

        // 4. Check banned words l3
        for (size_t i = 0; i < ARRAY_SIZE(patterns_l3); i++) {
            if (sv_eq_cstr(token, patterns_l3[i])) {
                nob_log(WARNING,
                        "Sensitive: "SV_Fmt" (type=%s, level=%d)",
                        SV_Arg(token),
                        "banned", 3);
                found_any = true;
                total_matches++;
                goto next_token;
            }
        }

        next_token:;
    }

    struct timespec end = {0};
    clock_gettime(CLOCK_MONOTONIC, &end);

    if (found_any) {
        nob_log(INFO, "Matches: %zu (%.3lfs)", total_matches, delta_secs(begin, end));
    } else {
        nob_log(INFO, "No sensitive data detected (%.3lfs)", delta_secs(begin, end));
    }

    return true;
}
