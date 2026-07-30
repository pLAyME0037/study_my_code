#include <gtk/gtk.h>
#include "review_tab_core.h"
#include "../ui/review_tab.h"
#include "../ui/table_tab.h"

static AppWidgets *self;
static SensitiveReport cached_report = {0};

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

void review_tab_init(AppWidgets *w) {
    self = w;
    g_signal_connect(self->submit_btn, "clicked", G_CALLBACK(on_submit_clicked), NULL);
    g_signal_connect(self->confirm_apply_btn, "clicked", G_CALLBACK(on_confirm_apply_clicked), NULL);
    g_signal_connect(self->confirm_back_btn, "clicked", G_CALLBACK(on_confirm_back_clicked), NULL);
}
