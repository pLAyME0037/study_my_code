#ifndef CORE_STOCK_CONTROLLER_H_
#define CORE_STOCK_CONTROLLER_H_

#include "serve.h"

void serve_stock_list(Serve_Context *sc);
void serve_stock_create(Serve_Context *sc);
void serve_stock_edit(Serve_Context *sc, int id);
void serve_stock_update(Serve_Context *sc, int id);
void serve_stock_delete(Serve_Context *sc, int id);

#endif // CORE_STOCK_CONTROLLER_H_
