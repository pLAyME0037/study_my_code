#include "table_tab.h"
#include <string.h>

static GPtrArray *tab_names = NULL;

static GtkWidget *make_data_row(size_t no, const SensitiveField *f) {
    char buf[64];
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_margin_start(box, 8);
    gtk_widget_set_margin_end(box, 8);
    gtk_widget_set_margin_top(box, 2);
    gtk_widget_set_margin_bottom(box, 2);

    snprintf(buf, sizeof(buf), "%zu", no);
    GtkWidget *lbl = gtk_label_new(buf);
    gtk_widget_set_size_request(lbl, 40, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    snprintf(buf, sizeof(buf), "%zd", f->user_idx);
    lbl = gtk_label_new(buf);
    gtk_widget_set_size_request(lbl, 50, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new(f->path);
    gtk_widget_set_hexpand(lbl, TRUE);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_label_set_wrap(GTK_LABEL(lbl), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(lbl), PANGO_WRAP_WORD_CHAR);
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new(f->sensitive ? "***" : f->value);
    gtk_widget_set_hexpand(lbl, TRUE);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_label_set_wrap(GTK_LABEL(lbl), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(lbl), PANGO_WRAP_WORD_CHAR);
    gtk_box_append(GTK_BOX(box), lbl);

    snprintf(buf, sizeof(buf), "%d", f->level);
    lbl = gtk_label_new(buf);
    gtk_widget_set_size_request(lbl, 40, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    return box;
}

static GtkWidget *make_header_row(void) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_margin_start(box, 8);
    gtk_widget_set_margin_end(box, 8);
    gtk_widget_set_margin_top(box, 4);
    gtk_widget_set_margin_bottom(box, 2);

    GtkWidget *lbl = gtk_label_new("No");
    gtk_widget_set_size_request(lbl, 40, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new("User");
    gtk_widget_set_size_request(lbl, 50, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new("Path");
    gtk_widget_set_hexpand(lbl, TRUE);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new("Value");
    gtk_widget_set_hexpand(lbl, TRUE);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new("Level");
    gtk_widget_set_size_request(lbl, 40, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    return box;
}

static GtkWidget *make_field_page(const char *name, const SensitiveReport *report) {
    char title[256];
    size_t count = 0;
    for (size_t i = 0; i < report->count; i++) {
        if (strcmp(report->items[i].field_name, name) == 0)
            count++;
    }

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_margin_top(vbox, 8);

    snprintf(title, sizeof(title), "%s - %zu occurrence(s)", name, count);
    GtkWidget *lbl = gtk_label_new(title);
    gtk_widget_set_halign(lbl, GTK_ALIGN_START);
    gtk_widget_set_margin_start(lbl, 8);
    gtk_widget_set_margin_end(lbl, 8);
    gtk_box_append(GTK_BOX(vbox), lbl);

    gtk_box_append(GTK_BOX(vbox), make_header_row());

    GtkWidget *list = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list), GTK_SELECTION_NONE);

    size_t no = 0;
    for (size_t i = 0; i < report->count; i++) {
        if (strcmp(report->items[i].field_name, name) != 0)
            continue;
        gtk_list_box_append(GTK_LIST_BOX(list), make_data_row(no++, &report->items[i]));
    }

    GtkWidget *sw = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw), list);
    gtk_widget_set_vexpand(sw, TRUE);
    gtk_box_append(GTK_BOX(vbox), sw);

    return vbox;
}

static void remove_field_tabs(AppWidgets *w) {
    if (!tab_names)
        return;
    for (guint i = 0; i < tab_names->len; i++) {
        const char *name = g_ptr_array_index(tab_names, i);
        GtkWidget *child = gtk_stack_get_child_by_name(w->stack, name);
        if (child)
            gtk_stack_remove(w->stack, child);
        g_free((char *)name);
    }
    g_ptr_array_set_size(tab_names, 0);
}

void view_populate_results(AppWidgets *w, const SensitiveReport *report) {
    remove_field_tabs(w);

    if (report->count == 0) {
        view_set_status(w, "No sensitive fields found.");
        view_show_page(w, "home");
        return;
    }

    if (!tab_names)
        tab_names = g_ptr_array_new();

    const char *first_page = NULL;
    for (size_t i = 0; i < report->count; i++) {
        const char *field_name = report->items[i].field_name;
        char *page_name = g_strdup_printf("field_%s", field_name);
        if (gtk_stack_get_child_by_name(w->stack, page_name)) {
            g_free(page_name);
            continue;
        }
        GtkWidget *page = make_field_page(field_name, report);
        gtk_stack_add_titled(w->stack, page, page_name, field_name);
        g_ptr_array_add(tab_names, page_name);
        if (!first_page)
            first_page = page_name;
    }

    if (first_page)
        view_show_page(w, first_page);
}

void view_clear_results(AppWidgets *w) {
    remove_field_tabs(w);
}
