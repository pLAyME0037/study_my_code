#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define OLIVE_NO_STDLIB
#define OLIVE_IMPLEMENTATION
#include "olive.c"

#define WIDTH 800
#define HEIGHT 600

#define COLS (8*2)
#define ROWS (6*2)
#define CELL_WIDTH  (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0x6495EDFF

static uint32_t pixels[WIDTH*HEIGHT];

int main(void) {

    return 0;
}

