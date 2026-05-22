#include "glib-object.h"
#include <gtk/gtk.h>

static void print_hello (GtkWidget *widget, gpointer data) {
  g_print ("Hello World\n");
}

static void print_gtk (GtkWidget *widget, gpointer data) {
  g_print ("GTK\n");
}

static void activate (GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *btn;
  GtkWidget *grid;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Hello");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  grid = gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW (window), grid);

  btn = gtk_button_new_with_label ("GTK");
  /* gtk_widget_set_halign(btn, GTK_ALIGN_END); */
  /* gtk_widget_set_valign(btn, GTK_ALIGN_END); */
  g_signal_connect(btn, "clicked", G_CALLBACK (print_gtk), NULL);
  /* gtk_window_set_child (GTK_WINDOW (window), btn); */
  gtk_grid_attach(GTK_GRID (grid), btn, 0, 0, 1, 1);

  btn = gtk_button_new_with_label ("Hello World");
  /* gtk_widget_set_halign(btn, GTK_ALIGN_CENTER); */
  /* gtk_widget_set_valign(btn, GTK_ALIGN_CENTER); */
  g_signal_connect(btn, "clicked", G_CALLBACK (print_hello), NULL);
  /* gtk_window_set_child (GTK_WINDOW (window), btn); */
  gtk_grid_attach(GTK_GRID (grid), btn, 1, 0, 2, 1);

  btn = gtk_button_new_with_label ("Quit");
  /* gtk_widget_set_halign(btn, GTK_ALIGN_CENTER); */
  /* gtk_widget_set_valign(btn, GTK_ALIGN_CENTER); */
  g_signal_connect_swapped(btn, "clicked", G_CALLBACK (gtk_window_destroy), window);
  /* gtk_window_set_child (GTK_WINDOW (window), btn); */
  gtk_grid_attach(GTK_GRID (grid), btn, 0, 2, 3, 1);

  gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
