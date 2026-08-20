#ifndef SRC_ORG_H_
#define SRC_ORG_H_

#include "../nob.h"
#include "../db/db.h"

// ---------------------------------------------------------------------------
// Organization Daily Invoice (header + illness detail lines)
// ---------------------------------------------------------------------------

typedef struct {
    long long id;
    char *org_name;
    char *patient_name;
    char *invoice_date;
    char *amount_in_riel;
    char *amount_in_dollar;
    int detail_count;
} Org_Daily_Header;

typedef struct {
    Org_Daily_Header *items;
    size_t count;
    size_t capacity;
} Org_Daily_Headers;

typedef struct {
    long long ill_id;
    char *ill_name;
    char *price;
    char *currency;
} Org_Daily_Detail;

typedef struct {
    Org_Daily_Detail *items;
    size_t count;
    size_t capacity;
} Org_Daily_Details;

bool org_daily_headers_load(sqlite3 *db, Org_Daily_Headers *rows, long long only_id);
bool org_daily_details_load(sqlite3 *db, Org_Daily_Details *rows, long long daily_id);
bool org_daily_insert(sqlite3 *db, long long *out_id,
                      long long organization_id, long long patient_id,
                      const char *invoice_date, const char *riel, const char *dollar);
bool org_daily_update(sqlite3 *db, long long id, const char *invoice_date,
                      const char *riel, const char *dollar);
bool org_daily_delete(sqlite3 *db, long long id);
bool org_daily_detail_insert(sqlite3 *db, long long daily_id, long long ill_id,
                             const char *price, const char *currency);
bool org_daily_detail_delete(sqlite3 *db, long long daily_id, long long ill_id);

// ---------------------------------------------------------------------------
// Organization Balance (composite PK: org + invoice)
// ---------------------------------------------------------------------------

typedef struct {
    long long org_id;
    long long invoice_id;
    char *org_name;
    char *invoice_label;
    char *balance_amount;
    char *balance;
} Org_Balance_Row;

typedef struct {
    Org_Balance_Row *items;
    size_t count;
    size_t capacity;
} Org_Balance_Rows;

bool org_balances_load(sqlite3 *db, Org_Balance_Rows *rows);
bool org_balance_insert(sqlite3 *db, long long organization_id, long long invoice_id,
                        const char *balance_amount, const char *balance);
bool org_balance_delete(sqlite3 *db, long long organization_id, long long invoice_id);

// ---------------------------------------------------------------------------
// Organization Invoice Out (header + linked org-invoice detail rows)
// ---------------------------------------------------------------------------

typedef struct {
    long long id;
    char *org_name;
    char *patient_name;
    char *room_name;
    char *room_price;
    char *start_date;
    char *end_date;
    char *room_day;
    int detail_count;
} Org_Out_Header;

typedef struct {
    Org_Out_Header *items;
    size_t count;
    size_t capacity;
} Org_Out_Headers;

typedef struct {
    long long invoice_id;
    char *invoice_label;
} Org_Out_Detail;

typedef struct {
    Org_Out_Detail *items;
    size_t count;
    size_t capacity;
} Org_Out_Details;

bool org_out_headers_load(sqlite3 *db, Org_Out_Headers *rows, long long only_id);
bool org_out_details_load(sqlite3 *db, Org_Out_Details *rows, long long out_id);
bool org_out_insert(sqlite3 *db, long long *out_id,
                    long long organization_id, long long patient_id, long long room_id,
                    const char *room_price, const char *start_date,
                    const char *end_date, const char *room_day);
bool org_out_update(sqlite3 *db, long long id, const char *room_price,
                    const char *start_date, const char *end_date, const char *room_day);
bool org_out_delete(sqlite3 *db, long long id);
bool org_out_detail_insert(sqlite3 *db, long long out_id, long long invoice_id);
bool org_out_detail_delete(sqlite3 *db, long long out_id, long long invoice_id);

#endif // SRC_ORG_H_
