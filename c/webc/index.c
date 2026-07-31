#include <stdio.h>
#include <unistd.h>

#define OUT STDOUT_FILENO
#define INT(x) dprintf(STDOUT_FILENO, "%d", x);

void create_html() {
#include "index.h"

}

int main(void) {
    create_html();

    return 0;
}
