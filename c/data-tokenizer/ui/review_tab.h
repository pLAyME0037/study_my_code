#ifndef REVIEW_TAB_H
#define REVIEW_TAB_H

#include "../third_party/json_format.h"
#include "window.h"

gboolean view_should_confirm(AppWidgets *w);
void view_populate_confirm(AppWidgets *w, const SensitiveReport *report);
void view_apply_confirm(AppWidgets *w, SensitiveReport *report);

#endif
