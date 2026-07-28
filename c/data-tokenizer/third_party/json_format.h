#ifndef JSON_FORMAT
#define JSON_FORMAT

#define NOB_STRIP_PREFIX
#include "../build/nob.h"

typedef struct {
    ssize_t user_idx;
    char path[256];
    char value[256];
    char field_name[64];
    int level;
    bool sensitive;
} SensitiveField;

typedef struct {
    SensitiveField *items;
    size_t count;
    size_t capacity;
} SensitiveReport;

bool find_sensetive_data(const char *json_path, SensitiveReport *report);
void confirm_sensitive_fields(SensitiveReport *report);
void print_report(const SensitiveReport *report);
#endif // JSON_FORMAT
