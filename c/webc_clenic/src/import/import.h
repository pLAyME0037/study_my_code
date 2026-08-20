#ifndef SRC_IMPORT_H_
#define SRC_IMPORT_H_

#include "../db/db.h"

typedef struct {
    long long id;
    char *date;
    char *amount;
    int detail_count;
} Import_Header;

typedef struct {
    Import_Header *items;
    size_t count;
    size_t capacity;
} Import_Headers;

typedef struct {
    long long medicine_id;
    char *medicine_name;
    char *qty;
    char *price;
} Import_Detail;

typedef struct {
    Import_Detail *items;
    size_t count;
    size_t capacity;
} Import_Details;

typedef struct {
    long long id;
    char *name;
} Medicine_Option;

typedef struct {
    Medicine_Option *items;
    size_t count;
    size_t capacity;
} Medicine_Options;

bool import_rows_load(sqlite3 *db, Import_Headers *rows, long long only_id);
bool import_details_load(sqlite3 *db, Import_Details *rows, long long import_id);
bool import_medicines_load(sqlite3 *db, Medicine_Options *opts);
bool import_insert(sqlite3 *db, const char *date, const char *amount);
bool import_update(sqlite3 *db, long long id, const char *date, const char *amount);
bool import_delete(sqlite3 *db, long long id);
bool import_detail_insert(sqlite3 *db, long long import_id,
                          long long medicine_id, const char *qty, const char *price);
bool import_detail_delete(sqlite3 *db, long long import_id, long long medicine_id);

#endif // SRC_IMPORT_H_
