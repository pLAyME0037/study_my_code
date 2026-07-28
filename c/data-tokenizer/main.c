#include "third_party/hash_table.h"
#include "third_party/json_format.h"

int main(void) {
    SensitiveReport report = {0};

    if (!find_sensetive_data("./data_2mb.json", &report)) return 1;

    confirm_sensitive_fields(&report);
    print_report(&report);

    // TODO: pass &report to hash_analysis for indexing
    // String_View content = ...;
    // hash_analysis(content, &report);

    free(report.items);
    return 0;
}
