#include <stddef.h>
#include <stdint.h>
#define OLIVE_IMPLEMENTATION
#include "olive.c"

#define WIDTH 800
#define HEIGHT 600

uint32_t pixels[WIDTH*HEIGHT];

uint32_t *render(void) {
    olivec_fill(pixels, WIDTH, HEIGHT, TERCOISE_COLOR);
    {
        int x1 = WIDTH/2,     y1 = HEIGHT/8;
        int x2 = WIDTH*2/8,   y2 = HEIGHT/2;
        int x3 = WIDTH*6/8,   y3 = HEIGHT/2;
        olivec_draw_triangle(pixels, WIDTH, HEIGHT,
                             x1, y1, x2, y2, x3, y3,
                             GREEN_COLOR);
    }
    return pixels;
}
