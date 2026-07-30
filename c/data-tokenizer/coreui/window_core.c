#include <gtk/gtk.h>
#include "window_core.h"

static AppWidgets *self;
static SensitiveReport cached_report = {0};

static void on_file_opened(GObject *dialog, GAsyncResult *result, gpointer user_data) {
    (void)user_data;
    GError *err = NULL;
    GFile *file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(dialog), result, &err);
    if (err) {
        if (!g_error_matches(err, G_IO_ERROR, G_IO_ERROR_CANCELLED))
            view_set_status(self, err->message);
        g_error_free(err);
        return;
    }
    if (file) {
        char *path = g_file_get_path(file);
        gtk_editable_set_text(self->file_entry, path);
        g_free(path);
        g_object_unref(file);
    }
    g_object_unref(dialog);
}

static void on_browse_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    (void)user_data;
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Select JSON File");

    GtkFileFilter *json_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(json_filter, "JSON Files (*.json)");
    gtk_file_filter_add_pattern(json_filter, "*.json");

    GtkFileFilter *all_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(all_filter, "All Files");
    gtk_file_filter_add_pattern(all_filter, "*");

    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(filters, json_filter);
    g_list_store_append(filters, all_filter);
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
    gtk_file_dialog_set_default_filter(dialog, json_filter);

    gtk_file_dialog_open(dialog, GTK_WINDOW(self->window), NULL, on_file_opened, NULL);
}

static void free_cached_report(void) {
    if (cached_report.items) {
        free(cached_report.items);
        cached_report.items = NULL;
        cached_report.count = 0;
        cached_report.capacity = 0;
    }
}

static void on_submit_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    (void)user_data;
    const char *path = view_get_file_path(self);
    if (!path || *path == '\0') {
        view_set_status(self, "Please select a JSON file.");
        return;
    }
    if (!g_file_test(path, G_FILE_TEST_EXISTS)) {
        view_set_status(self, "File not found.");
        return;
    }

    free_cached_report();

    view_set_busy(self, TRUE);
    view_set_status(self, "");

    gboolean ok = find_sensetive_data(path, &cached_report);

    view_set_busy(self, FALSE);

    if (!ok) {
        view_set_status(self, "Failed to parse or read JSON file.");
        return;
    }

    if (view_should_confirm(self)) {
        view_populate_confirm(self, &cached_report);
        view_show_page(self, "confirm");
    } else {
        view_populate_results(self, &cached_report);
        free_cached_report();
        view_show_page(self, "results");
    }
}

static void on_confirm_apply_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    (void)user_data;
    view_apply_confirm(self, &cached_report);
    view_populate_results(self, &cached_report);
    free_cached_report();
    view_show_page(self, "results");
}

static void on_confirm_back_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    (void)user_data;
    free_cached_report();
    gtk_editable_set_text(self->file_entry, "");
    view_set_status(self, "");
    view_show_page(self, "home");
}

static void on_cancel_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    (void)user_data;
    gtk_editable_set_text(self->file_entry, "");
    view_set_status(self, "");
    view_clear_results(self);
    view_set_summary(self, "");
    free_cached_report();
    view_show_page(self, "home");
}

void controller_init(AppWidgets *w) {
    self = w;
    g_signal_connect(self->browse_btn, "clicked", G_CALLBACK(on_browse_clicked), NULL);
    g_signal_connect(self->submit_btn, "clicked", G_CALLBACK(on_submit_clicked), NULL);
    g_signal_connect(self->cancel_btn, "clicked", G_CALLBACK(on_cancel_clicked), NULL);
    g_signal_connect(self->confirm_apply_btn, "clicked", G_CALLBACK(on_confirm_apply_clicked), NULL);
    g_signal_connect(self->confirm_back_btn, "clicked", G_CALLBACK(on_confirm_back_clicked), NULL);
}
