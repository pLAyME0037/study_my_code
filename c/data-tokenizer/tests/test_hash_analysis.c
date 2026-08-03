#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOB_STRIP_PREFIX
#include "../build/nob.h"
#include "../third_party/hash_table.h"

static int failures = 0;

static bool find_word(const FreqKeyVals *ht, const char *word, size_t *value) {
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->items[i].occupied && strcmp(ht->items[i].word, word) == 0) {
            *value = ht->items[i].value;
            return true;
        }
    }
    return false;
}

#define CHECK(cond, ...)                                                      \
    do {                                                                      \
        if (cond) {                                                           \
            printf("ok: " __VA_ARGS__);                                       \
            printf("\n");                                                     \
        } else {                                                              \
            printf("FAIL: " __VA_ARGS__);                                     \
            printf("\n");                                                     \
            failures++;                                                       \
        }                                                                     \
    } while (0)

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "tests/sample_text.txt";

    Nob_String_Builder sb = {0};
    if (!read_entire_file(path, &sb)) {
        fprintf(stderr, "cannot read %s\n", path);
        return 1;
    }

    FreqKeyVals ht = {0};
    if (!hash_analysis(sb_to_sv(sb), path, &ht)) {
        fprintf(stderr, "hash_analysis failed\n");
        return 1;
    }

    size_t v;

    CHECK(find_word(&ht, "kill", &v) && v == 4, "kill appears 4 times");
    CHECK(find_word(&ht, "bomb", &v) && v == 1, "bomb appears 1 time");
    CHECK(find_word(&ht, "prison", &v) && v == 2, "prison appears 2 times");
    CHECK(find_word(&ht, "dynamite", &v) && v == 1, "dynamite appears 1 time");
    CHECK(find_word(&ht, "shit", &v) && v == 1, "shit appears 1 time");
    CHECK(find_word(&ht, "demon", &v) && v == 1, "demon appears 1 time");
    CHECK(find_word(&ht, "the", &v) && v == 3, "the appears 3 times");
    CHECK(find_word(&ht, "fox", &v) && v == 1, "fox appears 1 time");

    size_t banned = 0;
    size_t flagged_ok = 0;
    for (size_t i = 0; i < ht.capacity; ++i) {
        if (ht.items[i].occupied && ht.items[i].sensitive) {
            banned++;
            if (ht.items[i].pattern != NULL &&
                strcmp(ht.items[i].word, ht.items[i].pattern) == 0) {
                flagged_ok++;
            }
        }
    }
    CHECK(banned == 6, "6 banned words flagged (kill, bomb, prison, dynamite, shit, demon)");
    CHECK(flagged_ok == banned, "every banned word carries its matching pattern");

    freq_keyvals_free(&ht);
    sb_free(sb);

    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    }
    printf("%d FAILURE(S)\n", failures);
    return 1;
}
