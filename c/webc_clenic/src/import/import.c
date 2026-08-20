#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "import.h"

bool import_rows_load(sqlite3 *db, Import_Headers *rows, long long only_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "";
    if (only_id < 0) {
        sql = "SELECT mi.id, mi.date, mi.amount, "
            "(SELECT COUNT(*) FROM MedicineImportDetails d "
            "WHERE d.medicine_import_id = mi.id) "
            "FROM MedicineImport mi ORDER BY mi.id DESC;";
    } else {
        sql = temp_sprintf("SELECT mi.id, mi.date, mi.amount, "
                "(SELECT COUNT(*) FROM MedicineImportDetails d "
                "WHERE d.medicine_import_id = mi.id) "
                "FROM MedicineImport mi WHERE mi.id = %lld;", only_id);
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Import_Header row = {0};
        row.id = sqlite3_column_int64(stmt, 0);
        const char *date = (const char *)sqlite3_column_text(stmt, 1);
        const char *amount = (const char *)sqlite3_column_text(stmt, 2);
        row.date = temp_strdup(date ? date : "");
        row.amount = temp_strdup(amount ? amount : "");
        row.detail_count = sqlite3_column_int(stmt, 3);
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

bool import_details_load(sqlite3 *db, Import_Details *rows, long long import_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = temp_sprintf(
        "SELECT d.medicine_id, m.name, d.qty, d.price "
        "FROM MedicineImportDetails d JOIN Medicines m ON m.id = d.medicine_id "
        "WHERE d.medicine_import_id = %lld ORDER BY m.name ASC;", import_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        Import_Detail row = {0};
        row.medicine_id = sqlite3_column_int64(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        const char *qty = (const char *)sqlite3_column_text(stmt, 2);
        const char *price = (const char *)sqlite3_column_text(stmt, 3);
        row.medicine_name = temp_strdup(name ? name : "");
        row.qty = temp_strdup(qty ? qty : "");
        row.price = temp_strdup(price ? price : "");
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

bool import_insert(sqlite3 *db, const char *date, const char *amount) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "INSERT INTO MedicineImport (date, amount) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (date && date[0]) sqlite3_bind_text(stmt, 1, date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 1);
    if (amount && amount[0]) sqlite3_bind_text(stmt, 2, amount, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 2);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}

bool import_update(sqlite3 *db, long long id, const char *date, const char *amount) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql = "UPDATE MedicineImport SET date = ?, amount = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (date && date[0]) sqlite3_bind_text(stmt, 1, date, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 1);
    if (amount && amount[0]) sqlite3_bind_text(stmt, 2, amount, -1, SQLITE_TRANSIENT);
    else sqlite3_bind_null(stmt, 2);
    sqlite3_bind_int64(stmt, 3, id);
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

bool import_detail_insert(sqlite3    *db,
                          long long   import_id,
                          long long   medicine_id,
                          const char *qty,
                          const char *price)
{
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT OR REPLACE INTO MedicineImportDetails (medicine_import_id, medicine_id, qty, price) "
        "VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, import_id);
    sqlite3_bind_int64(stmt, 2, medicine_id);
    sqlite3_bind_text(stmt, 3, qty, -1, SQLITE_TRANSIENT);
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

bool import_detail_delete(sqlite3 *db, long long import_id, long long medicine_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "DELETE FROM MedicineImportDetails WHERE medicine_import_id = ? AND medicine_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_bind_int64(stmt, 1, import_id);
    sqlite3_bind_int64(stmt, 2, medicine_id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    sqlite3_finalize(stmt);
    return result;
}
