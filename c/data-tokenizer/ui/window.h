#ifndef WINDOW_H
#define WINDOW_H

#include <gtk/gtk.h>
#include "../third_party/json_format.h"

typedef struct {
    GtkWidget      *window;
    GtkStack       *stack;
    GtkEditable    *file_entry;
    GtkLabel       *status_label;
    GtkListBox     *results_list;
    GtkListBox     *confirm_list;
    GtkLabel       *summary_label;
    GtkButton      *submit_btn;
    GtkButton      *cancel_btn;
    GtkButton      *browse_btn;
    GtkButton      *confirm_apply_btn;
    GtkButton      *confirm_back_btn;
    GtkCheckButton *confirm_check;
} AppWidgets;

GtkWidget* app_window_new(GtkApplication *app, AppWidgets *w);

void view_set_status(AppWidgets *w, const char *text);
void view_set_busy(AppWidgets *w, gboolean busy);
void view_show_page(AppWidgets *w, const char *name);
const char* view_get_file_path(AppWidgets *w);

#endif
