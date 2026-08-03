#include <gtk/gtk.h>
#include "window_core.h"
#include "review_tab_core.h"
#include "../ui/table_tab.h"

static AppWidgets *self;

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

static void on_cancel_clicked(GtkButton *btn, gpointer user_data) {
    (void)btn;
    (void)user_data;
    gtk_editable_set_text(self->file_entry, "");
    view_set_status(self, "");
    view_clear_results(self);
    view_show_page(self, "home");
}

void controller_init(AppWidgets *w) {
    self = w;
    g_signal_connect(self->browse_btn, "clicked", G_CALLBACK(on_browse_clicked), NULL);
    g_signal_connect(self->cancel_btn, "clicked", G_CALLBACK(on_cancel_clicked), NULL);
    review_tab_init(w);
}
