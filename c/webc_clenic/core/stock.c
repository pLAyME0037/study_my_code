#include <stdlib.h>
#include "../core/header.h"
#include "../core/footer.h"

#include "stock.h"
#include "../src/db/db.h"
#include "../src/stock/stock.h"
#include "../core/crud.h"
#include "../core/http/utils.h"

static void render_stock_list_page(Serve_Context *sc, Stock_Rows rows, Stock_Medicine_Options opts) {
    String_Builder *sb = &sc->body;
    render_page_header(sb, "Medicine Stock", "/medicine-stock");
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(x) sb_appendf(sb, "%d", (x));
#define LLINT(x) sb_appendf(sb, "%lld", (long long)(x));
#define STR(x) sb_append_cstr(sb, (x) ? (x) : "");
#define ESCAPED(x) sb_append_html_escaped(sb, (x) ? (x) : "");
#define PAGE_TITLE "Medicine Stock"
#include "../auto_ctrl/cttochtml/stock.h"
#undef PAGE_TITLE
#undef ESCAPED
#undef STR
#undef LLINT
#undef INT
#undef OUT
    render_page_footer(sb);
}

static void render_stock_edit_page(Serve_Context *sc, int id, Stock_Row row) {
    String_Builder *sb = &sc->body;
    render_page_header(sb, "Edit Medicine Stock", "/medicine-stock");
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(x) sb_appendf(sb, "%d", (x));
#define LLINT(x) sb_appendf(sb, "%lld", (long long)(x));
#define STR(x) sb_append_cstr(sb, (x) ? (x) : "");
#define ESCAPED(x) sb_append_html_escaped(sb, (x) ? (x) : "");
#define PAGE_TITLE "Edit Medicine Stock"
#include "../auto_ctrl/cttochtml/stock_edit.h"
#undef PAGE_TITLE
#undef ESCAPED
#undef STR
#undef LLINT
#undef INT
#undef OUT
    render_page_footer(sb);
}

void serve_stock_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Stock_Rows rows = {0};
    Stock_Medicine_Options opts = {0};
    load_stocks(db, &rows, -1);
    load_medicine_options(db, &opts);
    sqlite3_close(db);

    sc->body.count = 0;
    render_stock_list_page(sc, rows, opts);
    free(rows.items);
    free(opts.items);

    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_stock_create(Serve_Context *sc) {
    String_View body = sb_to_sv(sc->body);
    char medicine_id[64] = {0}, stock_qty[128] = {0}, date_last_import[64] = {0};
    if (!form_find(body, "medicine_id", medicine_id, sizeof(medicine_id)) || medicine_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    if (!form_find(body, "stock_qty", stock_qty, sizeof(stock_qty)) || stock_qty[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "date_last_import", date_last_import, sizeof(date_last_import));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }
    bool ok = insert_stock(db, atoll(medicine_id), stock_qty, date_last_import);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/medicine-stock");
}

void serve_stock_edit(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Stock_Rows rows = {0};
    load_stocks(db, &rows, id);
    sqlite3_close(db);

    if (rows.count == 0) {
        free(rows.items);
        serve_error(sc, 404);
        return;
    }
    Stock_Row row = rows.items[0];

    sc->body.count = 0;
    render_stock_edit_page(sc, id, row);
    free(rows.items);

    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_stock_update(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char stock_qty[128] = {0}, date_last_import[64] = {0};
    if (!form_find(body, "stock_qty", stock_qty, sizeof(stock_qty)) || stock_qty[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "date_last_import", date_last_import, sizeof(date_last_import));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }
    bool ok = update_stock(db, id, stock_qty, date_last_import);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/medicine-stock");
}

void serve_stock_delete(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = delete_stock(db, id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/medicine-stock");
}

