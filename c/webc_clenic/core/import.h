#ifndef CORE_IMPORT_CONTROLLER_H_
#define CORE_IMPORT_CONTROLLER_H_

#include "serve.h"

void serve_import_list(Serve_Context *sc);
void serve_import_create(Serve_Context *sc);
void serve_import_edit(Serve_Context *sc, int id);
void serve_import_update(Serve_Context *sc, int id);
void serve_import_delete(Serve_Context *sc, int id);
void serve_import_details(Serve_Context *sc, int id);
void serve_import_detail_create(Serve_Context *sc, int id);
void serve_import_detail_delete(Serve_Context *sc, int id, int medicine_id);

#endif // CORE_IMPORT_CONTROLLER_H_
