#include "cairo.h"
#include "glib.h"
#include <gtk/gtk.h>

static cairo_surface_t *surface = NULL;

static void clear_serface(void) {
    cairo_t *cr;

    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_destroy(cr);
}

static void resize_cb(GtkWidget *widget, int width, int height, gpointer data) {
    if (surface) {
        cairo_surface_destroy(surface);
        surface = NULL;
    }
}
