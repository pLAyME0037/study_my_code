#include "review_tab.h"

static void clear_list(GtkListBox *list) {
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(list));
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_list_box_remove(list, child);
        child = next;
    }
}

gboolean view_should_confirm(AppWidgets *w) {
    return gtk_check_button_get_active(w->cfm_check);
}

void view_populate_confirm(AppWidgets *w, const SensitiveReport *report) {
    clear_list(w->cfm_list);

    typedef struct { char name[64]; int level; size_t count; size_t sample_idx; } Entry;
    Entry entries[256] = {0};
    size_t entry_count = 0;

    for (size_t i = 0; i < report->count; i++) {
        const char *name = report->items[i].field_name;
        gboolean found = FALSE;
        for (size_t j = 0; j < entry_count; j++) {
            if (strcmp(entries[j].name, name) == 0) {
                entries[j].count++;
                found = TRUE;
                break;
            }
        }
        if (!found && entry_count < 256) {
            snprintf(entries[entry_count].name, sizeof(entries[entry_count].name), "%s", name);
            entries[entry_count].level = report->items[i].level;
            entries[entry_count].count = 1;
            entries[entry_count].sample_idx = i;
            entry_count++;
        }
    }

    for (size_t i = 0; i < entry_count; i++) {
        const SensitiveField *sample = &report->items[entries[i].sample_idx];
        char sample_str[256];
        snprintf(sample_str, sizeof(sample_str), "%s = \"%s\"", sample->path, sample->value);

        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        gtk_widget_set_margin_start(row, 8);
        gtk_widget_set_margin_end(row, 8);
        gtk_widget_set_margin_top(row, 2);
        gtk_widget_set_margin_bottom(row, 2);

        GtkWidget *check = gtk_check_button_new();
        gtk_check_button_set_active(GTK_CHECK_BUTTON(check), sample->sensitive);
        g_object_set_data_full(G_OBJECT(row), "field-name",
                               g_strdup(entries[i].name), g_free);
        gtk_box_append(GTK_BOX(row), check);

        GtkWidget *lbl = gtk_label_new(entries[i].name);
        gtk_widget_set_size_request(lbl, 160, -1);
        gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
        gtk_label_set_ellipsize(GTK_LABEL(lbl), PANGO_ELLIPSIZE_END);
        gtk_box_append(GTK_BOX(row), lbl);

        lbl = gtk_label_new(sample_str);
        gtk_widget_set_hexpand(lbl, TRUE);
        gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
        gtk_label_set_ellipsize(GTK_LABEL(lbl), PANGO_ELLIPSIZE_END);
        gtk_box_append(GTK_BOX(row), lbl);

        char buf[16];
        snprintf(buf, sizeof(buf), "%d", entries[i].level);
        lbl = gtk_label_new(buf);
        gtk_widget_set_size_request(lbl, 40, -1);
        gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
        gtk_box_append(GTK_BOX(row), lbl);

        gtk_list_box_append(w->cfm_list, row);
    }
}

void view_apply_confirm(AppWidgets *w, SensitiveReport *report) {
    GtkWidget *row = gtk_widget_get_first_child(GTK_WIDGET(w->cfm_list));
    while (row) {
        GtkWidget *box = gtk_list_box_row_get_child(GTK_LIST_BOX_ROW(row));
        const char *field_name = g_object_get_data(G_OBJECT(box), "field-name");
        if (field_name) {
            GtkWidget *check = gtk_widget_get_first_child(box);
            if (check && GTK_IS_CHECK_BUTTON(check)) {
                gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(check));
                for (size_t j = 0; j < report->count; j++) {
                    if (strcmp(report->items[j].field_name, field_name) == 0) {
                        report->items[j].sensitive = active;
                    }
                }
            }
        }
        row = gtk_widget_get_next_sibling(row);
    }
}
