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

bool load_stocks(sqlite3 *db, Stock_Rows *rows, int only_medicine_id);

#endif // SRC_STOCK_H_