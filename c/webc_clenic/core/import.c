#include "import.h"
#include "../src/db/db.h"
#include "../src/import/import.h"
#include "../core/crud.h"
#include "../core/header.h"
#include "../core/footer.h"
#include "../core/http/utils.h"

static void render_import_list_page(String_Builder *sb,
                                    Import_Headers *rows) {
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

static void render_import_edit_page(String_Builder *sb,
                                    long long       id,
                                    const char     *date,
                                    const char     *amount)
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

static void render_import_details_page(String_Builder   *sb,
                                       long long         import_id,
                                       const char       *date,
                                       const char       *amount,
                                       Medicine_Options *med_opts,
                                       Import_Details   *rows)
{
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/import_details.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

void serve_import_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Import_Headers rows = {0};
    import_rows_load(db, &rows, -1);

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Medicine Import", "/medicine-imports");
    render_import_list_page(sb, &rows);
    render_page_footer(sb);

    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_import_create(Serve_Context *sc) {
    String_View body = sb_to_sv(sc->body);
    char date[64] = {0}, amount[128] = {0};
    form_find(body, "date", date, sizeof(date));
    form_find(body, "amount", amount, sizeof(amount));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = import_insert(db, date, amount);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/medicine-imports");
}

void serve_import_edit(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Import_Headers rows = {0};
    import_rows_load(db, &rows, id);
    if (rows.count == 0) {
        free(rows.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Import_Header *r = &rows.items[0];

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Edit Medicine Import", "/medicine-imports");
    render_import_edit_page(sb, r->id, r->date, r->amount);
    render_page_footer(sb);

    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_import_update(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char date[64] = {0}, amount[128] = {0};
    form_find(body, "date", date, sizeof(date));
    form_find(body, "amount", amount, sizeof(amount));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = import_update(db, id, date, amount);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/medicine-imports");
}

void serve_import_delete(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = import_delete(db, id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/medicine-imports");
}

void serve_import_details(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Import_Headers headers = {0};
    import_rows_load(db, &headers, id);
    if (headers.count == 0) {
        free(headers.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Import_Header *h = &headers.items[0];

    Import_Details rows = {0};
    import_details_load(db, &rows, id);
    Medicine_Options med_opts = {0};
    import_medicines_load(db, &med_opts);

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Import Details", "/medicine-imports");
    render_import_details_page(sb, h->id, h->date, h->amount,
                               &med_opts, &rows);
    render_page_footer(sb);

    free(headers.items);
    free(med_opts.items);
    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_import_detail_create(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char medicine_id[64] = {0}, qty[128] = {0}, price[128] = {0};
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

    bool ok = import_detail_insert(db, id, atoll(medicine_id), qty, price);
    sqlite3_close(db);

    if (!ok) {
        serve_error(sc, 500);
        return;
    }
    http_render_redirect(sc, 302, temp_sprintf("/medicine-imports/%d/details", id));
}

void serve_import_detail_delete(Serve_Context *sc, int id, int medicine_id) {
    sqlite3 *db = open_webc_db();
    if (!db) {
        serve_error(sc, 500);
        return;
    }

    bool ok = import_detail_delete(db, id, medicine_id);
    sqlite3_close(db);

    if (!ok) {
        serve_error(sc, 500);
        return;
    }
    http_render_redirect(sc, 302, temp_sprintf("/medicine-imports/%d/details", id));
}
