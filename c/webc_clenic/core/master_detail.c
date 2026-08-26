#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "master_detail.h"
#include "../src/db/db.h"
#include "../core/header.h"
#include "../core/footer.h"
#include "../core/http/utils.h"

static MD_Column patient_columns[] = {
    { .name = "name",          .label = "Name",       .type = COL_TYPE_TEXT,     .nullable = false },
    { .name = "sex",           .label = "Sex",        .type = COL_TYPE_TEXT,     .nullable = true  },
    { .name = "age",           .label = "Age",        .type = COL_TYPE_NUM,      .nullable = true  },
    { .name = "tel",           .label = "Tel",        .type = COL_TYPE_TEXT,     .nullable = true  },
    { .name = "patient_type",  .label = "Type",       .type = COL_TYPE_TEXT,     .nullable = true  },
    { .name = "register_date", .label = "Register",   .type = COL_TYPE_DATE,     .nullable = true  },
    { .name = "district",      .label = "District",   .type = COL_TYPE_TEXT,     .nullable = true  },
    { .name = "province",      .label = "Province",   .type = COL_TYPE_TEXT,     .nullable = true  },
};

static MD_Column daily_invoice_columns[] = {
    { .name = "invoice_date",     .label = "Date",          .type = COL_TYPE_DATE, .nullable = true },
    { .name = "amount_in_riel",   .label = "Amount (Riel)", .type = COL_TYPE_NUM,  .nullable = true },
    { .name = "amount_in_dollar", .label = "Amount ($)",    .type = COL_TYPE_NUM,  .nullable = true },
};

static MD_Column medicine_invoice_columns[] = {
    { .name = "invoice_date",     .label = "Date",          .type = COL_TYPE_DATE, .nullable = true },
    { .name = "amount_in_riel",   .label = "Amount (Riel)", .type = COL_TYPE_NUM,  .nullable = true },
    { .name = "amount_in_dollar", .label = "Amount ($)",    .type = COL_TYPE_NUM,  .nullable = true },
};

static MD_Column room_invoice_columns[] = {
    { .name = "room_id",      .label = "Room",        .type = COL_TYPE_FK_SELECT, .nullable = false, .fk_table = "Rooms",      .fk_label = "price", .fk_value = "id" },
    { .name = "room_price",   .label = "Room Price",  .type = COL_TYPE_NUM,       .nullable = true },
    { .name = "start_date",   .label = "Start Date",  .type = COL_TYPE_DATE,      .nullable = true },
    { .name = "end_date",     .label = "End Date",    .type = COL_TYPE_DATE,      .nullable = true },
    { .name = "room_day",     .label = "Days",        .type = COL_TYPE_NUM,       .nullable = true },
};

static MD_Column appointment_columns[] = {
    { .name = "doctor_id",    .label = "Doctor",      .type = COL_TYPE_FK_SELECT, .nullable = false, .fk_table = "Doctors",    .fk_label = "name", .fk_value = "id" },
    { .name = "date",         .label = "Date",        .type = COL_TYPE_DATE,      .nullable = true },
    { .name = "time",         .label = "Time",        .type = COL_TYPE_TEXT,      .nullable = true },
};

static MD_ChildTab patient_children[] = {
    { .table = "PatientDailyInvoices",    .title = "Daily Invoices",
      .fk_column = "patient_id", .id_column = "id",
      .crud_path = "/patient-daily-invoices",
      .columns = daily_invoice_columns,    .column_count = ARRAY_LEN(daily_invoice_columns) },
    { .table = "PatientMedicineInvoices", .title = "Medicine Invoices",
      .fk_column = "patient_id", .id_column = "id",
      .crud_path = "/patient-medicine-invoices",
      .columns = medicine_invoice_columns, .column_count = ARRAY_LEN(medicine_invoice_columns) },
    { .table = "PatientInvoiceOut",       .title = "Room Invoices",
      .fk_column = "patient_id", .id_column = "id",
      .crud_path = "/patient-invoice-out",
      .columns = room_invoice_columns,     .column_count = ARRAY_LEN(room_invoice_columns) },
    { .table = "Appointment",             .title = "Appointments",
      .fk_column = "patient_id", .id_column = "id",
      .crud_path = "/appointments",
      .columns = appointment_columns,      .column_count = ARRAY_LEN(appointment_columns) },
};

static MD_Column pdi_detail_columns[] = {
    { .name = "ill_type_id", .label = "Illness",  .type = COL_TYPE_FK_SELECT, .nullable = false, .fk_table = "IllTypes", .fk_label = "type", .fk_value = "id" },
    { .name = "patientDaily_invoice_detail_price",    .label = "Price",    .type = COL_TYPE_NUM,  .nullable = true },
    { .name = "patientDaily_invoice_detail_currency", .label = "Currency", .type = COL_TYPE_TEXT, .nullable = true },
};
static MD_ChildTab pdi_children[] = {
    { .table = "PatientDailyInvoicesDetails", .title = "Details",
      .fk_column = "patient_daily_invoice_id", .id_column = "rowid",
      .crud_path = "/patient-daily-invoices-details",
      .columns = pdi_detail_columns, .column_count = ARRAY_LEN(pdi_detail_columns) },
};

static MD_Column pmi_detail_columns[] = {
    { .name = "medicine_id", .label = "Medicine", .type = COL_TYPE_FK_SELECT, .nullable = false, .fk_table = "Medicines", .fk_label = "name", .fk_value = "id" },
    { .name = "qty",         .label = "Qty",      .type = COL_TYPE_NUM,       .nullable = true },
    { .name = "price",       .label = "Price",    .type = COL_TYPE_NUM,       .nullable = true },
    { .name = "amount",      .label = "Amount",   .type = COL_TYPE_NUM,       .nullable = true },
    { .name = "currency",    .label = "Currency", .type = COL_TYPE_TEXT,      .nullable = true },
};
static MD_ChildTab pmi_children[] = {
    { .table = "PatientMedicineInvoiceDetails", .title = "Details",
      .fk_column = "patient_medicine_invoice_id", .id_column = "rowid",
      .crud_path = "/patient-medicine-invoices-details",
      .columns = pmi_detail_columns, .column_count = ARRAY_LEN(pmi_detail_columns) },
};

static MD_Column pio_detail_columns[] = {
    { .name = "patient_daily_invoice_id", .label = "Daily Invoice", .type = COL_TYPE_FK_SELECT, .nullable = true, .fk_table = "PatientDailyInvoices", .fk_label = "id", .fk_value = "id" },
};
static MD_ChildTab pio_children[] = {
    { .table = "PatientInvoiceOutDetails", .title = "Details",
      .fk_column = "patient_invoice_out_id", .id_column = "rowid",
      .crud_path = "/patient-invoice-out-details",
      .columns = pio_detail_columns, .column_count = ARRAY_LEN(pio_detail_columns) },
};

static MD_Column org_columns[] = {
    { .name = "name",        .label = "Name",        .type = COL_TYPE_TEXT,     .nullable = false },
    { .name = "start_date",  .label = "Start Date",  .type = COL_TYPE_DATE,     .nullable = true  },
    { .name = "end_date",    .label = "End Date",    .type = COL_TYPE_DATE,     .nullable = true  },
    { .name = "description", .label = "Description", .type = COL_TYPE_TEXTAREA, .nullable = true  },
};

static MD_Column org_daily_invoice_columns[] = {
    { .name = "invoice_date",     .label = "Date",          .type = COL_TYPE_DATE, .nullable = true },
    { .name = "amount_in_riel",   .label = "Amount (Riel)", .type = COL_TYPE_NUM,  .nullable = true },
    { .name = "amount_in_dollar", .label = "Amount ($)",    .type = COL_TYPE_NUM,  .nullable = true },
};

static MD_Column org_invoice_columns[] = {
    { .name = "invoice_date",     .label = "Date",          .type = COL_TYPE_DATE, .nullable = true },
    { .name = "amount_in_riel",   .label = "Amount (Riel)", .type = COL_TYPE_NUM,  .nullable = true },
    { .name = "amount_in_dollar", .label = "Amount ($)",    .type = COL_TYPE_NUM,  .nullable = true },
};


static MD_Column org_payment_columns[] = {
    { .name = "organization_id",        .label = "Organization", .type = COL_TYPE_FK_SELECT, .nullable = false, .fk_table = "Organizations",   .fk_label = "name", .fk_value = "id" },
    { .name = "organization_invoice_id",.label = "Org Invoice",   .type = COL_TYPE_FK_SELECT, .nullable = true,  .fk_table = "OrganizationInvoices", .fk_label = "invoice_date", .fk_value = "id" },
    { .name = "date",                   .label = "Date",          .type = COL_TYPE_DATE,     .nullable = true },
    { .name = "price",                  .label = "Price",         .type = COL_TYPE_NUM,      .nullable = true },
    { .name = "currency",               .label = "Currency",      .type = COL_TYPE_TEXT,     .nullable = true },
};

static MD_Column org_balance_columns[] = {
    { .name = "balance_amount", .label = "Balance Amount", .type = COL_TYPE_NUM,  .nullable = true },
    { .name = "balance",        .label = "Balance",        .type = COL_TYPE_NUM,  .nullable = true },
};

static MD_Column org_invoice_detail_columns[] = {
    { .name = "medicine_id", .label = "Medicine", .type = COL_TYPE_FK_SELECT, .nullable = false, .fk_table = "Medicines", .fk_label = "name", .fk_value = "id" },
    { .name = "qty",         .label = "Qty",      .type = COL_TYPE_NUM,       .nullable = true },
    { .name = "price",       .label = "Price",    .type = COL_TYPE_NUM,       .nullable = true },
    { .name = "amount",      .label = "Amount",   .type = COL_TYPE_NUM,       .nullable = true },
    { .name = "currency",    .label = "Currency", .type = COL_TYPE_TEXT,      .nullable = true },
};
static MD_ChildTab org_inv_detail_children[] = {
    { .table = "OrganizationInvoiceDetail", .title = "Details",
      .fk_column = "organization_invoice_id", .id_column = "rowid",
      .crud_path = "/organization-invoice-details",
      .columns = org_invoice_detail_columns, .column_count = ARRAY_LEN(org_invoice_detail_columns) },
};

static MD_Column org_out_columns[] = {
    { .name = "patient_id",   .label = "Patient",    .type = COL_TYPE_FK_SELECT, .nullable = false, .fk_table = "Patients", .fk_label = "name", .fk_value = "id" },
    { .name = "room_id",      .label = "Room",       .type = COL_TYPE_FK_SELECT, .nullable = true,  .fk_table = "Rooms",    .fk_label = "price", .fk_value = "id" },
    { .name = "room_price",   .label = "Room Price", .type = COL_TYPE_NUM,       .nullable = true },
    { .name = "start_date",   .label = "Start Date", .type = COL_TYPE_DATE,      .nullable = true },
    { .name = "end_date",     .label = "End Date",   .type = COL_TYPE_DATE,      .nullable = true },
    { .name = "room_day",     .label = "Days",       .type = COL_TYPE_NUM,       .nullable = true },
};

static MD_ChildTab org_children[] = {
    { .table = "OrganizationDailyInvoices", .title = "Daily Invoices",
      .fk_column = "organization_id", .id_column = "id",
      .crud_path = "/organization-daily-invoices",
      .columns = org_daily_invoice_columns, .column_count = ARRAY_LEN(org_daily_invoice_columns) },
    { .table = "OrganizationInvoices",      .title = "Invoices",
      .fk_column = "organization_id", .id_column = "id",
      .crud_path = "/organization-invoices",
      .columns = org_invoice_columns, .column_count = ARRAY_LEN(org_invoice_columns) },
    { .table = "OrganizationPayment",       .title = "Payments",
      .fk_column = "organization_id", .id_column = "id",
      .crud_path = "/organization-payments",
      .columns = org_payment_columns, .column_count = ARRAY_LEN(org_payment_columns) },
    { .table = "OrganizationBalance",       .title = "Balances",
      .fk_column = "organization_id", .id_column = "rowid",
      .crud_path = "/organization-balances",
      .columns = org_balance_columns, .column_count = ARRAY_LEN(org_balance_columns) },
    { .table = "OrganizationInvoiceOut",    .title = "Invoice Out",
      .fk_column = "organization_id", .id_column = "id",
      .crud_path = "/organization-invoice-out",
      .columns = org_out_columns, .column_count = ARRAY_LEN(org_out_columns) },
};

static MD_Column org_di_detail_columns[] = {
    { .name = "ill_id",   .label = "Illness",  .type = COL_TYPE_FK_SELECT, .nullable = false, .fk_table = "IllTypes", .fk_label = "type", .fk_value = "id" },
    { .name = "price",    .label = "Price",    .type = COL_TYPE_NUM,  .nullable = true },
    { .name = "currency", .label = "Currency", .type = COL_TYPE_TEXT, .nullable = true },
};
static MD_ChildTab org_di_children[] = {
    { .table = "OrganizationDailyInvoiceDetails", .title = "Details",
      .fk_column = "organization_daily_id", .id_column = "rowid",
      .crud_path = "/organization-daily-invoices-details",
      .columns = org_di_detail_columns, .column_count = ARRAY_LEN(org_di_detail_columns) },
};

static MD_Column org_io_detail_columns[] = {
    { .name = "organization_invoice_id", .label = "Org Invoice", .type = COL_TYPE_FK_SELECT, .nullable = true, .fk_table = "OrganizationInvoices", .fk_label = "invoice_date", .fk_value = "id" },
};
static MD_ChildTab org_io_children[] = {
    { .table = "OrganizationInvoiceOutDetails", .title = "Details",
      .fk_column = "organization_invoice_out_id", .id_column = "rowid",
      .crud_path = "/organization-invoice-out-details",
      .columns = org_io_detail_columns, .column_count = ARRAY_LEN(org_io_detail_columns) },
};

static MD_Column med_import_columns[] = {
    { .name = "date",    .label = "Date",   .type = COL_TYPE_DATE, .nullable = true },
};

static MD_Column med_import_detail_columns[] = {
    { .name = "medicine_id", .label = "Medicine", .type = COL_TYPE_FK_SELECT, .nullable = false, .fk_table = "Medicines", .fk_label = "name", .fk_value = "id" },
    { .name = "qty",         .label = "Qty",      .type = COL_TYPE_NUM,       .nullable = true },
    { .name = "price",       .label = "Price",    .type = COL_TYPE_NUM,       .nullable = true },
};

static MD_ChildTab med_import_children[] = {
    { .table = "MedicineImportDetails", .title = "Import Details",
      .fk_column = "medicine_import_id", .id_column = "id",
      .crud_path = "/medicine-imports",
      .columns = med_import_detail_columns, .column_count = ARRAY_LEN(med_import_detail_columns) },
};

static MD_MasterConfig master_configs[] = {
    { .table = "Patients",        .title = "Patients",        .id_column = "id",
      .columns = patient_columns, .column_count = ARRAY_LEN(patient_columns),
      .children = patient_children, .children_count = ARRAY_LEN(patient_children) },
    { .table = "Organizations",   .title = "Organizations",   .id_column = "id",
      .columns = org_columns,     .column_count = ARRAY_LEN(org_columns),
      .children = org_children,   .children_count = ARRAY_LEN(org_children) },
    { .table = "MedicineImport",  .title = "Medicine Import", .id_column = "id",
      .columns = med_import_columns, .column_count = ARRAY_LEN(med_import_columns),
      .children = med_import_children, .children_count = ARRAY_LEN(med_import_children) },
    { .table = "PatientDailyInvoices",    .title = "Patient Daily Invoices",
      .id_column = "id",
      .columns = daily_invoice_columns, .column_count = ARRAY_LEN(daily_invoice_columns),
      .children = pdi_children, .children_count = ARRAY_LEN(pdi_children) },
    { .table = "PatientMedicineInvoices", .title = "Patient Medicine Invoices",
      .id_column = "id",
      .columns = medicine_invoice_columns, .column_count = ARRAY_LEN(medicine_invoice_columns),
      .children = pmi_children, .children_count = ARRAY_LEN(pmi_children) },
    { .table = "PatientInvoiceOut",       .title = "Patient Room Invoices",
      .id_column = "id",
      .columns = room_invoice_columns, .column_count = ARRAY_LEN(room_invoice_columns),
      .children = pio_children, .children_count = ARRAY_LEN(pio_children) },
    { .table = "OrganizationDailyInvoices", .title = "Org Daily Invoices",
      .id_column = "id",
      .columns = org_daily_invoice_columns, .column_count = ARRAY_LEN(org_daily_invoice_columns),
      .children = org_di_children, .children_count = ARRAY_LEN(org_di_children) },
    { .table = "OrganizationInvoiceOut",    .title = "Org Invoice Out",
      .id_column = "id",
      .columns = org_out_columns, .column_count = ARRAY_LEN(org_out_columns),
      .children = org_io_children, .children_count = ARRAY_LEN(org_io_children) },
    { .table = "OrganizationInvoices",      .title = "Org Invoices",
      .id_column = "id",
      .columns = org_invoice_columns, .column_count = ARRAY_LEN(org_invoice_columns),
      .children = org_inv_detail_children, .children_count = ARRAY_LEN(org_inv_detail_children) },
};

static size_t master_configs_count = ARRAY_LEN(master_configs);

const MD_MasterConfig *find_master_config(const char *table) {
    for (size_t i = 0; i < master_configs_count; ++i) {
        if (strcmp(master_configs[i].table, table) == 0) {
            return &master_configs[i];
        }
    }
    return NULL;
}

MD_MasterRows *md_master_rows_new(void) {
    MD_MasterRows *rows = malloc(sizeof(MD_MasterRows));
    rows->items = NULL;
    rows->count = 0;
    rows->capacity = 0;
    return rows;
}

void md_master_rows_free(MD_MasterRows *rows) {
    if (!rows) return;
    for (size_t i = 0; i < rows->count; ++i) {
        MD_MasterRow *row = &rows->items[i];
        free(row->values);
        if (row->children) {
            for (size_t ci = 0; ci < row->children_count; ++ci) {
                MD_ChildRows *crows = &row->children[ci];
                free(crows->items);
            }
            free(row->children);
        }
    }
    free(rows->items);
    free(rows);
}

static char *col_to_str(sqlite3_stmt *stmt, int col) {
    const char *val = (const char *)sqlite3_column_text(stmt, col);
    return val ? temp_strdup(val) : temp_strdup("");
}

static char *build_column_list(const MD_ChildTab *child) {
    String_Builder sb = {0};
    sb_append_cstr(&sb, child->id_column);
    for (size_t i = 0; i < child->column_count; ++i) {
        sb_append_cstr(&sb, ", ");
        sb_append_cstr(&sb, child->columns[i].name);
    }
    sb_append_null(&sb);
    return sb.items;
}

static bool load_child_rows(sqlite3           *db,
                            const MD_ChildTab *child,
                            long long          master_id,
                            MD_ChildRows      *out_rows)
{
    char *cols = build_column_list(child);
    char *sql = temp_sprintf("SELECT %s FROM %s WHERE %s = %lld ORDER BY %s DESC;",
                            cols, child->table, child->fk_column, master_id, child->id_column);
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return false;
    }
    int ret;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        int col_count = sqlite3_column_count(stmt);
        MD_ChildRow row = {0};
        row.value_count = col_count;
        row.values = malloc(col_count * sizeof(char *));
        for (int i = 0; i < col_count; ++i) {
            row.values[i] = col_to_str(stmt, i);
        }
        da_append(out_rows, row);
    }
    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool md_load_master_with_children(sqlite3               *db,
                                  const MD_MasterConfig *config,
                                  MD_MasterRows         *rows)
{
    char *sql = temp_sprintf("SELECT * FROM %s ORDER BY %s DESC;", config->table, config->id_column);
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return false;
    }
    int ret;
    for (ret = sqlite3_step(stmt); ret == SQLITE_ROW; ret = sqlite3_step(stmt)) {
        int col_count = sqlite3_column_count(stmt);
        MD_MasterRow row = {0};
        row.id = sqlite3_column_int64(stmt, 0);
        row.value_count = col_count;
        row.values = malloc(col_count * sizeof(char *));
        for (int i = 0; i < col_count; ++i) {
            row.values[i] = col_to_str(stmt, i);
        }
        row.children = malloc(config->children_count * sizeof(MD_ChildRows));
        row.children_count = config->children_count;
        for (size_t ci = 0; ci < config->children_count; ++ci) {
            row.children[ci].items = NULL;
            row.children[ci].count = 0;
            row.children[ci].capacity = 0;
            if (!load_child_rows(db, &config->children[ci], row.id, &row.children[ci])) {
                free(row.values);
                for (size_t ci2 = 0; ci2 <= ci; ++ci2) {
                    MD_ChildRows *cr = &row.children[ci2];
                    for (size_t j = 0; j < cr->count; ++j) {
                        free(cr->items[j].values);
                    }
                    free(cr->items);
                }
                free(row.children);
                sqlite3_finalize(stmt);
                return false;
            }
        }
        da_append(rows, row);
    }
    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

void serve_master_detail_list(Serve_Context *sc, const MD_MasterConfig *config) {
    sqlite3 *db = open_webc_db();
    if (!db) { serve_error(sc, 500); return; }
    MD_MasterRows *rows = md_master_rows_new();
    bool ok = md_load_master_with_children(db, config, rows);
    sqlite3_close(db);
    if (!ok) {
        md_master_rows_free(rows);
        serve_error(sc, 500);
        return;
    }
    String_Builder *sb = &sc->body;
    sb->count = 0;
    render_page_header(sb, config->title, config->table);
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#define INT(v) sb_append_cstr(sb, temp_sprintf("%zu", v));
#define LLINT(v) sb_append_cstr(sb, temp_sprintf("%lld", v));
#define STR(s) sb_append_cstr(sb, s);
#define ESCAPED(s) sb_append_html_escaped(sb, s ? s : "");
#define PAGE_TITLE(s) sb_append_cstr(sb, s);
#include "../auto_ctrl/cttochtml/component/master_detail.h"
#undef OUT
#undef INT
#undef LLINT
#undef STR
#undef ESCAPED
#undef PAGE_TITLE
    render_page_footer(sb);
    md_master_rows_free(rows);
    http_render_response(sc, 200, "text/html", sb_to_sv(*sb));
}

void serve_master_detail_by_table(Serve_Context *sc, const char *master_table) {
    const MD_MasterConfig *config = find_master_config(master_table);
    if (!config) {
        serve_error(sc, 404);
        return;
    }
    serve_master_detail_list(sc, config);
}
