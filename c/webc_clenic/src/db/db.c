#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "sqlite3.h"
#include "db.h"

const char *WEBC_DIR_PATH = NULL;
const char *WEBC_DB_PATH = NULL;
bool WEBC_TRACE_MIGRATION_QUERIES = false;

bool txn_begin(sqlite3 *db) {
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return false;
    }
    return true;
}

bool txn_commit(sqlite3 *db) {
    if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return false;
    }
    return true;
}

const char *migrations[] = {
    "CREATE TABLE IF NOT EXISTS Notes (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    title TEXT,\n"
    "    body TEXT,\n"
    "    created_at DATETIME DEFAULT CURRENT_TIMESTAMP\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS RoomTypes (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    type TEXT,\n"
    "    description TEXT\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS Rooms (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    room_type_id INTEGER,\n"
    "    price REAL,\n"
    "    currency TEXT,\n"
    "    FOREIGN KEY(room_type_id) REFERENCES RoomTypes\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS IllTypes (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    type TEXT,\n"
    "    description TEXT\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS IllPrices (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    ill_type_id INTEGER,\n"
    "    name TEXT,\n"
    "    price REAL,\n"
    "    currency TEXT,\n"
    "    description TEXT,\n"
    "    FOREIGN KEY(ill_type_id) REFERENCES IllTypes\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS Doctors (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    name TEXT,\n"
    "    sex TEXT,\n"
    "    skill TEXT,\n"
    "    tel TEXT,\n"
    "    addr TEXT\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS Departments (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    name TEXT,\n"
    "    description TEXT\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS DoctorWorkTimes (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    department_id INTEGER,\n"
    "    table_name TEXT,\n"
    "    time TEXT,\n"
    "    FOREIGN KEY(department_id) REFERENCES Departments\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS Supplier (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    company TEXT,\n"
    "    contact TEXT,\n"
    "    addr TEXT,\n"
    "    description TEXT\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS MedicineTypes (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    type TEXT,\n"
    "    description TEXT\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS Medicines (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    medicine_type_id INTEGER,\n"
    "    supplier_id INTEGER,\n"
    "    name TEXT,\n"
    "    price REAL,\n"
    "    currency TEXT,\n"
    "    percent NUMERIC,\n"
    "    description TEXT,\n"
    "    FOREIGN KEY(medicine_type_id) REFERENCES MedicineTypes,\n"
    "    FOREIGN KEY(supplier_id) REFERENCES Supplier\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS MedicineImport (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    date NUMERIC,\n"
    "    amount NUMERIC\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS MedicineImportDetails (\n"
    "    medicine_import_id INTEGER,\n"
    "    medicine_id INTEGER,\n"
    "    qty NUMERIC,\n"
    "    price REAL,\n"
    "    PRIMARY KEY (medicine_import_id, medicine_id),\n"
    "    FOREIGN KEY(medicine_import_id) REFERENCES MedicineImport,\n"
    "    FOREIGN KEY(medicine_id) REFERENCES Medicines\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS MedicineStock (\n"
    "    medicine_id INTEGER PRIMARY KEY ASC,\n"
    "    stock_qty NUMERIC,\n"
    "    date_last_import NUMERIC,\n"
    "    FOREIGN KEY(medicine_id) REFERENCES Medicines\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS Patients (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    name TEXT,\n"
    "    sex TEXT,\n"
    "    age NUMERIC,\n"
    "    tel TEXT,\n"
    "    patient_type TEXT,\n"
    "    register_date NUMERIC,\n"
    "    district TEXT,\n"
    "    province TEXT\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS PatientDailyInvoices (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    patient_id INTEGER,\n"
    "    invoice_date NUMERIC,\n"
    "    amount_in_riel REAL,\n"
    "    amount_in_dollar REAL,\n"
    "    FOREIGN KEY(patient_id) REFERENCES Patients\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS PatientMedicineInvoices (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    patient_id INTEGER,\n"
    "    invoice_date NUMERIC,\n"
    "    amount_in_riel REAL,\n"
    "    amount_in_dollar REAL,\n"
    "    FOREIGN KEY(patient_id) REFERENCES Patients\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS PatientMedicineInvoiceDetails (\n"
    "    patient_medicine_invoice_id INTEGER,\n"
    "    medicine_id INTEGER,\n"
    "    qty NUMERIC,\n"
    "    price REAL,\n"
    "    amount NUMERIC,\n"
    "    currency TEXT,\n"
    "    PRIMARY KEY (patient_medicine_invoice_id, medicine_id),\n"
    "    FOREIGN KEY(patient_medicine_invoice_id) REFERENCES PatientMedicineInvoices,\n"
    "    FOREIGN KEY(medicine_id) REFERENCES Medicines\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS PatientInvoiceOut (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    patient_id INTEGER,\n"
    "    room_id INTEGER,\n"
    "    room_price REAL,\n"
    "    start_date NUMERIC,\n"
    "    end_date NUMERIC,\n"
    "    room_day INTEGER,\n"
    "    FOREIGN KEY(patient_id) REFERENCES Patients,\n"
    "    FOREIGN KEY(room_id) REFERENCES Rooms\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS PatientInvoiceOutDetails (\n"
    "    patient_invoice_out_id INTEGER,\n"
    "    patient_daily_invoice_id INTEGER,\n"
    "    FOREIGN KEY(patient_invoice_out_id) REFERENCES PatientInvoiceOut,\n"
    "    FOREIGN KEY(patient_daily_invoice_id) REFERENCES PatientDailyInvoices\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS Organizations (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    name TEXT,\n"
    "    start_date NUMERIC,\n"
    "    end_date NUMERIC,\n"
    "    description TEXT\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS OrganizationDailyInvoices (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    organization_id INTEGER,\n"
    "    patient_id INTEGER,\n"
    "    invoice_date NUMERIC,\n"
    "    amount_in_riel REAL,\n"
    "    amount_in_dollar REAL,\n"
    "    FOREIGN KEY(organization_id) REFERENCES Organizations,\n"
    "    FOREIGN KEY(patient_id) REFERENCES Patients\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS OrganizationDailyInvoiceDetails (\n"
    "    organization_daily_id INTEGER,\n"
    "    ill_id INTEGER,\n"
    "    price REAL,\n"
    "    currency TEXT,\n"
    "    PRIMARY KEY (organization_daily_id, ill_id),\n"
    "    FOREIGN KEY(organization_daily_id) REFERENCES OrganizationDailyInvoices,\n"
    "    FOREIGN KEY(ill_id) REFERENCES IllTypes\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS OrganizationInvoices (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    organization_id INTEGER,\n"
    "    invoice_date NUMERIC,\n"
    "    amount_in_riel REAL,\n"
    "    amount_in_dollar REAL,\n"
    "    FOREIGN KEY(organization_id) REFERENCES Organizations\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS OrganizationInvoiceDetail (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    medicine_id INTEGER,\n"
    "    qty NUMERIC,\n"
    "    price REAL,\n"
    "    amount NUMERIC,\n"
    "    currency TEXT,\n"
    "    FOREIGN KEY(medicine_id) REFERENCES Medicines\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS OrganizationPayment (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    organization_id INTEGER,\n"
    "    organization_invoice_id INTEGER,\n"
    "    date NUMERIC,\n"
    "    price REAL,\n"
    "    currency TEXT,\n"
    "    FOREIGN KEY(organization_id) REFERENCES Organizations,\n"
    "    FOREIGN KEY(organization_invoice_id) REFERENCES OrganizationInvoices\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS OrganizationBalance (\n"
    "    organization_id INTEGER,\n"
    "    organization_invoice_id INTEGER,\n"
    "    balance_amount INTEGER,\n"
    "    balance REAL,\n"
    "    PRIMARY KEY (organization_id, organization_invoice_id),\n"
    "    FOREIGN KEY(organization_id) REFERENCES Organizations,\n"
    "    FOREIGN KEY(organization_invoice_id) REFERENCES OrganizationInvoices\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS OrganizationInvoiceOut (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    organization_id INTEGER,\n"
    "    patient_id INTEGER,\n"
    "    room_id INTEGER,\n"
    "    room_price REAL,\n"
    "    start_date NUMERIC,\n"
    "    end_date NUMERIC,\n"
    "    room_day INTEGER,\n"
    "    FOREIGN KEY(organization_id) REFERENCES Organizations,\n"
    "    FOREIGN KEY(patient_id) REFERENCES Patients,\n"
    "    FOREIGN KEY(room_id) REFERENCES Rooms\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS OrganizationInvoiceOutDetails (\n"
    "    organization_invoice_out_id INTEGER,\n"
    "    organization_invoice_id INTEGER,\n"
    "    FOREIGN KEY(organization_invoice_out_id) REFERENCES OrganizationInvoiceOut,\n"
    "    FOREIGN KEY(organization_invoice_id) REFERENCES OrganizationInvoices\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS Appointment (\n"
    "    doctor_id INTEGER,\n"
    "    patient_id INTEGER,\n"
    "    date NUMERIC,\n"
    "    time NUMERIC,\n"
    "    FOREIGN KEY(doctor_id) REFERENCES Doctors,\n"
    "    FOREIGN KEY(patient_id) REFERENCES Patients\n"
    ");\n",
    "CREATE TABLE IF NOT EXISTS Appointment_old (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    doctor_id INTEGER,\n"
    "    patient_id INTEGER,\n"
    "    date NUMERIC,\n"
    "    time NUMERIC,\n"
    "    FOREIGN KEY(doctor_id) REFERENCES Doctors,\n"
    "    FOREIGN KEY(patient_id) REFERENCES Patients\n"
    ");\n"
    "INSERT INTO Appointment_old (doctor_id, patient_id, date, time)\n"
    "    SELECT doctor_id, patient_id, date, time FROM Appointment;\n"
    "DROP TABLE Appointment;\n"
    "ALTER TABLE Appointment_old RENAME TO Appointment;\n",
    "CREATE TABLE IF NOT EXISTS Users (\n"
    "    id INTEGER PRIMARY KEY ASC,\n"
    "    name TEXT,\n"
    "    username TEXT,\n"
    "    email TEXT,\n"
    "    profile_pic BLOB\n"
    ");\n"
};

// TODO: can we just extract webc_path from db somehow?
bool create_schema(sqlite3 *db, const char *webc_path) {
    bool result = true;
    sqlite3_stmt *stmt = NULL;
    if (!txn_begin(db)) return_defer(false);

    if (sqlite3_exec(db,
            "CREATE TABLE IF NOT EXISTS Migrations (\n"
            "    applied_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,\n"
            "    query TEXT NOT NULL\n"
            ");\n",
            NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

    if (sqlite3_prepare_v2(db, "SELECT query FROM Migrations;", -1, &stmt, NULL)!= SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }

    size_t index = 0;
    int ret = sqlite3_step(stmt);
    for (; ret == SQLITE_ROW; ++index) {
        if (index >= ARRAY_LEN(migrations)) {
            fprintf(stderr, "ERROR: %s: Database scheme is too new. Contains "
                    "more migrations applied than expected. Update your "
                    "application.\n", webc_path);
            return_defer(false);
        }
        const char *query = (const char *)sqlite3_column_text(stmt, 0);
        if (strcmp(query, migrations[index]) != 0) {
            fprintf(stderr, "ERROR: %s: Invalid database scheme. Mismatch in "
                    "migration %zu:\n", webc_path, index);
            fprintf(stderr, "EXPECTED: %s\n", migrations[index]);
            fprintf(stderr, "FOUND: %s\n", query);
            return_defer(false);
        }
        ret = sqlite3_step(stmt);
    }

    if (ret != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        return_defer(false);
    }
    sqlite3_finalize(stmt);
    stmt = NULL;

    for (; index < ARRAY_LEN(migrations); ++index) {
        printf("INFO: %s: applying migration %zu\n", webc_path, index);
        if (WEBC_TRACE_MIGRATION_QUERIES) printf("%s\n", migrations[index]);
        if (sqlite3_exec(db, migrations[index], NULL, NULL, NULL) != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }

        int ret = sqlite3_prepare_v2(db, "INSERT INTO Migrations (query) VALUES (?)", -1, &stmt, NULL);
        if (ret != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }

        if (sqlite3_bind_text(stmt, 1, migrations[index], strlen(migrations[index]), NULL) != SQLITE_OK) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            LOG_SQLITE3_ERROR(db);
            return_defer(false);
        }

        sqlite3_finalize(stmt);
        stmt = NULL;
    }

defer:
    if (stmt) sqlite3_finalize(stmt);
    if (result) result = txn_commit(db);
    return result;
}

sqlite3 *open_webc_db(void) {
    sqlite3 *result = NULL;

    int exists = file_exists(WEBC_DIR_PATH);
    if (exists < 0) return_defer(NULL);
    bool webc_dir_is_symlink = false;
    if (!exists) {
        if (!mkdir_if_not_exists(WEBC_DIR_PATH)) return_defer(NULL);
    } else {
        File_Type type = get_file_type(WEBC_DIR_PATH);
        if (type < 0) return_defer(NULL);
        switch (type) {
        case FILE_DIRECTORY: break;
        case FILE_REGULAR: {
            nob_log(INFO, "%s is a file! Migrating it to a directory...", WEBC_DIR_PATH);
            const char *webc_tmp_db_path = temp_sprintf("%s.tmp", WEBC_DIR_PATH);
            if (!nob_rename(WEBC_DIR_PATH, webc_tmp_db_path)) return_defer(NULL);
            if (!mkdir_if_not_exists(WEBC_DIR_PATH)) return_defer(NULL);
            if (!nob_rename(webc_tmp_db_path, WEBC_DB_PATH)) return_defer(NULL);
        } break;
        case FILE_SYMLINK: {
            webc_dir_is_symlink = true;
        } break;
        case FILE_OTHER: {
            fprintf(stderr, "ERROR: %s is a weird file! We expect it to be a "
                    "directory or a regular file in case of a legacy database...\n", WEBC_DIR_PATH);
            return_defer(NULL);
        } break;
        }
    }

    int ret = sqlite3_open(WEBC_DB_PATH, &result);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "ERROR: %s: %s\n", WEBC_DB_PATH, sqlite3_errstr(ret));
        if (webc_dir_is_symlink) {
            fprintf(stderr, "NOTE: Your %s is a symlink! We used to expect this "
                    "path to lead to an sqlite3 database file, but at some point "
                    "we changed it to a directory. And now the database file is "
                    "expected to be at %s. If you are using some clever symlink "
                    "setup, please update it accordingly so we could open %s as "
                    "the sqlite3 database.\n", WEBC_DIR_PATH, WEBC_DB_PATH, WEBC_DB_PATH);
        }
        return_defer(NULL);
    }

    if (!create_schema(result, WEBC_DB_PATH)) {
        sqlite3_close(result);
        return_defer(NULL);
    }

defer:
    return result;
}
