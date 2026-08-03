#include <json-c/json.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOB_STRIP_PREFIX
#include "../build/nob.h"
#include "json_format.h"

static const char *patterns_l3[] = {
    "kill", "demon", "shit", "jail", "bomb", "dinamie", "prison", "prisoner"
};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void walk_json(struct json_object *obj,
                      const char *prefix,
                      ssize_t user_idx,
                      SensitiveReport *report);

static void add_field(SensitiveReport *report,
                      size_t user_idx,
                      const char *path,
                      const char *field_name,
                      const char *value,
                      int level) {
    SensitiveField f = {0};
    f.user_idx = user_idx;
    snprintf(f.path, sizeof(f.path), "%s", path);
    snprintf(f.field_name, sizeof(f.field_name), "%s", field_name);
    snprintf(f.value, sizeof(f.value), "%s", value);
    f.level = level;
    f.sensitive = false;
    da_append(report, f);
}

static bool contains_word(const char *text, const char *word) {
    size_t word_len = strlen(word);
    const char *p = text;
    while ((p = strstr(p, word)) != NULL) {
        bool start_ok = (p == text || !isalnum((unsigned char)p[-1]));
        bool end_ok = (!p[word_len] || !isalnum((unsigned char)p[word_len]));
        if (start_ok && end_ok) return true;
        p += word_len;
    }
    return false;
}

static void check_string_value(const char *field_name,
                               const char *value,
                               const char *prefix,
                               ssize_t user_idx,
                               SensitiveReport *report) {
    for (size_t i = 0; i < ARRAY_SIZE(patterns_l3); i++) {
        if (contains_word(value, patterns_l3[i])) {
            add_field(report, user_idx, prefix, field_name, value, 3);
            return;
        }
    }
}

static void walk_json(struct json_object *obj,
                      const char *prefix,
                      ssize_t user_idx,
                      SensitiveReport *report) {
    if (!obj) return;

    enum json_type type = json_object_get_type(obj);

    if (type == json_type_object) {
        json_object_object_foreach(obj, key, val) {
            char path[256];
            if (prefix[0]) snprintf(path, sizeof(path), "%s.%s", prefix, key);
            else snprintf(path, sizeof(path), "%s", key);

            enum json_type val_type = json_object_get_type(val);

            if (val_type == json_type_object || val_type == json_type_array) {
                walk_json(val, path, user_idx, report);
                continue;
            }

            const char *str_val = "";
            if (val_type == json_type_string) {
                str_val = json_object_get_string(val);
            } else {
                str_val = json_object_to_json_string(val);
            }

            if (val_type == json_type_string) {
                check_string_value(key, str_val, path, user_idx, report);
            }
        }
    } else if (type == json_type_array) {
        size_t len = json_object_array_length(obj);
        for (size_t i = 0; i < len; i++) {
            struct json_object *elem = json_object_array_get_idx(obj, i);
            if (json_object_get_type(elem) == json_type_object) {
                char path[256];
                snprintf(path, sizeof(path), "%s[%zu]", prefix, i);
                walk_json(elem, path, (ssize_t)i, report);
            }
        }
    }
}

bool find_sensetive_data(const char *json_path, SensitiveReport *report) {
    Nob_String_Builder sb = {0};
    if (!read_entire_file(json_path, &sb)) {
        nob_log(ERROR, "Could not read: %s", json_path);
        return false;
    }

    char *str = strndup(sb.items, sb.count);
    sb_free(sb);

    struct json_object *root = json_tokener_parse(str);
    free(str);

    if (!root) {
        nob_log(ERROR, "JSON parse failed: %s", json_path);
        return false;
    }

    nob_log(INFO, "Parsed %s", json_path);

    walk_json(root, "", -1, report);
    json_object_put(root);

    nob_log(INFO, "Found %zu sensitive fields", report->count);
    return true;
}

void confirm_sensitive_fields(SensitiveReport *report) {
    typedef struct {
        char name[64];
        int level;
        size_t count;
        size_t sample_idx;
    } FieldEntry;

    FieldEntry entries[256] = {0};
    size_t entry_count = 0;

    for (size_t i = 0; i < report->count; i++) {
        const char *name = report->items[i].field_name;
        bool found = false;
        for (size_t j = 0; j < entry_count; j++) {
            if (strcmp(entries[j].name, name) == 0) {
                entries[j].count++;
                found = true;
                break;
            }
        }
        if (!found && entry_count < 256) {
            snprintf(entries[entry_count].name, sizeof(entries[entry_count].name), "%s", name);
            entries[entry_count].level = report->items[i].level;
            entries[entry_count].count = 1;
            entries[entry_count].sample_idx = i;
            entry_count++;
        }
    }

    printf("\nSensitive field names found:\n");
    for (size_t i = 0; i < entry_count; i++) {
        const SensitiveField *sample = &report->items[entries[i].sample_idx];
        printf("  [%zu] %-20s (level=%d, %zu fields) sample: %s = \"%s\"\n",
               i, entries[i].name, entries[i].level, entries[i].count,
               sample->path, sample->value);
    }

    printf("\nMark fields as sensitive? Enter y/n per field name:\n");
    for (size_t i = 0; i < entry_count; i++) {
        printf("  %s [y/N]: ", entries[i].name);
        fflush(stdout);

        char buf[16] = {0};
        bool yes = (fgets(buf, sizeof(buf), stdin) &&
                    (buf[0] == 'y' || buf[0] == 'Y'));

        if (yes) {
            for (size_t j = 0; j < report->count; j++) {
                if (strcmp(report->items[j].field_name, entries[i].name) == 0) {
                    report->items[j].sensitive = true;
                }
            }
        }
    }
}

#define W_NO    4
#define W_USR   6
#define W_PATH  35
#define W_VAL   45
#define W_LVL   5
#define W_SENS  9

static size_t get_wrap_length(const char *text, size_t max_width) {
    if (!text || *text == '\0') return 0;
    size_t len = strlen(text);
    if (len <= max_width) return len;

    // Look backward from max_width to find a space
    for (size_t i = max_width; i > 0; i--) {
        if (isspace((unsigned char)text[i])) {
            return i;
        }
    }
    // If no space is found within the limit, force a hard break
    return max_width;
}

void print_report(const SensitiveReport *report) {
    printf("\n%-*s | %-*s | %-*s | %-*s | %-*s | %-*s\n",
           W_NO, "No", W_USR, "User", W_PATH, "Path",
           W_VAL, "Value", W_LVL, "Level", W_SENS, "Sensitive");

    printf("-----|--------|-------------------------------------|-------------"
           "----------------------------------|-------|----------\n");

    for (size_t i = 0; i < report->count; i++) {
        const SensitiveField *f = &report->items[i];
        const char *display_val = f->sensitive ? "***" : f->value;

        char user_buf[16];
        if (f->user_idx >= 0) {
            snprintf(user_buf, sizeof(user_buf), "%zd", f->user_idx);
        } else {
            strcpy(user_buf, "-");
        }

        const char *p_path = f->path;
        const char *p_val = display_val;
        bool is_first_line = true;

        while (*p_path != '\0' || *p_val != '\0' || is_first_line) {

            size_t path_len = get_wrap_length(p_path, W_PATH);
            size_t val_len  = get_wrap_length(p_val, W_VAL);

            const char *next_path = p_path + path_len;
            while (isspace((unsigned char)*next_path)) next_path++;

            const char *next_val = p_val + val_len;
            while (isspace((unsigned char)*next_val)) next_val++;

            bool is_last_line = (*next_path == '\0') && (*next_val == '\0');

            if (is_first_line) {
                printf("%-*zu | %-*s | ", W_NO, i, W_USR, user_buf);
            } else {
                printf("%-*s | %-*s | ", W_NO, "", W_USR, "");
            }

            // Note: %-*.*s means "pad right to X width, but print max Y chars"
            printf("%-*.*s | %-*.*s | ",
                   W_PATH, (int)path_len, p_path,
                   W_VAL,  (int)val_len,  p_val);

            if (is_last_line) {
                printf("%-*d | %-*s\n", W_LVL, f->level, W_SENS, f->sensitive ? "yes ***" : "no");
            } else {
                printf("%-*s | %-*s\n", W_LVL, "", W_SENS, "");
            }

            // Advance pointers for the next loop iteration
            p_path = next_path;
            p_val = next_val;
            is_first_line = false;
        }
    }
    printf("\nTotal: %zu fields\n", report->count);
}
