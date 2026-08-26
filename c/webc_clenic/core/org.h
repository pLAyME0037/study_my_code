#ifndef CORE_ORG_CONTROLLER_H_
#define CORE_ORG_CONTROLLER_H_

#include "serve.h"

// Organization Daily Invoice (header + illness detail lines)
void serve_org_daily_list(Serve_Context *sc);
void serve_org_daily_create(Serve_Context *sc);
void serve_org_daily_edit(Serve_Context *sc, int id);
void serve_org_daily_update(Serve_Context *sc, int id);
void serve_org_daily_delete(Serve_Context *sc, int id);
void serve_org_daily_details(Serve_Context *sc, int id);
void serve_org_daily_detail_create(Serve_Context *sc, int id);
void serve_org_daily_detail_delete(Serve_Context *sc, int id, int ill_id);

// Organization Balance (composite PK: org + invoice)

// Organization Invoice Out (header + linked org-invoice detail rows)
void serve_org_out_list(Serve_Context *sc);
void serve_org_out_create(Serve_Context *sc);
void serve_org_out_edit(Serve_Context *sc, int id);
void serve_org_out_update(Serve_Context *sc, int id);
void serve_org_out_delete(Serve_Context *sc, int id);
void serve_org_out_details(Serve_Context *sc, int id);
void serve_org_out_detail_create(Serve_Context *sc, int id);
void serve_org_out_detail_delete(Serve_Context *sc, int id, int invoice_id);

#endif // CORE_ORG_CONTROLLER_H_
