#include <gtk/gtk.h>

GtkWidget *app_window_new(GtkApplication *app) {

    GtkWidget *win = gtk_application_window_new(app);

    gtk_window_set_title(GTK_WINDOW(win), "Tokenizer");
    gtk_window_set_default_size(GTK_WINDOW(win), 1280, 720);
    gtk_window_maximize(GTK_WINDOW(win));


    GtkBuilder *builder = gtk_builder_new_from_file("./ui/window.xml");

    GtkWidget *box = GTK_WIDGET(gtk_builder_get_object(builder, "main_box"));
    gtk_window_set_child(GTK_WINDOW(win), box);

    g_object_unref(builder);

    gtk_window_present(GTK_WINDOW(win));

    return win;
}
