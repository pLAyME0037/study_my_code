#include "import.h"
#include "../src/db/db.h"
#include "../src/import/import.h"
#include "../core/crud.h"
#include "../core/header.h"
#include "../core/footer.h"
#include "../core/http/utils.h"

static void render_import_list_page(String_Builder   *sb,
                                    Import_Rows      *rows,
                                    Medicine_Options *med_opts)
{
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/import_list.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

static void render_import_edit_page(String_Builder   *sb,
                                    long long         id,
                                    const char       *date,
                                    const char       *medicine_id,
                                    const char       *qty,
                                    const char       *price,
                                    Medicine_Options *med_opts)
{
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/import_edit.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

void serve_import_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) {
        serve_error(sc, 500);
        return;
    }

    Import_Rows rows = {0};
    Medicine_Options med_opts = {0};
    import_rows_load(db, &rows, -1);
    import_medicines_load(db, &med_opts);

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Medicine Import", "/medicine-imports");
    render_import_list_page(sb, &rows, &med_opts);
    render_page_footer(sb);

    free(rows.items);
    free(med_opts.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_import_create(Serve_Context *sc) {
    String_View body = sb_to_sv(sc->body);
    char date[64] = {0}, medicine_id[64] = {0}, qty[128] = {0}, price[128] = {0};
    form_find(body, "date", date, sizeof(date));
    if (!form_find(body, "medicine_id", medicine_id, sizeof(medicine_id)) ||
        medicine_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    if (!form_find(body, "qty", qty, sizeof(qty)) || qty[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "price", price, sizeof(price));

    sqlite3 *db = open_webc_db();
    if (!db) {
        serve_error(sc, 500);
        return;
    }

    bool ok = import_insert_line(db, date, atoll(medicine_id), qty, price);
    sqlite3_close(db);

    if (!ok) {
        serve_error(sc, 500);
        return;
    }
    http_render_redirect(sc, 302, "/medicine-imports");
}

void serve_import_edit(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) {
        serve_error(sc, 500);
        return;
    }

    Import_Rows rows = {0};
    Medicine_Options med_opts = {0};
    import_rows_load(db, &rows, id);
    import_medicines_load(db, &med_opts);
    if (rows.count == 0) {
        free(rows.items);
        free(med_opts.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Import_Row *r = &rows.items[0];

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Edit Medicine Import", "/medicine-imports");
    render_import_edit_page(sb, r->id, r->date, r->medicine_id, r->qty, r->price,
                            &med_opts);
    render_page_footer(sb);

    free(rows.items);
    free(med_opts.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_import_update(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char date[64] = {0}, medicine_id[64] = {0}, qty[128] = {0}, price[128] = {0};
    form_find(body, "date", date, sizeof(date));
    if (!form_find(body, "medicine_id", medicine_id, sizeof(medicine_id)) ||
        medicine_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    if (!form_find(body, "qty", qty, sizeof(qty)) || qty[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "price", price, sizeof(price));

    sqlite3 *db = open_webc_db();
    if (!db) {
        serve_error(sc, 500);
        return;
    }

    bool ok = import_update_line(db, id, date, atoll(medicine_id), qty, price);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/medicine-imports");
}

void serve_import_delete(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) {
        serve_error(sc, 500);
        return;
    }

    bool ok = import_delete(db, id);
    sqlite3_close(db);

    if (!ok) {
        serve_error(sc, 500);
        return;
    }
    http_render_redirect(sc, 302, "/medicine-imports");
}
