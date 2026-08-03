#include "text_tab.h"

#define PAGE_FREQ   "text_freq"
#define PAGE_BANNED "text_banned"

static GPtrArray *tab_names = NULL;

static void remove_text_tabs(AppWidgets *w) {
    if (!tab_names) return;
    for (guint i = 0; i < tab_names->len; i++) {
        const char *name = g_ptr_array_index(tab_names, i);
        GtkWidget *child = gtk_stack_get_child_by_name(w->stack, name);
        if (child)
            gtk_stack_remove(w->stack, child);
        g_free((char *)name);
    }
    g_ptr_array_set_size(tab_names, 0);
}

static GtkWidget *make_freq_row(size_t rank, const FreqKeyVal *f) {
    char buf[64];
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_margin_start(box, 8);
    gtk_widget_set_margin_end(box, 8);
    gtk_widget_set_margin_top(box, 2);
    gtk_widget_set_margin_bottom(box, 2);

    snprintf(buf, sizeof(buf), "%zu", rank);
    GtkWidget *lbl = gtk_label_new(buf);
    gtk_widget_set_size_request(lbl, 40, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new(f->word);
    gtk_widget_set_hexpand(lbl, TRUE);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_label_set_ellipsize(GTK_LABEL(lbl), PANGO_ELLIPSIZE_END);
    gtk_box_append(GTK_BOX(box), lbl);

    snprintf(buf, sizeof(buf), "%zu", f->value);
    lbl = gtk_label_new(buf);
    gtk_widget_set_size_request(lbl, 60, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 1.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    return box;
}

static GtkWidget *make_banned_row(const FreqKeyVal *f) {
    char buf[64];
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_margin_start(box, 8);
    gtk_widget_set_margin_end(box, 8);
    gtk_widget_set_margin_top(box, 2);
    gtk_widget_set_margin_bottom(box, 2);

    GtkWidget *lbl = gtk_label_new(f->word);
    gtk_widget_set_hexpand(lbl, TRUE);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_label_set_ellipsize(GTK_LABEL(lbl), PANGO_ELLIPSIZE_END);
    gtk_box_append(GTK_BOX(box), lbl);

    snprintf(buf, sizeof(buf), "%zu", f->value);
    lbl = gtk_label_new(buf);
    gtk_widget_set_size_request(lbl, 60, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 1.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new(f->pattern ? f->pattern : "");
    gtk_widget_set_size_request(lbl, 100, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    return box;
}

static GtkWidget *make_header_row(const char *c1, const char *c2, const char *c3,
                                  gboolean c1_num, gboolean c3_col) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_margin_start(box, 8);
    gtk_widget_set_margin_end(box, 8);
    gtk_widget_set_margin_top(box, 4);
    gtk_widget_set_margin_bottom(box, 2);

    GtkWidget *lbl = gtk_label_new(c1);
    if (c1_num) {
        gtk_widget_set_size_request(lbl, 40, -1);
    } else {
        gtk_widget_set_hexpand(lbl, TRUE);
    }
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new(c2);
    if (c1_num) {
        gtk_widget_set_hexpand(lbl, TRUE);
        gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
    } else {
        gtk_widget_set_size_request(lbl, 60, -1);
        gtk_label_set_xalign(GTK_LABEL(lbl), 1.0f);
    }
    gtk_box_append(GTK_BOX(box), lbl);

    lbl = gtk_label_new(c3);
    gtk_widget_set_size_request(lbl, c3_col ? 100 : 60, -1);
    gtk_label_set_xalign(GTK_LABEL(lbl), c3_col ? 0.0f : 1.0f);
    gtk_box_append(GTK_BOX(box), lbl);

    return box;
}

static GtkWidget *make_page(const char *title, GtkWidget *header, GtkWidget *list) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_margin_top(vbox, 8);

    GtkWidget *lbl = gtk_label_new(title);
    gtk_widget_set_halign(lbl, GTK_ALIGN_START);
    gtk_widget_set_margin_start(lbl, 8);
    gtk_widget_set_margin_end(lbl, 8);
    gtk_box_append(GTK_BOX(vbox), lbl);

    gtk_box_append(GTK_BOX(vbox), header);

    GtkWidget *sw = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw), list);
    gtk_widget_set_vexpand(sw, TRUE);
    gtk_box_append(GTK_BOX(vbox), sw);

    return vbox;
}

static GtkWidget *build_freq_page(const FreqKeyVals *ht) {
    char title[128];
    snprintf(title, sizeof(title), "Word Frequency - %zu unique words", ht->count);

    GtkWidget *list = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list), GTK_SELECTION_NONE);

    size_t n = 0;
    FreqKeyVal **arr = freq_keyvals_sorted(ht, &n);
    for (size_t i = 0; i < n; i++) {
        gtk_list_box_append(GTK_LIST_BOX(list), make_freq_row(i + 1, arr[i]));
    }
    free(arr);

    return make_page(title, make_header_row("Rank", "Word", "Count", TRUE, FALSE), list);
}

static GtkWidget *build_banned_page(const FreqKeyVals *ht) {
    char title[128];
    size_t banned = 0;
    for (size_t i = 0; i < ht->capacity; i++) {
        if (ht->items[i].occupied && ht->items[i].sensitive) banned++;
    }
    snprintf(title, sizeof(title), "Banned Words - %zu found", banned);

    GtkWidget *list = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list), GTK_SELECTION_NONE);

    size_t n = 0;
    FreqKeyVal **arr = freq_keyvals_sorted(ht, &n);
    for (size_t i = 0; i < n; i++) {
        if (arr[i]->sensitive) {
            gtk_list_box_append(GTK_LIST_BOX(list), make_banned_row(arr[i]));
        }
    }
    free(arr);

    return make_page(title, make_header_row("Word", "Count", "Pattern", FALSE, TRUE), list);
}

void view_populate_text_results(AppWidgets *w, const FreqKeyVals *ht) {
    remove_text_tabs(w);

    if (ht->count == 0) {
        view_set_status(w, "No words found.");
        view_show_page(w, "home");
        return;
    }

    if (!tab_names)
        tab_names = g_ptr_array_new();

    GtkWidget *freq_page = build_freq_page(ht);
    GtkWidget *banned_page = build_banned_page(ht);

    gtk_stack_add_titled(w->stack, freq_page, PAGE_FREQ, "Word Frequency");
    gtk_stack_add_titled(w->stack, banned_page, PAGE_BANNED, "Banned Words");

    g_ptr_array_add(tab_names, g_strdup(PAGE_FREQ));
    g_ptr_array_add(tab_names, g_strdup(PAGE_BANNED));

    view_show_page(w, PAGE_FREQ);
}

void view_clear_text_results(AppWidgets *w) {
    remove_text_tabs(w);
}
