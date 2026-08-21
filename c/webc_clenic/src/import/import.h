#ifndef SRC_IMPORT_H_
#define SRC_IMPORT_H_

#include "../db/db.h"

typedef struct {
    long long id;
    char *date;
    char *medicine_id;
    char *medicine_name;
    char *qty;
    char *price;
    char *amount;
} Import_Row;

typedef struct {
    Import_Row *items;
    size_t count;
    size_t capacity;
} Import_Rows;

typedef struct {
    long long id;
    char *name;
} Medicine_Option;

typedef struct {
    Medicine_Option *items;
    size_t count;
    size_t capacity;
} Medicine_Options;

bool import_rows_load(sqlite3 *db, Import_Rows *rows, long long only_id);
bool import_medicines_load(sqlite3 *db, Medicine_Options *opts);
bool import_insert_line(sqlite3 *db, const char *date,
                        long long medicine_id, const char *qty, const char *price);
bool import_update_line(sqlite3 *db, long long id, const char *date,
                        long long medicine_id, const char *qty, const char *price);
bool import_delete(sqlite3 *db, long long id);

#endif // SRC_IMPORT_H_
