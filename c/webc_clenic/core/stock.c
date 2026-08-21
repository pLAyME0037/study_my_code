#include <stdlib.h>
#include "../core/header.h"
#include "../core/footer.h"

#include "stock.h"
#include "../src/db/db.h"
#include "../src/stock/stock.h"
#include "../core/crud.h"

static void render_stock_list_page(Serve_Context *sc, Stock_Rows rows) {
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

void serve_stock_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Stock_Rows rows = {0};
    load_stocks(db, &rows, -1);
    sqlite3_close(db);

    sc->body.count = 0;
    render_stock_list_page(sc, rows);
    free(rows.items);

    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}