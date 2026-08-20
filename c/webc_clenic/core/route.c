#include <stdio.h>

#include "route.h"
#include "notes.h"
#include "version.h"
#include "crud.h"
#include "crud_modules.h"
#include "stock.h"
#include "import.h"
#include "patient_invoice.h"
#include "org.h"
#include "user.h"

static const Crud_Module *crud_find_module(String_View uri) {
    for (size_t i = 0; i < crud_modules_count; ++i) {
        if (sv_starts_with(uri, sv_from_cstr(crud_modules[i]->path))) {
            return crud_modules[i];
        }
    }
    return NULL;
}

// Extract the integer id from "/<path>/<id>/<suffix>".
static bool parse_uri_id(String_View path,
                         String_View uri,
                         const char *suffix,
                         int *id)
{
    String_View prefix = sv_from_cstr(temp_sprintf("%.*s/", (int)path.count, path.data));
    if (uri.count <= prefix.count || memcmp(uri.data, prefix.data, prefix.count) != 0) return false;
    String_View suffix_sv = sv_from_cstr(suffix);
    if (uri.count <= suffix_sv.count) return false;
    if (memcmp(uri.data + uri.count - suffix_sv.count, suffix_sv.data, suffix_sv.count) != 0) return false;

    String_View id_sv = {
        .data  = uri.data + prefix.count,
        .count = uri.count - prefix.count - suffix_sv.count,
    };
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%.*s", (int)id_sv.count, id_sv.data);
    char *end = NULL;
    long value = strtol(buf, &end, 10);
    if (end == buf || *end != '\0') return false;
    *id = (int)value;
    return true;
}

static bool crud_parse_id(const Crud_Module *mod,
                          String_View        uri,
                          const char        *suffix,
                          int               *id)
{
    return parse_uri_id(sv_from_cstr(mod->path), uri, suffix, id);
}

void serve_resource_route(Serve_Context *sc, String_View uri) {
    if (sv_eq(uri, sv_from_cstr("/css/output.css"))) {
        serve_resource(sc, "./css/output.css", "text/css; charset=utf-8");
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/favicon.ico"))) {
        serve_resource(sc, "./resource/image/user1.png", "image/png");
        return;
    }
    if (sv_starts_with(uri, sv_from_cstr("/resource/"))) {
        String_View resource_prefix = sv_from_cstr("/resource/");
        String_View rest = {
            .data  = uri.data + resource_prefix.count,
            .count = uri.count - resource_prefix.count,
        };
        String_Builder path = {0};
        sb_append_cstr(&path, "./resource/");
        sb_append_sv(&path, rest);
        sb_append_null(&path);

        const char *content_type = "application/octet-stream";
        if (sv_ends_with(rest, sv_from_cstr(".png")))  content_type = "image/png";
        if (sv_ends_with(rest, sv_from_cstr(".jpg")))  content_type = "image/jpeg";
        if (sv_ends_with(rest, sv_from_cstr(".svg")))  content_type = "image/svg+xml";
        if (sv_ends_with(rest, sv_from_cstr(".css")))  content_type = "text/css; charset=utf-8";
        if (sv_ends_with(rest, sv_from_cstr(".js")))   content_type = "text/javascript; charset=utf-8";
        if (sv_ends_with(rest, sv_from_cstr(".html"))) content_type = "text/html; charset=utf-8";
        serve_resource(sc, path.items, content_type);
        return;
    }
    serve_error(sc, 404);
}

void serve_dashboard(Serve_Context *sc);

void route_request(Serve_Context *sc, String_View method, String_View uri) {
    if (sv_eq(uri, sv_from_cstr("/")) ) {
        serve_dashboard(sc);
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/notes"))) {
        serve_notes(sc, method);
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/version"))) {
        serve_version_page(sc);
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/api/notes"))) {
        serve_notes_api(sc, method);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_eq(uri, sv_from_cstr("/notes/create"))) {
        serve_notes_create(sc);
        return;
    }
    if (sv_eq(uri, sv_from_cstr("/users"))) {
        serve_users(sc, method);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_eq(uri, sv_from_cstr("/users/create"))) {
        serve_users_create(sc);
        return;
    }
    int user_id = 0;
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/users"), uri, "/edit", &user_id)) {
        serve_users_edit(sc, uri);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/users"), uri, "/update", &user_id)) {
        serve_users_update(sc, uri);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/users"), uri, "/delete", &user_id)) {
        serve_users_delete(sc, uri);
        return;
    }
    if (sv_eq(method, sv_from_cstr("GET")) &&
        sv_eq(uri, sv_from_cstr("/medicine-stock"))) {
        serve_stock_list(sc);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_eq(uri, sv_from_cstr("/medicine-stock/create"))) {
        serve_stock_create(sc);
        return;
    }
    int stock_id = 0;
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/medicine-stock"), uri, "/edit", &stock_id)) {
        serve_stock_edit(sc, stock_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/medicine-stock"), uri, "/update", &stock_id)) {
        serve_stock_update(sc, stock_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/medicine-stock"), uri, "/delete", &stock_id)) {
        serve_stock_delete(sc, stock_id);
        return;
    }

    if (sv_eq(method, sv_from_cstr("GET")) &&
        sv_eq(uri, sv_from_cstr("/medicine-imports"))) {
        serve_import_list(sc);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_eq(uri, sv_from_cstr("/medicine-imports/create"))) {
        serve_import_create(sc);
        return;
    }
    int import_id = 0;
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/medicine-imports"), uri, "/edit", &import_id)) {
        serve_import_edit(sc, import_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/medicine-imports"), uri, "/update", &import_id)) {
        serve_import_update(sc, import_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/medicine-imports"), uri, "/delete", &import_id)) {
        serve_import_delete(sc, import_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/medicine-imports"), uri, "/details", &import_id)) {
        serve_import_details(sc, import_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/medicine-imports"), uri, "/details/create", &import_id)) {
        serve_import_detail_create(sc, import_id);
        return;
    }
    // /medicine-imports/<id>/details/<medicine_id>/delete
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_starts_with(uri, sv_from_cstr("/medicine-imports/")) &&
        sv_ends_with(uri, sv_from_cstr("/delete"))) {
        char buf[512] = {0};
        size_t n = uri.count < sizeof(buf) - 1 ? uri.count : sizeof(buf) - 1;
        memcpy(buf, uri.data, n);
        int med_id = 0;
        int iid = 0;
        if (sscanf(buf, "/medicine-imports/%d/details/%d/delete", &iid, &med_id) == 2) {
            serve_import_detail_delete(sc, iid, med_id);
            return;
        }
    }

    // ---- Patient Medicine Invoice (header + details) ----
    if (sv_eq(method, sv_from_cstr("GET")) &&
        sv_eq(uri, sv_from_cstr("/patient-medicine-invoices"))) {
        serve_pmi_list(sc);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_eq(uri, sv_from_cstr("/patient-medicine-invoices/create"))) {
        serve_pmi_create(sc);
        return;
    }
    int pmi_id = 0;
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/patient-medicine-invoices"), uri, "/edit", &pmi_id)) {
        serve_pmi_edit(sc, pmi_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/patient-medicine-invoices"), uri, "/update", &pmi_id)) {
        serve_pmi_update(sc, pmi_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/patient-medicine-invoices"), uri, "/delete", &pmi_id)) {
        serve_pmi_delete(sc, pmi_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/patient-medicine-invoices"), uri, "/details", &pmi_id)) {
        serve_pmi_details(sc, pmi_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/patient-medicine-invoices"), uri, "/details/create", &pmi_id)) {
        serve_pmi_detail_create(sc, pmi_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_starts_with(uri, sv_from_cstr("/patient-medicine-invoices/")) &&
        sv_ends_with(uri, sv_from_cstr("/delete"))) {
        char buf[512] = {0};
        size_t n = uri.count < sizeof(buf) - 1 ? uri.count : sizeof(buf) - 1;
        memcpy(buf, uri.data, n);
        int med_id = 0;
        int iid = 0;
        if (sscanf(buf, "/patient-medicine-invoices/%d/details/%d/delete", &iid, &med_id) == 2) {
            serve_pmi_detail_delete(sc, iid, med_id);
            return;
        }
    }

    // ---- Patient Invoice Out (header + details) ----
    if (sv_eq(method, sv_from_cstr("GET")) &&
        sv_eq(uri, sv_from_cstr("/patient-invoice-out"))) {
        serve_pio_list(sc);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_eq(uri, sv_from_cstr("/patient-invoice-out/create"))) {
        serve_pio_create(sc);
        return;
    }
    int pio_id = 0;
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/patient-invoice-out"), uri, "/edit", &pio_id)) {
        serve_pio_edit(sc, pio_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/patient-invoice-out"), uri, "/update", &pio_id)) {
        serve_pio_update(sc, pio_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/patient-invoice-out"), uri, "/delete", &pio_id)) {
        serve_pio_delete(sc, pio_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/patient-invoice-out"), uri, "/details", &pio_id)) {
        serve_pio_details(sc, pio_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/patient-invoice-out"), uri, "/details/create", &pio_id)) {
        serve_pio_detail_create(sc, pio_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_starts_with(uri, sv_from_cstr("/patient-invoice-out/")) &&
        sv_ends_with(uri, sv_from_cstr("/delete"))) {
        char buf[512] = {0};
        size_t n = uri.count < sizeof(buf) - 1 ? uri.count : sizeof(buf) - 1;
        memcpy(buf, uri.data, n);
        int daily_id = 0;
        int iid = 0;
        if (sscanf(buf, "/patient-invoice-out/%d/details/%d/delete", &iid, &daily_id) == 2) {
            serve_pio_detail_delete(sc, iid, daily_id);
            return;
        }
    }

    // ---- Organization Daily Invoice (header + details) ----
    if (sv_eq(method, sv_from_cstr("GET")) &&
        sv_eq(uri, sv_from_cstr("/organization-daily-invoices"))) {
        serve_org_daily_list(sc);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_eq(uri, sv_from_cstr("/organization-daily-invoices/create"))) {
        serve_org_daily_create(sc);
        return;
    }
    int org_daily_id = 0;
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/organization-daily-invoices"), uri, "/edit", &org_daily_id)) {
        serve_org_daily_edit(sc, org_daily_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/organization-daily-invoices"), uri, "/update", &org_daily_id)) {
        serve_org_daily_update(sc, org_daily_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/organization-daily-invoices"), uri, "/delete", &org_daily_id)) {
        serve_org_daily_delete(sc, org_daily_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/organization-daily-invoices"), uri, "/details", &org_daily_id)) {
        serve_org_daily_details(sc, org_daily_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/organization-daily-invoices"), uri, "/details/create", &org_daily_id)) {
        serve_org_daily_detail_create(sc, org_daily_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_starts_with(uri, sv_from_cstr("/organization-daily-invoices/")) &&
        sv_ends_with(uri, sv_from_cstr("/delete"))) {
        char buf[512] = {0};
        size_t n = uri.count < sizeof(buf) - 1 ? uri.count : sizeof(buf) - 1;
        memcpy(buf, uri.data, n);
        int ill_id = 0;
        int iid = 0;
        if (sscanf(buf, "/organization-daily-invoices/%d/details/%d/delete", &iid, &ill_id) == 2) {
            serve_org_daily_detail_delete(sc, iid, ill_id);
            return;
        }
    }

    // ---- Organization Balance (composite PK) ----
    if (sv_eq(method, sv_from_cstr("GET")) &&
        sv_eq(uri, sv_from_cstr("/organization-balance"))) {
        serve_org_balance_list(sc);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_eq(uri, sv_from_cstr("/organization-balance/create"))) {
        serve_org_balance_create(sc);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_starts_with(uri, sv_from_cstr("/organization-balance/")) &&
        sv_ends_with(uri, sv_from_cstr("/delete"))) {
        char buf[512] = {0};
        size_t n = uri.count < sizeof(buf) - 1 ? uri.count : sizeof(buf) - 1;
        memcpy(buf, uri.data, n);
        int org_id = 0;
        int inv_id = 0;
        if (sscanf(buf, "/organization-balance/%d/%d/delete", &org_id, &inv_id) == 2) {
            serve_org_balance_delete(sc, org_id, inv_id);
            return;
        }
    }

    // ---- Organization Invoice Out (header + details) ----
    if (sv_eq(method, sv_from_cstr("GET")) &&
        sv_eq(uri, sv_from_cstr("/organization-invoice-out"))) {
        serve_org_out_list(sc);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_eq(uri, sv_from_cstr("/organization-invoice-out/create"))) {
        serve_org_out_create(sc);
        return;
    }
    int org_out_id = 0;
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/organization-invoice-out"), uri, "/edit", &org_out_id)) {
        serve_org_out_edit(sc, org_out_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/organization-invoice-out"), uri, "/update", &org_out_id)) {
        serve_org_out_update(sc, org_out_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/organization-invoice-out"), uri, "/delete", &org_out_id)) {
        serve_org_out_delete(sc, org_out_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("GET")) &&
        parse_uri_id(sv_from_cstr("/organization-invoice-out"), uri, "/details", &org_out_id)) {
        serve_org_out_details(sc, org_out_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        parse_uri_id(sv_from_cstr("/organization-invoice-out"), uri, "/details/create", &org_out_id)) {
        serve_org_out_detail_create(sc, org_out_id);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST")) &&
        sv_starts_with(uri, sv_from_cstr("/organization-invoice-out/")) &&
        sv_ends_with(uri, sv_from_cstr("/delete"))) {
        char buf[512] = {0};
        size_t n = uri.count < sizeof(buf) - 1 ? uri.count : sizeof(buf) - 1;
        memcpy(buf, uri.data, n);
        int inv_id = 0;
        int iid = 0;
        if (sscanf(buf, "/organization-invoice-out/%d/details/%d/delete", &iid, &inv_id) == 2) {
            serve_org_out_detail_delete(sc, iid, inv_id);
            return;
        }
    }
    if (sv_eq(method, sv_from_cstr("GET"))       &&
        sv_ends_with(uri, sv_from_cstr("/edit")) &&
        sv_starts_with(uri, sv_from_cstr("/notes/"))) {
        serve_notes_edit(sc, uri);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST"))        &&
        sv_ends_with(uri, sv_from_cstr("/update")) &&
        sv_starts_with(uri, sv_from_cstr("/notes/"))) {
        serve_notes_update(sc, uri);
        return;
    }
    if (sv_eq(method, sv_from_cstr("POST"))        &&
        sv_ends_with(uri, sv_from_cstr("/delete")) &&
        sv_starts_with(uri, sv_from_cstr("/notes/"))) {
        serve_notes_delete(sc, uri);
        return;
    }
    const Crud_Module *crud_mod = crud_find_module(uri);
    if (crud_mod) {
        String_View path = sv_from_cstr(crud_mod->path);
        if (sv_eq(uri, path)) {
            if (sv_eq(method, sv_from_cstr("GET"))) {
                serve_crud_list(sc, crud_mod);
                return;
            }
            serve_error(sc, 405);
            return;
        }

        int id = 0;
        if (sv_eq(method, sv_from_cstr("POST")) &&
            sv_ends_with(uri, sv_from_cstr("/create")) &&
            uri.count == path.count + strlen("/create")) {
            serve_crud_create(sc, crud_mod);
            return;
        }
        if (sv_eq(method, sv_from_cstr("GET")) &&
            crud_parse_id(crud_mod, uri, "/edit", &id)) {
            serve_crud_edit(sc, crud_mod, id);
            return;
        }
        if (sv_eq(method, sv_from_cstr("POST")) &&
            crud_parse_id(crud_mod, uri, "/update", &id)) {
            serve_crud_update(sc, crud_mod, id);
            return;
        }
        if (sv_eq(method, sv_from_cstr("POST")) &&
            crud_parse_id(crud_mod, uri, "/delete", &id)) {
            serve_crud_delete(sc, crud_mod, id);
            return;
        }
    }

    if (sv_starts_with(uri, sv_from_cstr("/css/"))      ||
        sv_starts_with(uri, sv_from_cstr("/resource/")) ||
        sv_eq(uri, sv_from_cstr("/favicon.ico"))) {
        serve_resource_route(sc, uri);
        return;
    }

    serve_error(sc, 404);
}
