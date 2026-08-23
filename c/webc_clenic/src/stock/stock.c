#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "sqlite3.h"
#include "stock.h"
#include "../db/db.h"

bool load_stocks(sqlite3 *db, Stock_Rows *rows, int only_medicine_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    String_View sql;

    if (only_medicine_id < 0) {
        sql = sv_from_cstr("SELECT m.id, m.name, "
            "COALESCE(SUM(d.qty), 0) AS stock_qty, "
            "MAX(mi.date) AS date_last_import "
            "FROM Medicines m "
            "LEFT JOIN MedicineImportDetails d ON d.medicine_id = m.id "
            "LEFT JOIN MedicineImport mi ON mi.id = d.medicine_import_id "
            "GROUP BY m.id, m.name "
            "ORDER BY m.name ASC;");
    } else {
        sql = sv_from_cstr(temp_sprintf("SELECT m.id, m.name, "
            "COALESCE(SUM(d.qty), 0) AS stock_qty, "
            "MAX(mi.date) AS date_last_import "
            "FROM Medicines m "
            "LEFT JOIN MedicineImportDetails d ON d.medicine_id = m.id "
            "LEFT JOIN MedicineImport mi ON mi.id = d.medicine_import_id "
            "WHERE m.id = %d "
            "GROUP BY m.id, m.name;", only_medicine_id));
    }

    if (sqlite3_prepare_v2(db, sql.data, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        int column = 0;
        long long medicine_id = sqlite3_column_int64(stmt, column++);
        const char *name = (const char *)sqlite3_column_text(stmt, column++);
        const char *qty = (const char *)sqlite3_column_text(stmt, column++);
        const char *date = (const char *)sqlite3_column_text(stmt, column++);
        da_append(rows, ((Stock_Row) {
            .medicine_id       = medicine_id,
            .medicine_name     = name ? temp_strdup(name) : NULL,
            .stock_qty         = qty  ? temp_strdup(qty)  : NULL,
            .date_last_import  = date ? temp_strdup(date) : NULL,
        }));
    }

    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    if (stmt) sqlite3_finalize(stmt);
    return result;
}
