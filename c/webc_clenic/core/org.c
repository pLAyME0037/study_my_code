#include "org.h"
#include "../src/db/db.h"
#include "../src/patient_invoice/patient_invoice.h"
#include "../src/org/org.h"
#include "../core/crud.h"
#include "../core/header.h"
#include "../core/footer.h"
#include "../core/http/utils.h"

// ---------------------------------------------------------------------------
// Organization Daily Invoice
// ---------------------------------------------------------------------------

static void render_org_daily_list_page(String_Builder *sb, Org_Daily_Headers *rows,
                                       Pio_Options *org_opts, Pio_Options *patient_opts) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/org_daily_list.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

static void render_org_daily_edit_page(String_Builder *sb, long long id,
                                       const char *org_name, const char *patient_name,
                                       const char *invoice_date, const char *amount_in_riel,
                                       const char *amount_in_dollar) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/org_daily_edit.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

static void render_org_daily_details_page(String_Builder *sb, long long import_id,
                                          const char *org_name, const char *patient_name,
                                          const char *invoice_date, Pio_Options *ill_opts,
                                          Org_Daily_Details *rows) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/org_daily_details.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

void serve_org_daily_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Org_Daily_Headers rows = {0};
    org_daily_headers_load(db, &rows, -1);
    Pio_Options org_opts = {0};
    pio_options_load(db, &org_opts, "Organizations", "name");
    Pio_Options patient_opts = {0};
    pio_options_load(db, &patient_opts, "Patients", "name");

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Organization Daily Invoice", "/organization-daily-invoices");
    render_org_daily_list_page(sb, &rows, &org_opts, &patient_opts);
    render_page_footer(sb);

    free(rows.items);
    free(org_opts.items);
    free(patient_opts.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_org_daily_create(Serve_Context *sc) {
    String_View body = sb_to_sv(sc->body);
    char organization_id[64] = {0}, patient_id[64] = {0}, invoice_date[64] = {0};
    char riel[128] = {0}, dollar[128] = {0};
    if (!form_find(body, "organization_id", organization_id, sizeof(organization_id)) || organization_id[0] == '\0') {
        if (sc->query_string.count > 0) {
            char param[] = "organization_id=";
            if (sv_starts_with(sc->query_string, sv_from_cstr(param))) {
                size_t offset = strlen(param);
                size_t remaining = sc->query_string.count - offset;
                size_t copy_len = remaining < sizeof(organization_id) - 1 ? remaining : sizeof(organization_id) - 1;
                memcpy(organization_id, (char *)sc->query_string.data + offset, copy_len);
                organization_id[copy_len] = '\0';
            }
        }
    }
    if (organization_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "patient_id", patient_id, sizeof(patient_id));
    form_find(body, "invoice_date", invoice_date, sizeof(invoice_date));
    form_find(body, "amount_in_riel", riel, sizeof(riel));
    form_find(body, "amount_in_dollar", dollar, sizeof(dollar));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    long long new_id = 0;
    bool ok = org_daily_insert(db, &new_id, atoll(organization_id),
                               patient_id[0] ? atoll(patient_id) : 0,
                               invoice_date, riel, dollar);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/organization-daily-invoices/%lld/details", new_id));
}

void serve_org_daily_edit(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Org_Daily_Headers rows = {0};
    org_daily_headers_load(db, &rows, id);
    if (rows.count == 0) {
        free(rows.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Org_Daily_Header *r = &rows.items[0];

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Edit Org Daily Invoice", "/organization-daily-invoices");
    render_org_daily_edit_page(sb, r->id, r->org_name, r->patient_name, r->invoice_date,
                               r->amount_in_riel, r->amount_in_dollar);
    render_page_footer(sb);

    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_org_daily_update(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char invoice_date[64] = {0}, riel[128] = {0}, dollar[128] = {0};
    form_find(body, "invoice_date", invoice_date, sizeof(invoice_date));
    form_find(body, "amount_in_riel", riel, sizeof(riel));
    form_find(body, "amount_in_dollar", dollar, sizeof(dollar));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_daily_update(db, id, invoice_date, riel, dollar);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/organization-daily-invoices");
}

void serve_org_daily_delete(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_daily_delete(db, id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/organization-daily-invoices");
}

void serve_org_daily_details(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Org_Daily_Headers headers = {0};
    org_daily_headers_load(db, &headers, id);
    if (headers.count == 0) {
        free(headers.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Org_Daily_Header *h = &headers.items[0];

    Org_Daily_Details rows = {0};
    org_daily_details_load(db, &rows, id);
    Pio_Options ill_opts = {0};
    pio_options_load(db, &ill_opts, "IllTypes", "type");

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Org Daily Details", "/organization-daily-invoices");
    render_org_daily_details_page(sb, h->id, h->org_name, h->patient_name,
                                  h->invoice_date, &ill_opts, &rows);
    render_page_footer(sb);

    free(headers.items);
    free(ill_opts.items);
    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_org_daily_detail_create(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char ill_id[64] = {0}, price[128] = {0}, currency[64] = {0};
    if (!form_find(body, "ill_id", ill_id, sizeof(ill_id)) || ill_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    if (!form_find(body, "price", price, sizeof(price)) || price[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "currency", currency, sizeof(currency));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_daily_detail_insert(db, id, atoll(ill_id), price, currency);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/organization-daily-invoices/%d/details", id));
}

void serve_org_daily_detail_delete(Serve_Context *sc, int id, int ill_id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_daily_detail_delete(db, id, ill_id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/organization-daily-invoices/%d/details", id));
}

// ---------------------------------------------------------------------------
// Organization Balance
// ---------------------------------------------------------------------------

static void render_org_balance_list_page(String_Builder *sb, Org_Balance_Rows *rows,
                                         Pio_Options *org_opts, Pio_Options *inv_opts) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/org_balance_list.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

void serve_org_balance_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Org_Balance_Rows rows = {0};
    org_balances_load(db, &rows);
    Pio_Options org_opts = {0};
    pio_options_load(db, &org_opts, "Organizations", "name");
    Pio_Options inv_opts = {0};
    pio_options_load(db, &inv_opts, "OrganizationInvoices", "invoice_date");

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Organization Balance", "/organization-balance");
    render_org_balance_list_page(sb, &rows, &org_opts, &inv_opts);
    render_page_footer(sb);

    free(rows.items);
    free(org_opts.items);
    free(inv_opts.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_org_balance_create(Serve_Context *sc) {
    String_View body = sb_to_sv(sc->body);
    char organization_id[64] = {0}, organization_invoice_id[64] = {0};
    char balance_amount[128] = {0}, balance[128] = {0};
    if (!form_find(body, "organization_id", organization_id, sizeof(organization_id)) || organization_id[0] == '\0') {
        if (sc->query_string.count > 0) {
            char param[] = "organization_id=";
            if (sv_starts_with(sc->query_string, sv_from_cstr(param))) {
                size_t offset = strlen(param);
                size_t remaining = sc->query_string.count - offset;
                size_t copy_len = remaining < sizeof(organization_id) - 1 ? remaining : sizeof(organization_id) - 1;
                memcpy(organization_id, (char *)sc->query_string.data + offset, copy_len);
                organization_id[copy_len] = '\0';
            }
        }
    }
    if (organization_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    if (!form_find(body, "organization_invoice_id", organization_invoice_id, sizeof(organization_invoice_id)) || organization_invoice_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "balance_amount", balance_amount, sizeof(balance_amount));
    form_find(body, "balance", balance, sizeof(balance));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_balance_insert(db, atoll(organization_id), atoll(organization_invoice_id),
                                 balance_amount, balance);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/organization-balance");
}

void serve_org_balance_delete(Serve_Context *sc, int org_id, int invoice_id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_balance_delete(db, org_id, invoice_id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/organization-balance");
}

// ---------------------------------------------------------------------------
// Organization Invoice Out
// ---------------------------------------------------------------------------

static void render_org_out_list_page(String_Builder *sb, Org_Out_Headers *rows,
                                     Pio_Options *org_opts, Pio_Options *patient_opts,
                                     Pio_Options *room_opts) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/org_out_list.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

static void render_org_out_edit_page(String_Builder *sb, long long id,
                                     const char *org_name, const char *patient_name,
                                     const char *room_name, const char *room_price,
                                     const char *start_date, const char *end_date,
                                     const char *room_day) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/org_out_edit.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

static void render_org_out_details_page(String_Builder *sb, long long import_id,
                                        const char *org_name, const char *patient_name,
                                        Pio_Options *inv_opts, Org_Out_Details *rows) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/org_out_details.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

void serve_org_out_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Org_Out_Headers rows = {0};
    org_out_headers_load(db, &rows, -1);
    Pio_Options org_opts = {0};
    pio_options_load(db, &org_opts, "Organizations", "name");
    Pio_Options patient_opts = {0};
    pio_options_load(db, &patient_opts, "Patients", "name");
    Pio_Options room_opts = {0};
    pio_room_options_load(db, &room_opts);

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Organization Invoice Out", "/organization-invoice-out");
    render_org_out_list_page(sb, &rows, &org_opts, &patient_opts, &room_opts);
    render_page_footer(sb);

    free(rows.items);
    free(org_opts.items);
    free(patient_opts.items);
    free(room_opts.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_org_out_create(Serve_Context *sc) {
    String_View body = sb_to_sv(sc->body);
    char organization_id[64] = {0}, patient_id[64] = {0}, room_id[64] = {0};
    char room_price[128] = {0}, start_date[64] = {0}, end_date[64] = {0}, room_day[128] = {0};
    if (!form_find(body, "organization_id", organization_id, sizeof(organization_id)) || organization_id[0] == '\0') {
        if (sc->query_string.count > 0) {
            char param[] = "organization_id=";
            if (sv_starts_with(sc->query_string, sv_from_cstr(param))) {
                size_t offset = strlen(param);
                size_t remaining = sc->query_string.count - offset;
                size_t copy_len = remaining < sizeof(organization_id) - 1 ? remaining : sizeof(organization_id) - 1;
                memcpy(organization_id, (char *)sc->query_string.data + offset, copy_len);
                organization_id[copy_len] = '\0';
            }
        }
    }
    if (organization_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    if (!form_find(body, "patient_id", patient_id, sizeof(patient_id)) || patient_id[0] == '\0') {
        if (sc->query_string.count > 0) {
            char param[] = "patient_id=";
            if (sv_starts_with(sc->query_string, sv_from_cstr(param))) {
                size_t offset = strlen(param);
                size_t remaining = sc->query_string.count - offset;
                size_t copy_len = remaining < sizeof(patient_id) - 1 ? remaining : sizeof(patient_id) - 1;
                memcpy(patient_id, (char *)sc->query_string.data + offset, copy_len);
                patient_id[copy_len] = '\0';
            }
        }
    }
    if (patient_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "room_id", room_id, sizeof(room_id));
    form_find(body, "room_price", room_price, sizeof(room_price));
    form_find(body, "start_date", start_date, sizeof(start_date));
    form_find(body, "end_date", end_date, sizeof(end_date));
    form_find(body, "room_day", room_day, sizeof(room_day));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    long long new_id = 0;
    bool ok = org_out_insert(db, &new_id, atoll(organization_id), atoll(patient_id),
                             room_id[0] ? atoll(room_id) : 0,
                             room_price, start_date, end_date, room_day);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/organization-invoice-out/%lld/details", new_id));
}

void serve_org_out_edit(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Org_Out_Headers rows = {0};
    org_out_headers_load(db, &rows, id);
    if (rows.count == 0) {
        free(rows.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Org_Out_Header *r = &rows.items[0];

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Edit Org Invoice Out", "/organization-invoice-out");
    render_org_out_edit_page(sb, r->id, r->org_name, r->patient_name, r->room_name,
                             r->room_price, r->start_date, r->end_date, r->room_day);
    render_page_footer(sb);

    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_org_out_update(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char room_price[128] = {0}, start_date[64] = {0}, end_date[64] = {0}, room_day[128] = {0};
    form_find(body, "room_price", room_price, sizeof(room_price));
    form_find(body, "start_date", start_date, sizeof(start_date));
    form_find(body, "end_date", end_date, sizeof(end_date));
    form_find(body, "room_day", room_day, sizeof(room_day));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_out_update(db, id, room_price, start_date, end_date, room_day);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/organization-invoice-out");
}

void serve_org_out_delete(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_out_delete(db, id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/organization-invoice-out");
}

void serve_org_out_details(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Org_Out_Headers headers = {0};
    org_out_headers_load(db, &headers, id);
    if (headers.count == 0) {
        free(headers.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Org_Out_Header *h = &headers.items[0];

    Org_Out_Details rows = {0};
    org_out_details_load(db, &rows, id);
    Pio_Options inv_opts = {0};
    pio_options_load(db, &inv_opts, "OrganizationInvoices", "invoice_date");

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Org Invoice Out Details", "/organization-invoice-out");
    render_org_out_details_page(sb, h->id, h->org_name, h->patient_name, &inv_opts, &rows);
    render_page_footer(sb);

    free(headers.items);
    free(inv_opts.items);
    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_org_out_detail_create(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char invoice_id[64] = {0};
    if (!form_find(body, "organization_invoice_id", invoice_id, sizeof(invoice_id)) || invoice_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_out_detail_insert(db, id, atoll(invoice_id));
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/organization-invoice-out/%d/details", id));
}

void serve_org_out_detail_delete(Serve_Context *sc, int id, int invoice_id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = org_out_detail_delete(db, id, invoice_id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/organization-invoice-out/%d/details", id));
}
