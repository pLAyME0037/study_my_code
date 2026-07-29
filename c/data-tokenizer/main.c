#include <gtk/gtk.h>
#include "ui/window.h"
#include "third_party/hash_table.h"
#include "third_party/json_format.h"

int main(int argc, char **argv) {
    // const char *file_path = "./data_2mb.json";

    // {
    //     String_Builder sb = {0};
    //     if (!nob_read_entire_file(file_path, &sb)) return 1;
    //     String_View content = {
    //         .count = sb.count,
    //         .data  = sb.items,
    //     };
    //
    //     if (hash_analysis(content, file_path)) return 1;
    // }
    // {
    //     SensitiveReport report = {0};
    //
    //     if (!find_sensetive_data(file_path, &report)) return 1;
    //
    //     confirm_sensitive_fields(&report);
    //     print_report(&report);
    //
    //     free(report.items);
    // }

    GtkApplication *app = gtk_application_new("token.app", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(app_window_new), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
