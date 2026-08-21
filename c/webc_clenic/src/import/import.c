#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "import.h"

bool import_rows_load(sqlite3 *db, Import_Rows *rows, long long only_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "";
    if (only_id < 0) {
        sql = "SELECT mi.id, mi.date, d.medicine_id, m.name, d.qty, d.price, "
            "(d.qty * d.price) AS amount "
            "FROM MedicineImport mi "
            "JOIN MedicineImportDetails d ON d.medicine_import_id = mi.id "
            "JOIN Medicines m ON m.id = d.medicine_id "
            "ORDER BY mi.id DESC;";
    } else {
        sql = temp_sprintf("SELECT mi.id, mi.date, d.medicine_id, m.name, d.qty, d.price, "
                "(d.qty * d.price) AS amount "
                "FROM MedicineImport mi "
                "JOIN MedicineImportDetails d ON d.medicine_import_id = mi.id "
                "JOIN Medicines m ON m.id = d.medicine_id "
                "WHERE mi.id = %lld;", only_id);
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Import_Row row = {0};
        row.id = sqlite3_column_int64(stmt, 0);
        const char *date        = (const char *)sqlite3_column_text(stmt, 1);
        const char *medicine_id = (const char *)sqlite3_column_text(stmt, 2);
        const char *name        = (const char *)sqlite3_column_text(stmt, 3);
        const char *qty         = (const char *)sqlite3_column_text(stmt, 4);
        const char *price       = (const char *)sqlite3_column_text(stmt, 5);
        const char *amount      = (const char *)sqlite3_column_text(stmt, 6);
        row.date          = temp_strdup(date   ? date   : "");
        row.medicine_id   = temp_strdup(medicine_id ? medicine_id : "");
        row.medicine_name = temp_strdup(name   ? name   : "");
        row.qty           = temp_strdup(qty    ? qty    : "");
        row.price         = temp_strdup(price  ? price  : "");
        row.amount        = temp_strdup(amount ? amount : "");
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

bool import_medicines_load(sqlite3 *db, Medicine_Options *opts) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "SELECT id, name FROM Medicines ORDER BY name ASC;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Medicine_Option opt = {0};
        opt.id = sqlite3_column_int64(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        opt.name = name ? temp_strdup(name) : NULL;
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

bool import_insert_line(sqlite3    *db,
                        const char *date,
                        long long   medicine_id,
                        const char *qty,
                        const char *price)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "INSERT INTO MedicineImport (date) VALUES (?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (date && date[0]) sqlite3_bind_text(stmt, 1, date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 1);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    long long import_id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    stmt = NULL;

    const char *sql2 =
        "INSERT INTO MedicineImportDetails (medicine_import_id, medicine_id, qty, price) "
        "VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, import_id);
    sqlite3_bind_int64(stmt, 2, medicine_id);
    sqlite3_bind_text(stmt,  3, qty, -1, SQLITE_TRANSIENT);
    if (price && price[0]) sqlite3_bind_text(stmt, 4, price, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 4);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool import_update_line(sqlite3    *db,
                        long long   id,
                        const char *date,
                        long long   medicine_id,
                        const char *qty,
                        const char *price)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "UPDATE MedicineImport SET date = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (date && date[0]) sqlite3_bind_text(stmt, 1, date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 1);
    sqlite3_bind_int64(stmt, 2, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_finalize(stmt);
    stmt = NULL;

    const char *sql2 =
        "UPDATE MedicineImportDetails SET medicine_id = ?, qty = ?, price = ? "
        "WHERE medicine_import_id = ?;";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, medicine_id);
    sqlite3_bind_text(stmt, 2, qty, -1, SQLITE_TRANSIENT);
    if (price && price[0]) sqlite3_bind_text(stmt, 3, price, -1, SQLITE_TRANSIENT);
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

bool import_delete(sqlite3 *db, long long id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "DELETE FROM MedicineImportDetails WHERE medicine_import_id = ?;";
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

    const char *sql2 = "DELETE FROM MedicineImport WHERE id = ?;";
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
