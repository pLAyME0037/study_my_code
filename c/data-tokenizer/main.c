#include <gtk/gtk.h>
#include "coreui/window_core.h"

static void activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;
    static AppWidgets widgets = {0};
    GtkWidget *win = app_window_new(app, &widgets);
    controller_init(&widgets);
    gtk_window_present(GTK_WINDOW(win));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    app = gtk_application_new("token.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
