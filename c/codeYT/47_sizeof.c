#include <stdbool.h>
#include <stdio.h>

typedef struct {
    void *rsp;
    void *stack_base;
    int a;
    bool is_dead;
    bool is_num;
} Generator;

int main() {
    char* name = "me";
    printf("=--------=\n");
    printf("%s\n", name);
    printf("%zu\n", sizeof(Generator));
    printf("%zu\n", sizeof(int));
    printf("%zu\n", sizeof(bool));
    printf("%zu\n", sizeof(char));
    printf("%zu\n", sizeof(char*));

    return 0;
}
