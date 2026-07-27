#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOB_STRIP_PREFIX
#include "../build/nob.h"
#include "json_format.h"

static const char *patterns_l1[] = { "phone", "date_of_birth" };
static const char *patterns_l2[] = { "status", "id", "password" };
static const char *patterns_l3[] = { "kill", "rape", "shit", "jail" };

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void walk_json(struct json_object *obj, const char *prefix,
                      ssize_t user_idx, SensitiveReport *report);

static void add_field(SensitiveReport *report, size_t user_idx,
                      const char *path, const char *field_name,
                      const char *value, int level) {
    SensitiveField f = {0};
    f.user_idx = user_idx;
    snprintf(f.path, sizeof(f.path), "%s", path);
    snprintf(f.field_name, sizeof(f.field_name), "%s", field_name);
    snprintf(f.value, sizeof(f.value), "%s", value);
    f.level = level;
    f.sensitive = false;
    da_append(report, f);
}

static void check_string_value(const char *field_name, const char *value,
                               const char *prefix, ssize_t user_idx,
                               SensitiveReport *report) {
    for (size_t i = 0; i < ARRAY_SIZE(patterns_l3); i++) {
        if (strstr(value, patterns_l3[i])) {
            add_field(report, user_idx, prefix, field_name, value, 3);
            return;
        }
    }
}

static void walk_json(struct json_object *obj, const char *prefix,
                      ssize_t user_idx, SensitiveReport *report) {
    if (!obj) return;

    enum json_type type = json_object_get_type(obj);

    if (type == json_type_object) {
        json_object_object_foreach(obj, key, val) {
            char path[256];
            if (prefix[0])
                snprintf(path, sizeof(path), "%s.%s", prefix, key);
            else
                snprintf(path, sizeof(path), "%s", key);

            enum json_type val_type = json_object_get_type(val);

            if (val_type == json_type_object || val_type == json_type_array) {
                walk_json(val, path, user_idx, report);
                continue;
            }

            const char *str_val = "";
            if (val_type == json_type_string)
                str_val = json_object_get_string(val);
            else
                str_val = json_object_to_json_string(val);

            int level = 0;
            for (size_t i = 0; i < ARRAY_SIZE(patterns_l1); i++) {
                if (strcmp(key, patterns_l1[i]) == 0) { level = 1; break; }
            }
            if (!level) {
                for (size_t i = 0; i < ARRAY_SIZE(patterns_l2); i++) {
                    if (strcmp(key, patterns_l2[i]) == 0) { level = 2; break; }
                }
            }

            if (level)
                add_field(report, user_idx, path, key, str_val, level);

            if (val_type == json_type_string)
                check_string_value(key, str_val, path, user_idx, report);
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
                if (strcmp(report->items[j].field_name, entries[i].name) == 0)
                    report->items[j].sensitive = true;
            }
        }
    }
}

void print_report(const SensitiveReport *report) {
    printf("\n%-4s %-6s %-35s %-20s %-6s %-9s\n",
           "No", "User", "Path", "Value", "Level", "Sensitive");
    printf("----+------+-----------------------------------+"
           "--------------------+------+----------\n");

    for (size_t i = 0; i < report->count; i++) {
        const SensitiveField *f = &report->items[i];
        const char *display_val = f->sensitive ? "***" : f->value;
        char user_buf[16];
        snprintf(user_buf, sizeof(user_buf), "%zd", f->user_idx);

        printf("%-4zu %-6s %-35s %-20s %-6d %-9s\n",
               i, f->user_idx >= 0 ? user_buf : "-", f->path, display_val,
               f->level, f->sensitive ? "yes ***" : "no");
    }
    printf("\nTotal: %zu fields\n", report->count);
}
