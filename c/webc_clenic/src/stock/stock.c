#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "sqlite3.h"
#include "stock.h"
#include "../db/db.h"

bool load_stocks(sqlite3 *db, Stock_Rows *rows, int only_medicine_id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    String_View sql = sv_from_cstr(
        only_medicine_id < 0
        ? "SELECT s.medicine_id, m.name, s.stock_qty, s.date_last_import "
          "FROM MedicineStock s JOIN Medicines m ON m.id = s.medicine_id "
          "ORDER BY m.name ASC;"
        : temp_sprintf("SELECT s.medicine_id, m.name, s.stock_qty, s.date_last_import "
                       "FROM MedicineStock s JOIN Medicines m ON m.id = s.medicine_id "
                       "WHERE s.medicine_id = %d;", only_medicine_id));

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

bool load_medicine_options(sqlite3 *db, Stock_Medicine_Options *opts) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    if (sqlite3_prepare_v2(db, "SELECT id, name FROM Medicines ORDER BY name ASC;",
                           -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

    int ret = SQLITE_DONE;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        long long id = sqlite3_column_int64(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        da_append(opts, ((Stock_Medicine_Option) {
            .id   = id,
            .name = name ? temp_strdup(name) : NULL,
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

bool insert_stock(sqlite3 *db, long long medicine_id, const char *stock_qty, const char *date_last_import) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    if (sqlite3_prepare_v2(db, "INSERT OR REPLACE INTO MedicineStock (medicine_id, stock_qty, date_last_import) VALUES (?, ?, ?);",
                           -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (sqlite3_bind_int64(stmt, 1, medicine_id) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (sqlite3_bind_text(stmt, 2, stock_qty, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (date_last_import && date_last_import[0]) {
        if (sqlite3_bind_text(stmt, 3, date_last_import, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }
    } else {
        if (sqlite3_bind_null(stmt, 3) != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    if (stmt) sqlite3_finalize(stmt);
    return result;
}

bool update_stock(sqlite3 *db, int id, const char *stock_qty, const char *date_last_import) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    if (sqlite3_prepare_v2(db, "UPDATE MedicineStock SET stock_qty = ?, date_last_import = ? WHERE medicine_id = ?;",
                           -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (sqlite3_bind_text(stmt, 1, stock_qty, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (date_last_import && date_last_import[0]) {
        if (sqlite3_bind_text(stmt, 2, date_last_import, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }
    } else {
        if (sqlite3_bind_null(stmt, 2) != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }
    }
    if (sqlite3_bind_int(stmt, 3, id) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    if (stmt) sqlite3_finalize(stmt);
    return result;
}

bool delete_stock(sqlite3 *db, int id) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;

    if (sqlite3_prepare_v2(db, "DELETE FROM MedicineStock WHERE medicine_id = ?;", -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

defer:
    if (stmt) sqlite3_finalize(stmt);
    return result;
}
