#ifndef TABLE_TAB_H
#define TABLE_TAB_H

#include "../third_party/json_format.h"
#include "window.h"

void view_populate_results(AppWidgets *w, const SensitiveReport *report);
void view_clear_results(AppWidgets *w);
void view_set_summary(AppWidgets *w, const char *text);

#endif
