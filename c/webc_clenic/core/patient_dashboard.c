#include "patient_dashboard.h"
#include "../core/master_detail.h"
#include "../src/db/db.h"
#include "../core/header.h"
#include "../core/footer.h"

static MD_Column patient_columns[] = {
    { .name = "name",          .label = "Name",          .type = COL_TYPE_TEXT, .nullable = false },
    { .name = "sex",           .label = "Sex",           .type = COL_TYPE_TEXT, .nullable = true },
    { .name = "age",           .label = "Age",           .type = COL_TYPE_NUM, .nullable = true },
    { .name = "tel",           .label = "Tel",           .type = COL_TYPE_TEXT, .nullable = true },
    { .name = "patient_type",  .label = "Type",          .type = COL_TYPE_TEXT, .nullable = true },
    { .name = "register_date", .label = "Register Date", .type = COL_TYPE_DATE, .nullable = true },
    { .name = "district",      .label = "District",      .type = COL_TYPE_TEXT, .nullable = true },
    { .name = "province",      .label = "Province",      .type = COL_TYPE_TEXT, .nullable = true },
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
    {
        .name     = "room_id",
        .label    = "Room",
        .type     = COL_TYPE_FK_SELECT,
        .nullable = false,
        .fk_table = "Rooms",
        .fk_label = "price",
        .fk_value = "id"
    },
    { .name = "room_price", .label = "Room Price", .type = COL_TYPE_NUM,  .nullable = true },
    { .name = "start_date", .label = "Start Date", .type = COL_TYPE_DATE, .nullable = true },
    { .name = "end_date",   .label = "End Date",   .type = COL_TYPE_DATE, .nullable = true },
    { .name = "room_day",   .label = "Days",       .type = COL_TYPE_NUM,  .nullable = true },
};

static MD_Column appointment_columns[] = {
    {
        .name     = "doctor_id",
        .label    = "Doctor",
        .type     = COL_TYPE_FK_SELECT,
        .nullable = false,
        .fk_table = "Doctors",
        .fk_label = "name",
        .fk_value = "id"
    },
    { .name = "date", .label = "Date", .type = COL_TYPE_DATE, .nullable = true },
    { .name = "time", .label = "Time", .type = COL_TYPE_TEXT, .nullable = true },
};

static const char *daily_invoice_sums[]    = { "amount_in_riel", "amount_in_dollar" };
static const char *medicine_invoice_sums[] = { "amount_in_riel", "amount_in_dollar" };
static const char *room_invoice_sums[]     = { "room_price" };
static const char *appointment_sums[]      = { };

static MD_ChildTab patient_children[] = {
    { .table       = "PatientDailyInvoices",    .title = "Daily Invoices",
      .fk_column   = "patient_id",              .id_column = "id",
      .columns     = daily_invoice_columns,     .column_count = ARRAY_LEN(daily_invoice_columns),
      .sum_columns = daily_invoice_sums,        .sum_column_count = ARRAY_LEN(daily_invoice_sums) },

    { .table       = "PatientMedicineInvoices", .title = "Medicine Invoices",
      .fk_column   = "patient_id",              .id_column = "id",
      .columns     = medicine_invoice_columns,  .column_count = ARRAY_LEN(medicine_invoice_columns),
      .sum_columns = medicine_invoice_sums,     .sum_column_count = ARRAY_LEN(medicine_invoice_sums) },

    { .table       = "PatientInvoiceOut",       .title = "Room Invoices",
      .fk_column   = "patient_id",              .id_column = "id",
      .columns     = room_invoice_columns,      .column_count = ARRAY_LEN(room_invoice_columns),
      .sum_columns = room_invoice_sums,         .sum_column_count = ARRAY_LEN(room_invoice_sums) },

    { .table       = "Appointment",             .title = "Appointments",
      .fk_column   = "patient_id",              .id_column = "id",
      .columns     = appointment_columns,       .column_count = ARRAY_LEN(appointment_columns),
      .sum_columns = appointment_sums,          .sum_column_count = ARRAY_LEN(appointment_sums) },
};

static MD_MasterConfig patient_config = {
    .table          = "Patients",
    .title          = "Patients",
    .id_column      = "id",
    .columns        = patient_columns,
    .column_count   = ARRAY_LEN(patient_columns),
    .children       = patient_children,
    .children_count = ARRAY_LEN(patient_children),
};

void serve_patient_dashboard(Serve_Context *sc) {
    serve_master_detail_list(sc, &patient_config);
}
