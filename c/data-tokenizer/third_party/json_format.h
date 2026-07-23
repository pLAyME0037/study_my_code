#ifndef JSON_FORMAT
#define JSON_FORMAT

#define NOB_STRIP_PREFIX
#include "../build/nob.h"

bool find_sensetive_data(String_View content, const char *json_path);
#endif // JSON_FORMAT
