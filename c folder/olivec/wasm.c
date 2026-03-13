#include <stddef.h>
#include <stdint.h>
#include "olive.c"

#define WIDTH 800
#define HEIGHT 600

uint32_t pixels[WIDTH*HEIGHT];

uint32_t *render(void) {
    olivec_fill(pixels, WIDTH, HEIGHT, 0xFF00FF00);
    return pixels;
}
