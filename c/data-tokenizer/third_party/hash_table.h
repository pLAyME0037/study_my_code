#ifndef IMPLEMENT_HASH_TABLE
#define IMPLEMENT_HASH_TABLE

#define NOB_STRIP_PREFIX
#include "../build/nob.h"

uint32_t old_hash(uint8_t *buf, size_t buf_size);
uint32_t djb2(uint8_t *buf, size_t buf_size);

void native_analysis(String_View content, const char *file_path);
bool hash_analysis(String_View content, const char *file_path);
bool find_sensetive_data(String_View content, const char *json_path);
#endif // IMPLEMENT_HASH_TABLE
