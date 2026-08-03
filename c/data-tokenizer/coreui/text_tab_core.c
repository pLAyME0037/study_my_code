#include <gtk/gtk.h>
#include "text_tab_core.h"
#include "../ui/text_tab.h"
#include "../third_party/hash_table.h"

static FreqKeyVals cached_freq = {0};

static void free_cached_freq(void) {
    freq_keyvals_free(&cached_freq);
}

bool text_analyze(AppWidgets *w) {
    const char *path = view_get_file_path(w);
    if (!path || *path == '\0') {
        view_set_status(w, "Please select a text file.");
        return false;
    }
    if (!g_file_test(path, G_FILE_TEST_EXISTS)) {
        view_set_status(w, "File not found.");
        return false;
    }

    view_set_busy(w, TRUE);
    view_set_status(w, "");

    Nob_String_Builder sb = {0};
    if (!read_entire_file(path, &sb)) {
        view_set_status(w, "Failed to read file.");
        view_set_busy(w, FALSE);
        return false;
    }

    free_cached_freq();
    bool ok = hash_analysis(sb_to_sv(sb), path, &cached_freq);

    if (ok) {
        view_populate_text_results(w, &cached_freq);
    } else {
        view_set_status(w, "Text analysis failed.");
    }

    free_cached_freq();
    sb_free(sb);
    view_set_busy(w, FALSE);

    return ok;
}

void text_clear_results(AppWidgets *w) {
    free_cached_freq();
    view_clear_text_results(w);
}
