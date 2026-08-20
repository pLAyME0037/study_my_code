#ifndef HTTP_UTILS
#define HTTP_UTILS

#include "../../nob.h"

bool form_find(String_View body,
              const char  *key,
              char        *out,
              size_t       out_cap);
#endif // !HTTP_UTILS
