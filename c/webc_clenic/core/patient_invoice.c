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

static void render_pmi_list_page(String_Builder *sb,
                                 Pmi_Headers    *rows,
                                 Pio_Options    *patient_opts)
{
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

void serve_pmi_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) {
		serve_error(sc, 500);
		return;
	}

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
    form_find(body, "invoice_date", invoice_date, sizeof(invoice_date));
    form_find(body, "amount_in_riel", riel, sizeof(riel));
    form_find(body, "amount_in_dollar", dollar, sizeof(dollar));

    sqlite3 *db = open_webc_db();
    if (!db) {
		serve_error(sc, 500);
		return;
	}

    long long new_id = 0;
    bool ok = pmi_insert(db, &new_id, atoll(patient_id), invoice_date, riel, dollar);
    sqlite3_close(db);

    if (!ok) {
		serve_error(sc, 500);
		return;
	}
    http_render_redirect(sc, 302, "/patients");
}

void serve_pmi_update(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char invoice_date[64] = {0}, riel[128] = {0}, dollar[128] = {0};
    form_find(body, "invoice_date", invoice_date, sizeof(invoice_date));
    form_find(body, "amount_in_riel", riel, sizeof(riel));
    form_find(body, "amount_in_dollar", dollar, sizeof(dollar));

    sqlite3 *db = open_webc_db();
    if (!db) {
		serve_error(sc, 500);
		return;
	}

    bool ok = pmi_update(db, id, invoice_date, riel, dollar);
    sqlite3_close(db);

    if (!ok) {
		serve_error(sc, 500);
		return;
	}
    http_render_redirect(sc, 302, "/patients");
}

void serve_pmi_delete(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) {
		serve_error(sc, 500);
		return;
	}

    bool ok = pmi_delete(db, id);
    sqlite3_close(db);

    if (!ok) {
		serve_error(sc, 500);
		return;
	}
    http_render_redirect(sc, 302, "/patients");
}

// ---------------------------------------------------------------------------
// Patient Invoice Out
// ---------------------------------------------------------------------------

static void render_pio_list_page(String_Builder *sb,
                                 Pio_Headers    *rows,
                                 Pio_Options    *patient_opts,
                                 Pio_Options    *room_opts)
{
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

void serve_pio_list(Serve_Context *sc) {
    sqlite3 *db = open_webc_db();
    if (!db) {
        serve_error(sc, 500);
        return;
    }

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
    char patient_id[64]  = {0};
    char room_id[64]     = {0};
    char room_price[128] = {0};
    char start_date[64]  = {0};
    char end_date[64]    = {0};
    char room_day[128]   = {0};

    if (!form_find(body, "patient_id", patient_id, sizeof(patient_id))
        || patient_id[0] == '\0') {
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
    form_find(body, "room_id",    room_id,    sizeof(room_id));
    form_find(body, "room_price", room_price, sizeof(room_price));
    form_find(body, "start_date", start_date, sizeof(start_date));
    form_find(body, "end_date",   end_date,   sizeof(end_date));
    form_find(body, "room_day",   room_day,   sizeof(room_day));

    sqlite3 *db = open_webc_db();
    if (!db) {
		serve_error(sc, 500);
		return;
	}

    long long new_id = 0;
    bool ok = pio_insert(db, &new_id, atoll(patient_id),
                         room_id[0] ? atoll(room_id) : 0,
                         room_price, start_date, end_date, room_day);
    sqlite3_close(db);

    if (!ok) {
        serve_error(sc, 500);
        return;
    }
    http_render_redirect(sc, 302, "/patients");
}

void serve_pio_update(Serve_Context *sc, int id) {
    String_View body = sb_to_sv(sc->body);
    char room_price[128] = {0};
    char start_date[64]  = {0};
    char end_date[64]    = {0};
    char room_day[128]   = {0};
    form_find(body, "room_price", room_price, sizeof(room_price));
    form_find(body, "start_date", start_date, sizeof(start_date));
    form_find(body, "end_date",   end_date,   sizeof(end_date));
    form_find(body, "room_day",   room_day,   sizeof(room_day));

    sqlite3 *db = open_webc_db();
    if (!db) {
        serve_error(sc, 500);
        return;
    }

    bool ok = pio_update(db, id, room_price, start_date, end_date, room_day);
    sqlite3_close(db);

    if (!ok) {
        serve_error(sc, 500);
        return;
    }
    http_render_redirect(sc, 302, "/patients");
}

void serve_pio_delete(Serve_Context *sc, int id) {
    sqlite3 *db = open_webc_db();
    if (!db) {
        serve_error(sc, 500);
        return;
    }

    bool ok = pio_delete(db, id);
    sqlite3_close(db);

    if (!ok) {
        serve_error(sc, 500);
        return;
    }
    http_render_redirect(sc, 302, "/patients");
}
