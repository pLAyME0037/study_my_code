#ifndef IMPLEMENT_HASH_TABLE
#define IMPLEMENT_HASH_TABLE

#define NOB_STRIP_PREFIX
#include "../build/nob.h"

typedef struct {
    char *word;
    size_t value;
    const char *pattern;
    bool sensitive;
    bool occupied;
} FreqKeyVal;

typedef struct {
    FreqKeyVal *items;
    size_t count;
    size_t capacity;
} FreqKeyVals;

uint32_t djb2(uint8_t *buf, size_t buf_size);

bool hash_analysis(String_View content, const char *file_path, FreqKeyVals *out);
void freq_keyvals_free(FreqKeyVals *ht);
FreqKeyVal **freq_keyvals_sorted(const FreqKeyVals *ht, size_t *out_n);
void print_top_hash_analysis(const FreqKeyVals *ht);
void print_sensitive_patterns_from_hash_analysis(const FreqKeyVals *ht);

#endif // IMPLEMENT_HASH_TABLE
