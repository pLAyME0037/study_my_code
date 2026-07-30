#include "table_tab.h"

static void clear_list(GtkListBox *list) {
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(list));
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_list_box_remove(list, child);
        child = next;
    }
}

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

    lbl = gtk_label_new(f->value);
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

    lbl = gtk_label_new(f->sensitive ? "yes" : "no");
    gtk_widget_set_size_request(lbl, 60, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    return box;
}

void view_populate_results(AppWidgets *w, const SensitiveReport *report) {
    clear_list(w->results_list);

    for (size_t i = 0; i < report->count; i++) {
        GtkWidget *row = make_data_row(i, &report->items[i]);
        gtk_list_box_append(w->results_list, row);
    }

    char summary[256];
    snprintf(summary, sizeof(summary), "Found %zu sensitive field(s).", report->count);
    gtk_label_set_text(w->summary_label, summary);
}

void view_clear_results(AppWidgets *w) {
    clear_list(w->results_list);
}

void view_set_summary(AppWidgets *w, const char *text) {
    gtk_label_set_text(w->summary_label, text);
}
