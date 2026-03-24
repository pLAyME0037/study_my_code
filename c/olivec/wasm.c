#include <stddef.h>
#include <stdint.h>
#define OLIVE_IMPLEMENTATION
#include "olive.c"

#define WIDTH 800
#define HEIGHT 600
#define PI 3.1415926535897932384626433

uint32_t pixels[WIDTH*HEIGHT];
float angle = 0;
float sqrtf(float x);
float atan2f(float y, float x);
float cosf(float x);
float sinf(float x);

void rotate_point(int *x, int *y) {
    int dx = *x - WIDTH/2;
    int dy = *y - HEIGHT/2;
    float mag = sqrtf(dx * dx + dy * dy);
    float dir = atan2f(dy, dx) + angle;
    *x = cosf(dir) * mag + (float) WIDTH/2;
    *y = sinf(dir) * mag + (float) HEIGHT/2;
}

uint32_t *render(float dt) {
    angle += 0.9f * PI * dt;

    olivec_fill(pixels, WIDTH, HEIGHT, TERCOISE_COLOR);
    {
        int x1 = WIDTH/2,     y1 = HEIGHT/8;
        int x2 = WIDTH*2/8,   y2 = HEIGHT/3;
        int x3 = WIDTH*6/8,   y3 = HEIGHT/2;

        rotate_point(&x1, &y1);
        rotate_point(&x2, &y2);
        rotate_point(&x3, &y3);

        olivec_draw_triangle(pixels, WIDTH, HEIGHT,
                             x1, y1, x2, y2, x3, y3,
                             GREEN_COLOR);
    }
    return pixels;
}
