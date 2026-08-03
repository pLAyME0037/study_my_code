#include "window.h"

GtkWidget *app_window_new(GtkApplication *app, AppWidgets *w) {
    GtkWidget *win = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(win), "Data Tokenizer");
    gtk_window_set_default_size(GTK_WINDOW(win), 960, 640);

    GtkBuilder *bp = gtk_builder_new_from_file("./ui/window.xml");
    GtkBuilder *bs = gtk_builder_new_from_file("./ui/sidebar.xml");
    GtkBuilder *bh = gtk_builder_new_from_file("./ui/home_tab.xml");
    GtkBuilder *br = gtk_builder_new_from_file("./ui/review_tab.xml");

    GtkWidget *paned       = GTK_WIDGET(gtk_builder_get_object(bp, "main_paned"));
    GtkWidget *sidebar     = GTK_WIDGET(gtk_builder_get_object(bs, "sidebar"));
    GtkWidget *home_page   = GTK_WIDGET(gtk_builder_get_object(bh, "home_page"));
    GtkWidget *confirm_page= GTK_WIDGET(gtk_builder_get_object(br, "confirm_page"));

    GtkStack *stack = GTK_STACK(gtk_stack_new());
    gtk_stack_set_transition_type(stack, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(stack, 200);

    gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), stack);
    gtk_paned_set_start_child(GTK_PANED(paned), sidebar);
    gtk_paned_set_end_child(GTK_PANED(paned), GTK_WIDGET(stack));
    gtk_window_set_child(GTK_WINDOW(win), paned);

    gtk_stack_add_titled(stack, home_page, "home", "Home");
    gtk_stack_add_titled(stack, confirm_page, "confirm", "Confirm Sensitivity");

    w->file_entry      = GTK_EDITABLE(gtk_builder_get_object(bh, "file_entry"));
    w->status_label    = GTK_LABEL(gtk_builder_get_object(bh, "status_label"));
    w->submit_btn      = GTK_BUTTON(gtk_builder_get_object(bh, "submit_btn"));
    w->cancel_btn      = GTK_BUTTON(gtk_builder_get_object(bh, "cancel_btn"));
    w->browse_btn      = GTK_BUTTON(gtk_builder_get_object(bh, "browse_btn"));
    w->confirm_check   = GTK_CHECK_BUTTON(gtk_builder_get_object(bh, "confirm_check"));

    w->confirm_list     = GTK_LIST_BOX(gtk_builder_get_object(br, "confirm_list"));
    w->confirm_apply_btn= GTK_BUTTON(gtk_builder_get_object(br, "confirm_apply_btn"));
    w->confirm_back_btn = GTK_BUTTON(gtk_builder_get_object(br, "confirm_back_btn"));

    g_object_unref(bp);
    g_object_unref(bs);
    g_object_unref(bh);
    g_object_unref(br);

    w->window = win;
    w->stack  = stack;

    return win;
}

void view_set_status(AppWidgets *w, const char *text) {
    gtk_label_set_text(w->status_label, text);
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
