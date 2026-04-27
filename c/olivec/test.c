#ifndef TEST_C_
#define  TEST_C_

#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
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

void checkerEx(void);
void circleEx(void);
void lineEx(void);
void fill_triangle(void);
void test_alpha_blending(void);

int main(void) {
    checkerEx();
    circleEx();
    lineEx();
    fill_triangle();
    test_alpha_blending();

    return 0;
}

bool saveFile(const char *file_path) {
    Errno err = olivec_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}

void lineEx(void) {
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    olivec_draw_line(pixels, WIDTH, HEIGHT,
                     0, 0, WIDTH, HEIGHT,
                     FOREGROUND_COLOR);

    olivec_draw_line(pixels, WIDTH, HEIGHT,
                     0, 0, WIDTH/4, HEIGHT,
                     FOREGROUND_COLOR);

    olivec_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH, 0, 0, HEIGHT,
                     FOREGROUND_COLOR);

    olivec_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH/4, 0, 0, HEIGHT,
                     FOREGROUND_COLOR);

    olivec_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH, 0, WIDTH/4*3, HEIGHT,
                     FOREGROUND_COLOR);

    olivec_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH/4*3, 0, WIDTH, HEIGHT,
                     FOREGROUND_COLOR);

    olivec_draw_line(pixels, WIDTH, HEIGHT,
                     0, HEIGHT/2, WIDTH, HEIGHT/2,
                     0xFF20FF20);

    olivec_draw_line(pixels, WIDTH, HEIGHT,
                     WIDTH/2, 0, WIDTH/2, HEIGHT,
                     0xFFFF3030);

    saveFile("sample_output/line_example.ppm");
}

void checkerEx(void) {
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    for (int x = 0; x < COLS; ++x) {
        for (int y = 0; y < ROWS; ++y) {
            uint32_t color = BACKGROUND_COLOR;
            if ((x + y) % 2 == 0) {
                color = 0xFF0000FF;
            }
            olivec_fill_rect(pixels, WIDTH, HEIGHT,
                             x*CELL_WIDTH, y*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT,
                             color);
        }
    }

    saveFile("sample_output/checker_example.ppm");
}

void circleEx(void) {
    Olivec_Canvas canvas = Olivec_make_canvas(pixels, WIDTH, HEIGHT);
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    for (int x = 0; x < COLS; ++x) {
        for (int y = 0; y < ROWS; ++y) {
            float u = (float) x/COLS;
            float v = (float) y/ROWS;
            float t = (float) (u + v)/2;

            size_t radius = CELL_WIDTH;
            if (CELL_HEIGHT < radius) radius = CELL_HEIGHT;
            int cx = x*CELL_WIDTH + CELL_WIDTH/2;
            int cy = y*CELL_HEIGHT + CELL_HEIGHT/2;
            int r = radius/8 + (radius/2 - radius/8)*(int)(t*255)/255;
            olivec_fill_circle(canvas, cx, cy, r, FOREGROUND_COLOR);
        }
    }

    saveFile("sample_output/circle.ppm");
}

void fill_triangle(void) {
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

    saveFile("sample_output/test_triangle.ppm");
}

void test_alpha_blending(void) {
    Olivec_Canvas canvas = Olivec_make_canvas(pixels, WIDTH, HEIGHT);
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);
    olivec_fill_rect(pixels, WIDTH, HEIGHT, 0, 0, WIDTH*3/4, HEIGHT*3/4, RED_COLOR);
    olivec_fill_rect(pixels, WIDTH, HEIGHT, WIDTH/4, HEIGHT/4, WIDTH*3/4, HEIGHT*3/4, 0x5500FF00);
    olivec_fill_circle(canvas, WIDTH/2, HEIGHT/2, WIDTH/4, 0x55AA2020);
    olivec_draw_triangle(pixels, WIDTH, HEIGHT, 0, HEIGHT, WIDTH, HEIGHT, WIDTH/2, 0, 0x9920AAAA);

    saveFile("sample_output/alpha_blending.png");
}

#endif /* ifndef TEST_C_ #define  TEST_C_ */
