#ifndef SRC_STOCK_H_
#define SRC_STOCK_H_

#include <stdbool.h>
#include <stddef.h>
#include "sqlite3.h"

typedef struct {
    long long medicine_id;
    const char *medicine_name;
    const char *stock_qty;
    const char *date_last_import;
} Stock_Row;

typedef struct {
    Stock_Row *items;
    size_t count;
    size_t capacity;
} Stock_Rows;

typedef struct {
    long long id;
    const char *name;
} Stock_Medicine_Option;

typedef struct {
    Stock_Medicine_Option *items;
    size_t count;
    size_t capacity;
} Stock_Medicine_Options;

bool load_stocks(sqlite3 *db, Stock_Rows *rows, int only_medicine_id);
bool load_medicine_options(sqlite3 *db, Stock_Medicine_Options *opts);
bool insert_stock(sqlite3 *db, long long medicine_id, const char *stock_qty, const char *date_last_import);
bool update_stock(sqlite3 *db, int id, const char *stock_qty, const char *date_last_import);
bool delete_stock(sqlite3 *db, int id);

#endif // SRC_STOCK_H_
