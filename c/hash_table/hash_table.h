#ifndef IMPLEMENT_HASH_TABLE
#define IMPLEMENT_HASH_TABLE

#define NOB_STRIP_PREFIX
#include "nob.h"

typedef struct FreqKeyVal FreqKeyVal;
typedef struct FreqKeyVals FreqKeyVals;

FreqKeyVal *find_key(FreqKeyVals haystack, String_View needle);
double delta_secs(struct timespec begin, struct timespec end);
int compare_freqkeyval_count(const void *a, const void *b);

uint32_t old_hash(uint8_t *buf, size_t buf_size);
uint32_t djb2(uint8_t *buf, size_t buf_size);

void native_analysis(String_View content, const char *file_path);
bool hash_analysis(String_View content, const char *file_path);
#endif // IMPLEMENT_HASH_TABLE
