#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "sqlite3.h"
#include "stock.h"
#include "../db/db.h"

bool load_stocks(sqlite3 *db, Stock_Rows *rows, int only_medicine_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    const char *where = "";
    if (only_medicine_id >= 0) {
        where = temp_sprintf(" WHERE m.id = %d", only_medicine_id);
    }

    char *sql = temp_sprintf(
        "SELECT m.id, m.name, "
        "COALESCE(i.q,0) AS in_qty, "
        "COALESCE(p.q,0)+COALESCE(o.q,0) AS sold_qty, "
        "COALESCE(i.q,0)-COALESCE(p.q,0)-COALESCE(o.q,0) AS stock_qty, "
        "last.d AS date_last_import "
        "FROM Medicines m "
        "LEFT JOIN (SELECT medicine_id, SUM(qty) q FROM MedicineImportDetails GROUP BY medicine_id) i ON i.medicine_id=m.id "
        "LEFT JOIN (SELECT medicine_id, SUM(qty) q FROM PatientMedicineInvoiceDetails GROUP BY medicine_id) p ON p.medicine_id=m.id "
        "LEFT JOIN (SELECT medicine_id, SUM(qty) q FROM OrganizationInvoiceDetail GROUP BY medicine_id) o ON o.medicine_id=m.id "
        "LEFT JOIN (SELECT d.medicine_id mid, MAX(mi.date) d FROM MedicineImportDetails d "
        "           JOIN MedicineImport mi ON mi.id=d.medicine_import_id GROUP BY d.medicine_id) last ON last.mid=m.id "
        "%s ORDER BY m.name ASC;", where);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        int column = 0;
        long long medicine_id = sqlite3_column_int64(stmt, column++);
        const char *name = (const char *)sqlite3_column_text(stmt, column++);
        const char *inq  = (const char *)sqlite3_column_text(stmt, column++);
        const char *sold = (const char *)sqlite3_column_text(stmt, column++);
        const char *qty  = (const char *)sqlite3_column_text(stmt, column++);
        const char *date = (const char *)sqlite3_column_text(stmt, column++);
        da_append(rows, ((Stock_Row) {
            .medicine_id       = medicine_id,
            .medicine_name     = name ? temp_strdup(name) : NULL,
            .in_qty            = inq  ? temp_strdup(inq)  : NULL,
            .sold_qty          = sold ? temp_strdup(sold) : NULL,
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
