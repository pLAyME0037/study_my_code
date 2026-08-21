#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "patient_invoice.h"

// ---------------------------------------------------------------------------
// Generic FK option lists
// ---------------------------------------------------------------------------

bool pio_options_load(sqlite3     *db,
                      Pio_Options *opts,
                      const char  *table,
                      const char  *label_col)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = temp_sprintf(
        "SELECT id, %s FROM %s ORDER BY %s ASC;", label_col, table, label_col);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Pio_Option opt = {0};
        opt.id = sqlite3_column_int64(stmt, 0);
        const char *label = (const char *)sqlite3_column_text(stmt, 1);
        opt.label = label ? temp_strdup(label) : NULL;
        da_append(opts, opt);
    }
    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool pio_room_options_load(sqlite3 *db, Pio_Options *opts) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "SELECT r.id, rt.type || ' ($' || r.price || ')' FROM Rooms r "
        "LEFT JOIN RoomTypes rt ON rt.id = r.room_type_id ORDER BY rt.type ASC;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Pio_Option opt = {0};
        opt.id = sqlite3_column_int64(stmt, 0);
        const char *label = (const char *)sqlite3_column_text(stmt, 1);
        opt.label = label ? temp_strdup(label) : NULL;
        da_append(opts, opt);
    }
    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

// ---------------------------------------------------------------------------
// Patient Medicine Invoice
// ---------------------------------------------------------------------------

bool pmi_headers_load(sqlite3 *db, Pmi_Headers *rows, long long only_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = only_id < 0
        ? "SELECT i.id, p.name, i.invoice_date, i.amount_in_riel, i.amount_in_dollar, "
          "(SELECT COUNT(*) FROM PatientMedicineInvoiceDetails d WHERE d.patient_medicine_invoice_id = i.id) "
          "FROM PatientMedicineInvoices i LEFT JOIN Patients p ON p.id = i.patient_id "
          "ORDER BY i.id DESC;"
        : temp_sprintf("SELECT i.id, p.name, i.invoice_date, i.amount_in_riel, i.amount_in_dollar, "
                       "(SELECT COUNT(*) FROM PatientMedicineInvoiceDetails d WHERE d.patient_medicine_invoice_id = i.id) "
                       "FROM PatientMedicineInvoices i LEFT JOIN Patients p ON p.id = i.patient_id "
                       "WHERE i.id = %lld;", only_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Pmi_Header row = {0};
        row.id = sqlite3_column_int64(stmt, 0);
        const char *p   = (const char *)sqlite3_column_text(stmt, 1);
        const char *d   = (const char *)sqlite3_column_text(stmt, 2);
        const char *r   = (const char *)sqlite3_column_text(stmt, 3);
        const char *usd = (const char *)sqlite3_column_text(stmt, 4);
        row.patient_name     = temp_strdup(p   ? p   : "");
        row.invoice_date     = temp_strdup(d   ? d   : "");
        row.amount_in_riel   = temp_strdup(r   ? r   : "");
        row.amount_in_dollar = temp_strdup(usd ? usd : "");
        row.detail_count     = sqlite3_column_int(stmt, 5);
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

bool pmi_details_load(sqlite3 *db, Pmi_Details *rows, long long invoice_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = temp_sprintf(
        "SELECT d.medicine_id, m.name, d.qty, d.price, d.amount, d.currency "
        "FROM PatientMedicineInvoiceDetails d JOIN Medicines m ON m.id = d.medicine_id "
        "WHERE d.patient_medicine_invoice_id = %lld ORDER BY m.name ASC;", invoice_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Pmi_Detail row = {0};
        row.medicine_id = sqlite3_column_int64(stmt, 0);
        const char *n = (const char *)sqlite3_column_text(stmt, 1);
        const char *q = (const char *)sqlite3_column_text(stmt, 2);
        const char *p = (const char *)sqlite3_column_text(stmt, 3);
        const char *a = (const char *)sqlite3_column_text(stmt, 4);
        const char *c = (const char *)sqlite3_column_text(stmt, 5);
        row.medicine_name = temp_strdup(n ? n : "");
        row.qty           = temp_strdup(q ? q : "");
        row.price         = temp_strdup(p ? p : "");
        row.amount        = temp_strdup(a ? a : "");
        row.currency      = temp_strdup(c ? c : "");
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

bool pmi_insert(sqlite3    *db,
                long long  *out_id,
                long long   patient_id,
                const char *invoice_date,
                const char *riel,
                const char *dollar)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "INSERT INTO PatientMedicineInvoices "
        "(patient_id, invoice_date, amount_in_riel, amount_in_dollar) "
        "VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, patient_id);
    if (invoice_date && invoice_date[0]) sqlite3_bind_text(stmt, 2, invoice_date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 2);
    if (riel && riel[0]) sqlite3_bind_text(stmt, 3, riel, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 3);
    if (dollar && dollar[0]) sqlite3_bind_text(stmt, 4, dollar, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 4);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    *out_id = (long long)sqlite3_last_insert_rowid(db);

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool pmi_update(sqlite3    *db,
                long long   id,
                const char *invoice_date,
                const char *riel,
                const char *dollar)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "UPDATE PatientMedicineInvoices SET invoice_date = ?, "
        "amount_in_riel = ?, "
        "amount_in_dollar = ? "
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

bool pmi_delete(sqlite3 *db, long long id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "DELETE FROM PatientMedicineInvoiceDetails "
        "WHERE patient_medicine_invoice_id = ?;";
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

    const char *sql2 = "DELETE FROM PatientMedicineInvoices WHERE id = ?;";
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

bool pmi_detail_insert(sqlite3    *db,
                       long long   invoice_id,
                       long long   medicine_id,
                       const char *qty,
                       const char *price,
                       const char *amount,
                       const char *currency)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "INSERT OR REPLACE INTO PatientMedicineInvoiceDetails "
        "(patient_medicine_invoice_id, medicine_id, qty, price, amount, currency) "
        "VALUES (?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, invoice_id);
    sqlite3_bind_int64(stmt, 2, medicine_id);
    sqlite3_bind_text(stmt, 3, qty, -1, SQLITE_TRANSIENT);
    if (price && price[0]) sqlite3_bind_text(stmt, 4, price, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 4);
    if (amount && amount[0]) sqlite3_bind_text(stmt, 5, amount, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 5);
    if (currency && currency[0]) sqlite3_bind_text(stmt, 6, currency, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 6);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool pmi_detail_delete(sqlite3 *db, long long invoice_id, long long medicine_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "DELETE FROM PatientMedicineInvoiceDetails "
        "WHERE patient_medicine_invoice_id = ? "
        "AND medicine_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, invoice_id);
    sqlite3_bind_int64(stmt, 2, medicine_id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

// ---------------------------------------------------------------------------
// Patient Invoice Out
// ---------------------------------------------------------------------------

bool pio_headers_load(sqlite3 *db, Pio_Headers *rows, long long only_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = only_id < 0
        ? "SELECT i.id, p.name, rt.type, i.room_price, i.start_date, i.end_date, i.room_day, "
          "(SELECT COUNT(*) FROM PatientInvoiceOutDetails d WHERE d.patient_invoice_out_id = i.id) "
          "FROM PatientInvoiceOut i "
          "LEFT JOIN Patients p ON p.id = i.patient_id "
          "LEFT JOIN Rooms r ON r.id = i.room_id "
          "LEFT JOIN RoomTypes rt ON rt.id = r.room_type_id "
          "ORDER BY i.id DESC;"
        : temp_sprintf("SELECT i.id, p.name, rt.type, i.room_price, i.start_date, i.end_date, i.room_day, "
                       "(SELECT COUNT(*) FROM PatientInvoiceOutDetails d WHERE d.patient_invoice_out_id = i.id) "
                       "FROM PatientInvoiceOut i "
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
        Pio_Header row = {0};
        row.id = sqlite3_column_int64(stmt, 0);
        const char *p  = (const char *)sqlite3_column_text(stmt, 1);
        const char *r  = (const char *)sqlite3_column_text(stmt, 2);
        const char *rp = (const char *)sqlite3_column_text(stmt, 3);
        const char *sd = (const char *)sqlite3_column_text(stmt, 4);
        const char *ed = (const char *)sqlite3_column_text(stmt, 5);
        const char *rd = (const char *)sqlite3_column_text(stmt, 6);
        row.patient_name = temp_strdup(p  ? p  : "");
        row.room_name    = temp_strdup(r  ? r  : "");
        row.room_price   = temp_strdup(rp ? rp : "");
        row.start_date   = temp_strdup(sd ? sd : "");
        row.end_date     = temp_strdup(ed ? ed : "");
        row.room_day     = temp_strdup(rd ? rd : "");
        row.detail_count = sqlite3_column_int(stmt, 7);
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

bool pio_details_load(sqlite3 *db, Pio_Details *rows, long long out_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = temp_sprintf(
        "SELECT d.patient_daily_invoice_id, di.invoice_date "
        "FROM PatientInvoiceOutDetails d "
        "LEFT JOIN PatientDailyInvoices di ON di.id = d.patient_daily_invoice_id "
        "WHERE d.patient_invoice_out_id = %lld ORDER BY d.patient_daily_invoice_id DESC;", out_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Pio_Detail row = {0};
        row.daily_id = sqlite3_column_int64(stmt, 0);
        const char *d = (const char *)sqlite3_column_text(stmt, 1);
        row.daily_label = d ? temp_strdup(d) : NULL;
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

bool pio_insert(sqlite3   *db,
                long long *out_id,
                long long   patient_id,
                long long   room_id,
                const char *room_price,
                const char *start_date,
                const char *end_date,
                const char *room_day)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "INSERT INTO PatientInvoiceOut ("
        "patient_id, "
        "room_id, "
        "room_price, "
        "start_date, "
        "end_date, "
        "room_day) "
        "VALUES (?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, patient_id);
    if (room_id > 0) sqlite3_bind_int64(stmt, 2, room_id);
    else sqlite3_bind_null(stmt, 2);
    if (room_price && room_price[0]) sqlite3_bind_text(stmt, 3, room_price, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 3);
    if (start_date && start_date[0]) sqlite3_bind_text(stmt, 4, start_date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 4);
    if (end_date && end_date[0]) sqlite3_bind_text(stmt, 5, end_date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 5);
    if (room_day && room_day[0]) sqlite3_bind_text(stmt, 6, room_day, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 6);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    *out_id = (long long)sqlite3_last_insert_rowid(db);

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool pio_update(sqlite3    *db,
                long long   id,
                const char *room_price,
                const char *start_date,
                const char *end_date,
                const char *room_day)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "UPDATE PatientInvoiceOut SET room_price = ?, "
        "start_date = ?, "
        "end_date = ?, "
        "room_day = ? "
        "WHERE id = ?;";
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

bool pio_delete(sqlite3 *db, long long id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "DELETE FROM PatientInvoiceOutDetails "
        "WHERE patient_invoice_out_id = ?;";
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

    const char *sql2 = "DELETE FROM PatientInvoiceOut WHERE id = ?;";
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

bool pio_detail_insert(sqlite3 *db, long long out_id, long long daily_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "INSERT OR REPLACE INTO PatientInvoiceOutDetails ("
        "patient_invoice_out_id, "
        "patient_daily_invoice_id) "
        "VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, out_id);
    sqlite3_bind_int64(stmt, 2, daily_id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool pio_detail_delete(sqlite3 *db, long long out_id, long long daily_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "DELETE FROM PatientInvoiceOutDetails "
        "WHERE patient_invoice_out_id = ? "
        "AND patient_daily_invoice_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, out_id);
    sqlite3_bind_int64(stmt, 2, daily_id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}
