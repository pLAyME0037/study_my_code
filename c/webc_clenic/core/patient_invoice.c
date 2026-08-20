#include "patient_invoice.h"
#include "../src/db/db.h"
#include "../src/patient_invoice/patient_invoice.h"
#include "../core/crud.h"
#include "../core/header.h"
#include "../core/footer.h"
#include "../core/http/utils.h"

// ---------------------------------------------------------------------------
// Patient Medicine Invoice
// ---------------------------------------------------------------------------

static void render_pmi_list_page(String_Builder *sb, Pmi_Headers *rows,
                                 Pio_Options *patient_opts) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/pmi_list.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

static void render_pmi_edit_page(String_Builder *sb, long long id,
                                 const char *patient_name, const char *invoice_date,
                                 const char *amount_in_riel, const char *amount_in_dollar) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/pmi_edit.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

static void render_pmi_details_page(String_Builder *sb, long long import_id,
                                    const char *patient_name, const char *invoice_date,
                                    Pio_Options *med_opts, Pmi_Details *rows) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/pmi_details.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

void serve_pmi_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Pmi_Headers rows = {0};
    pmi_headers_load(db, &rows, -1);
    Pio_Options patient_opts = {0};
    pio_options_load(db, &patient_opts, "Patients", "name");

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Patient Medicine Invoice", "/patient-medicine-invoices");
    render_pmi_list_page(sb, &rows, &patient_opts);
    render_page_footer(sb);

    free(rows.items);
    free(patient_opts.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_pmi_create(Serve_Context *sc) {
    String_View body = sb_to_sv(sc->body);
    char patient_id[64] = {0}, invoice_date[64] = {0}, riel[128] = {0}, dollar[128] = {0};
    if (!form_find(body, "patient_id", patient_id, sizeof(patient_id)) || patient_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "invoice_date", invoice_date, sizeof(invoice_date));
    form_find(body, "amount_in_riel", riel, sizeof(riel));
    form_find(body, "amount_in_dollar", dollar, sizeof(dollar));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    long long new_id = 0;
    bool ok = pmi_insert(db, &new_id, atoll(patient_id), invoice_date, riel, dollar);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/patient-medicine-invoices/%lld/details", new_id));
}

void serve_pmi_edit(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Pmi_Headers rows = {0};
    pmi_headers_load(db, &rows, id);
    if (rows.count == 0) {
        free(rows.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Pmi_Header *r = &rows.items[0];

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Edit Patient Medicine Invoice", "/patient-medicine-invoices");
    render_pmi_edit_page(sb, r->id, r->patient_name, r->invoice_date,
                         r->amount_in_riel, r->amount_in_dollar);
    render_page_footer(sb);

    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_pmi_update(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char invoice_date[64] = {0}, riel[128] = {0}, dollar[128] = {0};
    form_find(body, "invoice_date", invoice_date, sizeof(invoice_date));
    form_find(body, "amount_in_riel", riel, sizeof(riel));
    form_find(body, "amount_in_dollar", dollar, sizeof(dollar));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = pmi_update(db, id, invoice_date, riel, dollar);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/patient-medicine-invoices");
}

void serve_pmi_delete(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = pmi_delete(db, id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/patient-medicine-invoices");
}

void serve_pmi_details(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Pmi_Headers headers = {0};
    pmi_headers_load(db, &headers, id);
    if (headers.count == 0) {
        free(headers.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Pmi_Header *h = &headers.items[0];

    Pmi_Details rows = {0};
    pmi_details_load(db, &rows, id);
    Pio_Options med_opts = {0};
    pio_options_load(db, &med_opts, "Medicines", "name");

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Invoice Details", "/patient-medicine-invoices");
    render_pmi_details_page(sb, h->id, h->patient_name, h->invoice_date, &med_opts, &rows);
    render_page_footer(sb);

    free(headers.items);
    free(med_opts.items);
    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_pmi_detail_create(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char medicine_id[64] = {0}, qty[128] = {0}, price[128] = {0}, amount[128] = {0}, currency[64] = {0};
    if (!form_find(body, "medicine_id", medicine_id, sizeof(medicine_id)) || medicine_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    if (!form_find(body, "qty", qty, sizeof(qty)) || qty[0] == '\0') {
        serve_error(sc, 400);
        return;
    }
    form_find(body, "price", price, sizeof(price));
    form_find(body, "amount", amount, sizeof(amount));
    form_find(body, "currency", currency, sizeof(currency));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = pmi_detail_insert(db, id, atoll(medicine_id), qty, price, amount, currency);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/patient-medicine-invoices/%d/details", id));
}

void serve_pmi_detail_delete(Serve_Context *sc, int id, int medicine_id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = pmi_detail_delete(db, id, medicine_id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/patient-medicine-invoices/%d/details", id));
}

// ---------------------------------------------------------------------------
// Patient Invoice Out
// ---------------------------------------------------------------------------

static void render_pio_list_page(String_Builder *sb, Pio_Headers *rows,
                                 Pio_Options *patient_opts, Pio_Options *room_opts) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/pio_list.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

static void render_pio_edit_page(String_Builder *sb, long long id,
                                 const char *patient_name, const char *room_name,
                                 const char *room_price, const char *start_date,
                                 const char *end_date, const char *room_day) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/pio_edit.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

static void render_pio_details_page(String_Builder *sb, long long import_id,
                                    const char *patient_name, const char *room_name,
                                    Pio_Options *daily_opts, Pio_Details *rows) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%d", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/pio_details.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
}

void serve_pio_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Pio_Headers rows = {0};
    pio_headers_load(db, &rows, -1);
    Pio_Options patient_opts = {0};
    pio_options_load(db, &patient_opts, "Patients", "name");
    Pio_Options room_opts = {0};
    pio_room_options_load(db, &room_opts);

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Patient Invoice Out", "/patient-invoice-out");
    render_pio_list_page(sb, &rows, &patient_opts, &room_opts);
    render_page_footer(sb);

    free(rows.items);
    free(patient_opts.items);
    free(room_opts.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_pio_create(Serve_Context *sc) {
    String_View body = sb_to_sv(sc->body);
    char patient_id[64] = {0}, room_id[64] = {0}, room_price[128] = {0};
    char start_date[64] = {0}, end_date[64] = {0}, room_day[128] = {0};
    if (!form_find(body, "patient_id", patient_id, sizeof(patient_id)) || patient_id[0] == '\0') {
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
    bool ok = pio_insert(db, &new_id, atoll(patient_id),
                         room_id[0] ? atoll(room_id) : 0,
                         room_price, start_date, end_date, room_day);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/patient-invoice-out/%lld/details", new_id));
}

void serve_pio_edit(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Pio_Headers rows = {0};
    pio_headers_load(db, &rows, id);
    if (rows.count == 0) {
        free(rows.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Pio_Header *r = &rows.items[0];

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Edit Patient Invoice Out", "/patient-invoice-out");
    render_pio_edit_page(sb, r->id, r->patient_name, r->room_name, r->room_price,
                         r->start_date, r->end_date, r->room_day);
    render_page_footer(sb);

    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_pio_update(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char room_price[128] = {0}, start_date[64] = {0}, end_date[64] = {0}, room_day[128] = {0};
    form_find(body, "room_price", room_price, sizeof(room_price));
    form_find(body, "start_date", start_date, sizeof(start_date));
    form_find(body, "end_date", end_date, sizeof(end_date));
    form_find(body, "room_day", room_day, sizeof(room_day));

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = pio_update(db, id, room_price, start_date, end_date, room_day);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/patient-invoice-out");
}

void serve_pio_delete(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = pio_delete(db, id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, "/patient-invoice-out");
}

void serve_pio_details(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    Pio_Headers headers = {0};
    pio_headers_load(db, &headers, id);
    if (headers.count == 0) {
        free(headers.items);
        sqlite3_close(db);
        serve_error(sc, 404);
        return;
    }
    Pio_Header *h = &headers.items[0];

    Pio_Details rows = {0};
    pio_details_load(db, &rows, id);
    Pio_Options daily_opts = {0};
    pio_options_load(db, &daily_opts, "PatientDailyInvoices", "invoice_date");

    String_Builder *sb = &sc->body;
    render_page_header(sb, "Invoice Out Details", "/patient-invoice-out");
    render_pio_details_page(sb, h->id, h->patient_name, h->room_name, &daily_opts, &rows);
    render_page_footer(sb);

    free(headers.items);
    free(daily_opts.items);
    free(rows.items);
    sqlite3_close(db);
    http_render_response(sc, 200, "text/html", sb_to_sv(sc->body));
}

void serve_pio_detail_create(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char daily_id[64] = {0};
    if (!form_find(body, "patient_daily_invoice_id", daily_id, sizeof(daily_id)) || daily_id[0] == '\0') {
        serve_error(sc, 400);
        return;
    }

    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = pio_detail_insert(db, id, atoll(daily_id));
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/patient-invoice-out/%d/details", id));
}

void serve_pio_detail_delete(Serve_Context *sc, int id, int daily_id) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }

    bool ok = pio_detail_delete(db, id, daily_id);
    sqlite3_close(db);

    if (!ok) { serve_error(sc, 500); return; }
    http_render_redirect(sc, 302, temp_sprintf("/patient-invoice-out/%d/details", id));
}
