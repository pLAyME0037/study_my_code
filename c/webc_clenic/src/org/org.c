#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "org.h"

// ---------------------------------------------------------------------------
// Organization Daily Invoice
// ---------------------------------------------------------------------------

bool org_daily_headers_load(sqlite3 *db, Org_Daily_Headers *rows, long long only_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = only_id < 0
        ? "SELECT i.id, o.name, p.name, i.invoice_date, i.amount_in_riel, i.amount_in_dollar, "
          "(SELECT COUNT(*) FROM OrganizationDailyInvoiceDetails d WHERE d.organization_daily_id = i.id) "
          "FROM OrganizationDailyInvoices i "
          "LEFT JOIN Organizations o ON o.id = i.organization_id "
          "LEFT JOIN Patients p ON p.id = i.patient_id "
          "ORDER BY i.id DESC;"
        : temp_sprintf("SELECT i.id, o.name, p.name, i.invoice_date, i.amount_in_riel, i.amount_in_dollar, "
                       "(SELECT COUNT(*) FROM OrganizationDailyInvoiceDetails d WHERE d.organization_daily_id = i.id) "
                       "FROM OrganizationDailyInvoices i "
                       "LEFT JOIN Organizations o ON o.id = i.organization_id "
                       "LEFT JOIN Patients p ON p.id = i.patient_id "
                       "WHERE i.id = %lld;", only_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Org_Daily_Header row = {0};
        row.id = sqlite3_column_int64(stmt, 0);
        const char *o   = (const char *)sqlite3_column_text(stmt, 1);
        const char *p   = (const char *)sqlite3_column_text(stmt, 2);
        const char *d   = (const char *)sqlite3_column_text(stmt, 3);
        const char *r   = (const char *)sqlite3_column_text(stmt, 4);
        const char *usd = (const char *)sqlite3_column_text(stmt, 5);
        row.org_name         = temp_strdup(o ? o : "");
        row.patient_name     = temp_strdup(p ? p : "");
        row.invoice_date     = temp_strdup(d ? d : "");
        row.amount_in_riel   = temp_strdup(r ? r : "");
        row.amount_in_dollar = temp_strdup(usd ? usd : "");
        row.detail_count     = sqlite3_column_int(stmt, 6);
        da_append(rows, row);
    }
    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_daily_details_load(sqlite3 *db, Org_Daily_Details *rows, long long daily_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = temp_sprintf(
        "SELECT d.ill_id, it.type, d.price, d.currency "
        "FROM OrganizationDailyInvoiceDetails d "
        "LEFT JOIN IllTypes it ON it.id = d.ill_id "
        "WHERE d.organization_daily_id = %lld ORDER BY it.type ASC;", daily_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Org_Daily_Detail row = {0};
        row.ill_id = sqlite3_column_int64(stmt, 0);
        const char *n = (const char *)sqlite3_column_text(stmt, 1);
        const char *p = (const char *)sqlite3_column_text(stmt, 2);
        const char *c = (const char *)sqlite3_column_text(stmt, 3);
        row.ill_name = temp_strdup(n ? n : "");
        row.price    = temp_strdup(p ? p : "");
        row.currency = temp_strdup(c ? c : "");
        da_append(rows, row);
    }
    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_daily_insert(sqlite3    *db,
                      long long  *out_id,
                      long long   organization_id,
                      long long   patient_id,
                      const char *invoice_date,
                      const char *riel,
                      const char *dollar)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT INTO OrganizationDailyInvoices (organization_id, patient_id, invoice_date, amount_in_riel, amount_in_dollar) "
        "VALUES (?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, organization_id);
    if (patient_id > 0) sqlite3_bind_int64(stmt, 2, patient_id);
    else sqlite3_bind_null(stmt, 2);
    if (invoice_date && invoice_date[0]) sqlite3_bind_text(stmt, 3, invoice_date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 3);
    if (riel && riel[0]) sqlite3_bind_text(stmt, 4, riel, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 4);
    if (dollar && dollar[0]) sqlite3_bind_text(stmt, 5, dollar, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 5);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    *out_id = (long long)sqlite3_last_insert_rowid(db);

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_daily_update(sqlite3    *db,
                      long long   id,
                      const char *invoice_date,
                      const char *riel,
                      const char *dollar)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "UPDATE OrganizationDailyInvoices SET "
        "invoice_date = ?,"
        "amount_in_riel = ?,"
        "amount_in_dollar = ?"
        "WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (invoice_date && invoice_date[0]) sqlite3_bind_text(stmt, 1, invoice_date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 1);
    if (riel && riel[0]) sqlite3_bind_text(stmt, 2, riel, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 2);
    if (dollar && dollar[0]) sqlite3_bind_text(stmt, 3, dollar, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 3);
    sqlite3_bind_int64(stmt, 4, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_daily_delete(sqlite3 *db, long long id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "DELETE FROM OrganizationDailyInvoiceDetails WHERE organization_daily_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_finalize(stmt);
    stmt = NULL;

    const char *sql2 = "DELETE FROM OrganizationDailyInvoices WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_daily_detail_insert(sqlite3    *db,
                             long long   daily_id,
                             long long   ill_id,
                             const char *price,
                             const char *currency)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT OR REPLACE INTO OrganizationDailyInvoiceDetails (organization_daily_id, ill_id, price, currency) "
        "VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, daily_id);
    sqlite3_bind_int64(stmt, 2, ill_id);
    sqlite3_bind_text(stmt, 3, price, -1, SQLITE_TRANSIENT);
    if (currency && currency[0]) {
        sqlite3_bind_text(stmt, 4, currency, -1, SQLITE_TRANSIENT);
    }
    else sqlite3_bind_null(stmt, 4);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_daily_detail_delete(sqlite3 *db, long long daily_id, long long ill_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "DELETE FROM OrganizationDailyInvoiceDetails WHERE organization_daily_id = ? AND ill_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, daily_id);
    sqlite3_bind_int64(stmt, 2, ill_id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

// ---------------------------------------------------------------------------
// Organization Balance
// ---------------------------------------------------------------------------

bool org_balances_load(sqlite3 *db, Org_Balance_Rows *rows) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "SELECT b.organization_id, b.organization_invoice_id, o.name, "
        "i.invoice_date, b.balance_amount, b.balance "
        "FROM OrganizationBalance b "
        "LEFT JOIN Organizations o ON o.id = b.organization_id "
        "LEFT JOIN OrganizationInvoices i ON i.id = b.organization_invoice_id "
        "ORDER BY o.name ASC, i.invoice_date DESC;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Org_Balance_Row row = {0};
        row.org_id = sqlite3_column_int64(stmt, 0);
        row.invoice_id = sqlite3_column_int64(stmt, 1);
        const char *o  = (const char *)sqlite3_column_text(stmt, 2);
        const char *il = (const char *)sqlite3_column_text(stmt, 3);
        const char *ba = (const char *)sqlite3_column_text(stmt, 4);
        const char *b  = (const char *)sqlite3_column_text(stmt, 5);
        row.org_name       = temp_strdup(o  ? o  : "");
        row.invoice_label  = temp_strdup(il ? il : "");
        row.balance_amount = temp_strdup(ba ? ba : "");
        row.balance        = temp_strdup(b  ? b  : "");
        da_append(rows, row);
    }
    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_balance_insert(sqlite3    *db,
                        long long   organization_id,
                        long long   invoice_id,
                        const char *balance_amount,
                        const char *balance)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT OR REPLACE INTO OrganizationBalance "
        "(organization_id, organization_invoice_id, balance_amount, balance) "
        "VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, organization_id);
    sqlite3_bind_int64(stmt, 2, invoice_id);
    if (balance_amount && balance_amount[0]) {
        sqlite3_bind_text(stmt, 3, balance_amount, -1, SQLITE_TRANSIENT);
    }
    else sqlite3_bind_null(stmt, 3);
    if (balance && balance[0]) {
        sqlite3_bind_text(stmt, 4, balance, -1, SQLITE_TRANSIENT);
    }
    else sqlite3_bind_null(stmt, 4);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_balance_delete(sqlite3 *db, long long organization_id, long long invoice_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "DELETE FROM OrganizationBalance WHERE organization_id = ? AND organization_invoice_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, organization_id);
    sqlite3_bind_int64(stmt, 2, invoice_id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

// ---------------------------------------------------------------------------
// Organization Invoice Out
// ---------------------------------------------------------------------------

bool org_out_headers_load(sqlite3 *db, Org_Out_Headers *rows, long long only_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = only_id < 0
        ? "SELECT i.id, o.name, p.name, rt.type, i.room_price, i.start_date, i.end_date, i.room_day, "
          "(SELECT COUNT(*) FROM OrganizationInvoiceOutDetails d WHERE d.organization_invoice_out_id = i.id) "
          "FROM OrganizationInvoiceOut i "
          "LEFT JOIN Organizations o ON o.id = i.organization_id "
          "LEFT JOIN Patients p ON p.id = i.patient_id "
          "LEFT JOIN Rooms r ON r.id = i.room_id "
          "LEFT JOIN RoomTypes rt ON rt.id = r.room_type_id "
          "ORDER BY i.id DESC;"
        : temp_sprintf("SELECT i.id, o.name, p.name, rt.type, i.room_price, i.start_date, i.end_date, i.room_day, "
                       "(SELECT COUNT(*) FROM OrganizationInvoiceOutDetails d WHERE d.organization_invoice_out_id = i.id) "
                       "FROM OrganizationInvoiceOut i "
                       "LEFT JOIN Organizations o ON o.id = i.organization_id "
                       "LEFT JOIN Patients p ON p.id = i.patient_id "
                       "LEFT JOIN Rooms r ON r.id = i.room_id "
                       "LEFT JOIN RoomTypes rt ON rt.id = r.room_type_id "
                       "WHERE i.id = %lld;", only_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Org_Out_Header row = {0};
        row.id = sqlite3_column_int64(stmt, 0);
        const char *o  = (const char *)sqlite3_column_text(stmt, 1);
        const char *p  = (const char *)sqlite3_column_text(stmt, 2);
        const char *rt = (const char *)sqlite3_column_text(stmt, 3);
        const char *rp = (const char *)sqlite3_column_text(stmt, 4);
        const char *sd = (const char *)sqlite3_column_text(stmt, 5);
        const char *ed = (const char *)sqlite3_column_text(stmt, 6);
        const char *rd = (const char *)sqlite3_column_text(stmt, 7);
        row.org_name     = temp_strdup(o  ? o  : "");
        row.patient_name = temp_strdup(p  ? p  : "");
        row.room_name    = temp_strdup(rt ? rt : "");
        row.room_price   = temp_strdup(rp ? rp : "");
        row.start_date   = temp_strdup(sd ? sd : "");
        row.end_date     = temp_strdup(ed ? ed : "");
        row.room_day     = temp_strdup(rd ? rd : "");
        row.detail_count = sqlite3_column_int(stmt, 8);
        da_append(rows, row);
    }
    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_out_details_load(sqlite3 *db, Org_Out_Details *rows, long long out_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = temp_sprintf(
        "SELECT d.organization_invoice_id, i.invoice_date "
        "FROM OrganizationInvoiceOutDetails d "
        "LEFT JOIN OrganizationInvoices i ON i.id = d.organization_invoice_id "
        "WHERE d.organization_invoice_out_id = %lld ORDER BY d.organization_invoice_id DESC;", out_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Org_Out_Detail row = {0};
        row.invoice_id = sqlite3_column_int64(stmt, 0);
        const char *l = (const char *)sqlite3_column_text(stmt, 1);
        row.invoice_label = l ? temp_strdup(l) : NULL;
        da_append(rows, row);
    }
    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_out_insert(sqlite3    *db,
                    long long  *out_id,
                    long long   organization_id,
                    long long   patient_id,
                    long long   room_id,
                    const char *room_price,
                    const char *start_date,
                    const char *end_date,
                    const char *room_day)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT INTO OrganizationInvoiceOut (organization_id, patient_id, room_id, room_price, start_date, end_date, room_day) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, organization_id);
    sqlite3_bind_int64(stmt, 2, patient_id);
    if (room_id > 0) sqlite3_bind_int64(stmt, 3, room_id);
    else sqlite3_bind_null(stmt, 3);
    if (room_price && room_price[0]) sqlite3_bind_text(stmt, 4, room_price, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 4);
    if (start_date && start_date[0]) sqlite3_bind_text(stmt, 5, start_date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 5);
    if (end_date && end_date[0]) sqlite3_bind_text(stmt, 6, end_date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 6);
    if (room_day && room_day[0]) sqlite3_bind_text(stmt, 7, room_day, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 7);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    *out_id = (long long)sqlite3_last_insert_rowid(db);

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_out_update(sqlite3    *db,
                    long long   id,
                    const char *room_price,
                    const char *start_date,
                    const char *end_date,
                    const char *room_day)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "UPDATE OrganizationInvoiceOut SET room_price = ?, start_date = ?, end_date = ?, room_day = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (room_price && room_price[0]) sqlite3_bind_text(stmt, 1, room_price, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 1);
    if (start_date && start_date[0]) sqlite3_bind_text(stmt, 2, start_date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 2);
    if (end_date && end_date[0]) sqlite3_bind_text(stmt, 3, end_date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 3);
    if (room_day && room_day[0]) sqlite3_bind_text(stmt, 4, room_day, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 4);
    sqlite3_bind_int64(stmt, 5, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_out_delete(sqlite3 *db, long long id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "DELETE FROM OrganizationInvoiceOutDetails WHERE organization_invoice_out_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_finalize(stmt);
    stmt = NULL;

    const char *sql2 = "DELETE FROM OrganizationInvoiceOut WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_out_detail_insert(sqlite3 *db, long long out_id, long long invoice_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT OR REPLACE INTO OrganizationInvoiceOutDetails (organization_invoice_out_id, organization_invoice_id) "
        "VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, out_id);
    sqlite3_bind_int64(stmt, 2, invoice_id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool org_out_detail_delete(sqlite3 *db, long long out_id, long long invoice_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "DELETE FROM OrganizationInvoiceOutDetails WHERE organization_invoice_out_id = ? AND organization_invoice_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, out_id);
    sqlite3_bind_int64(stmt, 2, invoice_id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}
