#ifndef CORE_PATIENT_INVOICE_CONTROLLER_H_
#define CORE_PATIENT_INVOICE_CONTROLLER_H_

#include "serve.h"

// Patient Medicine Invoice (header + details)
void serve_pmi_list(Serve_Context *sc);
void serve_pmi_create(Serve_Context *sc);
void serve_pmi_update(Serve_Context *sc, int id);
void serve_pmi_delete(Serve_Context *sc, int id);

// Patient Invoice Out (header + details)
void serve_pio_list(Serve_Context *sc);
void serve_pio_create(Serve_Context *sc);
void serve_pio_update(Serve_Context *sc, int id);
void serve_pio_delete(Serve_Context *sc, int id);

#endif // CORE_PATIENT_INVOICE_CONTROLLER_H_
