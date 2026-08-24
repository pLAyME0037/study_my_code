#define NOB_STRIP_PREFIX
#include "../../nob.h"

#include "master_detail.h"
#include "../src/db/db.h"
#include "../core/header.h"
#include "../core/footer.h"
#include "../core/http/utils.h"

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
            for (size_t ci = 0; row->children[ci].items != NULL && ci < 10; ++ci) {
                MD_ChildRows *crows = &row->children[ci];
                for (size_t j = 0; j < crows->count; ++j) {
                    free(crows->items[j].values);
                }
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

static bool load_child_rows(sqlite3           *db,
                            const MD_ChildTab *child,
                            long long          master_id,
                            MD_ChildRows      *out_rows)
{
    char *sql = temp_sprintf("SELECT * FROM %s WHERE %s = %lld ORDER BY %s DESC;",
                            child->table, child->fk_column, master_id, child->id_column);

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
        for (size_t ci = 0; ci < config->children_count; ++ci) {
            row.children[ci].items = NULL;
            row.children[ci].count = 0;
            row.children[ci].capacity = 0;
            if (!load_child_rows(db, &config->children[ci], row.id, &row.children[ci])) {
                for (int i = 0; i < col_count; ++i) free(row.values[i]);
                free(row.values);
                for (size_t ci2 = 0; ci2 <= ci; ++ci2) {
                    MD_ChildRows *cr = &row.children[ci2];
                    for (size_t j = 0; j < cr->count; ++j) {
                        for (size_t k = 0; k < cr->items[j].value_count; ++k) {
                            free(cr->items[j].values[k]);
                        }
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
    const MD_MasterConfig *cfg = config;

    sb->count = 0;
    render_page_header(sb, cfg->title, cfg->table);

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
