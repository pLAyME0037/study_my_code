#include "window.h"

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

GtkWidget *app_window_new(GtkApplication *app, AppWidgets *w) {
    GtkWidget *win = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(win), "Data Tokenizer");
    gtk_window_set_default_size(GTK_WINDOW(win), 960, 640);

    GtkBuilder *builder = gtk_builder_new_from_file("./ui/window.xml");

    GtkWidget *paned = GTK_WIDGET(gtk_builder_get_object(builder, "main_paned"));
    gtk_window_set_child(GTK_WINDOW(win), paned);

    w->window          = win;
    w->stack           = GTK_STACK(gtk_builder_get_object(builder, "content_stack"));
    w->file_entry      = GTK_EDITABLE(gtk_builder_get_object(builder, "file_entry"));
    w->status_label    = GTK_LABEL(gtk_builder_get_object(builder, "status_label"));
    w->results_list    = GTK_LIST_BOX(gtk_builder_get_object(builder, "results_list"));
    w->summary_label   = GTK_LABEL(gtk_builder_get_object(builder, "summary_label"));
    w->submit_btn      = GTK_BUTTON(gtk_builder_get_object(builder, "submit_btn"));
    w->cancel_btn      = GTK_BUTTON(gtk_builder_get_object(builder, "cancel_btn"));
    w->browse_btn      = GTK_BUTTON(gtk_builder_get_object(builder, "browse_btn"));
    w->confirm_check   = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "confirm_check"));
    w->confirm_list    = GTK_LIST_BOX(gtk_builder_get_object(builder, "confirm_list"));
    w->confirm_apply_btn= GTK_BUTTON(gtk_builder_get_object(builder, "confirm_apply_btn"));
    w->confirm_back_btn= GTK_BUTTON(gtk_builder_get_object(builder, "confirm_back_btn"));

    g_object_unref(builder);

    return win;
}

void view_clear_results(AppWidgets *w) {
    clear_list(w->results_list);
}

void view_set_status(AppWidgets *w, const char *text) {
    gtk_label_set_text(w->status_label, text);
}

void view_set_summary(AppWidgets *w, const char *text) {
    gtk_label_set_text(w->summary_label, text);
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

void view_set_busy(AppWidgets *w, gboolean busy) {
    if (busy) {
        gtk_button_set_label(w->submit_btn, "Analyzing...");
        gtk_widget_set_sensitive(GTK_WIDGET(w->submit_btn), FALSE);
    } else {
        gtk_button_set_label(w->submit_btn, "Submit");
        gtk_widget_set_sensitive(GTK_WIDGET(w->submit_btn), TRUE);
    }
}

void view_show_page(AppWidgets *w, const char *name) {
    gtk_stack_set_visible_child_name(w->stack, name);
}

const char* view_get_file_path(AppWidgets *w) {
    return gtk_editable_get_text(w->file_entry);
}

gboolean view_should_confirm(AppWidgets *w) {
    return gtk_check_button_get_active(w->confirm_check);
}

void view_populate_confirm(AppWidgets *w, const SensitiveReport *report) {
    clear_list(w->confirm_list);

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

        gtk_list_box_append(w->confirm_list, row);
    }
}

void view_apply_confirm(AppWidgets *w, SensitiveReport *report) {
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(w->confirm_list));
    while (child) {
        const char *field_name = g_object_get_data(G_OBJECT(child), "field-name");
        if (field_name) {
            GtkWidget *check = gtk_widget_get_first_child(child);
            if (check && GTK_IS_CHECK_BUTTON(check)) {
                gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(check));
                for (size_t j = 0; j < report->count; j++) {
                    if (strcmp(report->items[j].field_name, field_name) == 0) {
                        report->items[j].sensitive = active;
                    }
                }
            }
        }
        child = gtk_widget_get_next_sibling(child);
    }
}
