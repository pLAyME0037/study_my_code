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

bool fill_triangle2(void);

int main(void) {
    if (! fill_triangle2()) return -1;

    return 0;
}

bool fill_triangle2(void) {
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    {
        int x1 = WIDTH/2,   y1 = HEIGHT/8;
        int x2 = WIDTH/8,   y2 = HEIGHT/2;
        int x3 = WIDTH*7/8, y3 = HEIGHT*7/8;
        olivec_draw_triangle(pixels, WIDTH, HEIGHT, x1, y1, x2, y2, x3, y3, YELLOW_COLOR);
    }

    {
        int x1 = WIDTH/2,   y1 = HEIGHT/8;
        int x2 = WIDTH*2/8,   y2 = HEIGHT/2;
        int x3 = WIDTH*6/8, y3 = HEIGHT/2;
        olivec_draw_triangle(pixels, WIDTH, HEIGHT, x1, y1, x2, y2, x3, y3, BLUE_COLOR);
    }

    const char *file_path = "sample_output/triangle.jpg";
    Errno err = olivec_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}
