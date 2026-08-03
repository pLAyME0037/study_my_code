#ifndef TEXT_TAB_H
#define TEXT_TAB_H

#include "../third_party/hash_table.h"
#include "window.h"

void view_populate_text_results(AppWidgets *w, const FreqKeyVals *ht);
void view_clear_text_results(AppWidgets *w);

#endif
