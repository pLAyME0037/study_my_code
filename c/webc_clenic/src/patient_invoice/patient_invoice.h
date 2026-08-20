#ifndef SRC_PATIENT_INVOICE_H_
#define SRC_PATIENT_INVOICE_H_

#include "../db/db.h"

// ---------------------------------------------------------------------------
// Generic FK option lists (for select dropdowns)
// ---------------------------------------------------------------------------

typedef struct {
    long long id;
    char *label;
} Pio_Option;

typedef struct {
    Pio_Option *items;
    size_t count;
    size_t capacity;
} Pio_Options;

// Loads `SELECT id, <label_col> FROM <table> ORDER BY <label_col> ASC`.
bool pio_options_load(sqlite3 *db, Pio_Options *opts,
                      const char *table, const char *label_col);

// Loads room options with joined room-type label + price.
bool pio_room_options_load(sqlite3 *db, Pio_Options *opts);

// ---------------------------------------------------------------------------
// Patient Medicine Invoice (header + details)
// ---------------------------------------------------------------------------

typedef struct {
    long long id;
    char *patient_name;
    char *invoice_date;
    char *amount_in_riel;
    char *amount_in_dollar;
    int detail_count;
} Pmi_Header;

typedef struct {
    Pmi_Header *items;
    size_t count;
    size_t capacity;
} Pmi_Headers;

typedef struct {
    long long medicine_id;
    char *medicine_name;
    char *qty;
    char *price;
    char *amount;
    char *currency;
} Pmi_Detail;

typedef struct {
    Pmi_Detail *items;
    size_t count;
    size_t capacity;
} Pmi_Details;

bool pmi_headers_load(sqlite3 *db, Pmi_Headers *rows, long long only_id);
bool pmi_details_load(sqlite3 *db, Pmi_Details *rows, long long invoice_id);
bool pmi_insert(sqlite3 *db, long long *out_id,
                long long patient_id, const char *invoice_date,
                const char *riel, const char *dollar);
bool pmi_update(sqlite3 *db, long long id, const char *invoice_date,
                const char *riel, const char *dollar);
bool pmi_delete(sqlite3 *db, long long id);
bool pmi_detail_insert(sqlite3 *db, long long invoice_id, long long medicine_id,
                       const char *qty, const char *price,
                       const char *amount, const char *currency);
bool pmi_detail_delete(sqlite3 *db, long long invoice_id, long long medicine_id);

// ---------------------------------------------------------------------------
// Patient Invoice Out (header + details)
// ---------------------------------------------------------------------------

typedef struct {
    long long id;
    char *patient_name;
    char *room_name;
    char *room_price;
    char *start_date;
    char *end_date;
    char *room_day;
    int detail_count;
} Pio_Header;

typedef struct {
    Pio_Header *items;
    size_t count;
    size_t capacity;
} Pio_Headers;

typedef struct {
    long long daily_id;
    char *daily_label;
} Pio_Detail;

typedef struct {
    Pio_Detail *items;
    size_t count;
    size_t capacity;
} Pio_Details;

bool pio_headers_load(sqlite3 *db, Pio_Headers *rows, long long only_id);
bool pio_details_load(sqlite3 *db, Pio_Details *rows, long long out_id);
bool pio_insert(sqlite3 *db, long long *out_id,
                long long patient_id, long long room_id, const char *room_price,
                const char *start_date, const char *end_date, const char *room_day);
bool pio_update(sqlite3 *db, long long id, const char *room_price,
                const char *start_date, const char *end_date, const char *room_day);
bool pio_delete(sqlite3 *db, long long id);
bool pio_detail_insert(sqlite3 *db, long long out_id, long long daily_id);
bool pio_detail_delete(sqlite3 *db, long long out_id, long long daily_id);

#endif // SRC_PATIENT_INVOICE_H_
