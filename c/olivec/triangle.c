#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <stdint.h>
#define OLIVEC_NO_STDLIB
#define OLIVEC_IMPLEMENTATION
#include "olive.c"

#define WIDTH 800
#define HEIGHT 600
#define PI 3.1415926535897932384626433
#define CIRCLE_RADIUS 100
#define CIRCLE_COLOR 0x88AA2020

static uint32_t pixels[WIDTH*HEIGHT];
static float triangleAngle = 0;
static int circle_x = WIDTH/2;
static int circle_y = HEIGHT/2;
static int circle_offset_x = 180;
static int circle_offset_y = 0;

float sqrtf(float x);
float atan2f(float y, float x);
float cosf(float x);
float sinf(float x);

void rotate_point(int *x, int *y) {
    int dx = *x - WIDTH/2;
    int dy = *y - HEIGHT/2;
    float mag = sqrtf(dx * dx + dy * dy);
    float dir = atan2f(dy, dx) + triangleAngle;
    *x = cosf(dir) * mag + (float) WIDTH/2;
    *y = sinf(dir) * mag + (float) HEIGHT/2;
}

uint32_t *render(float dt) {
    triangleAngle += 0.9f * PI * dt;

    Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT);
    olivec_fill(oc, TERCOISE_COLOR);
    {
        int x1 = WIDTH/2,     y1 = HEIGHT/8;
        int x2 = WIDTH*2/8,   y2 = HEIGHT/3;
        int x3 = WIDTH*6/8,   y3 = HEIGHT/2;

        rotate_point(&x1, &y1);
        rotate_point(&x2, &y2);
        rotate_point(&x3, &y3);

        olivec_draw_triangle(oc, x1, y1, x2, y2, x3, y3, GREEN_COLOR);
    }
    {
        int cx = circle_x + circle_offset_x;
        int cy = circle_y + circle_offset_y;

        rotate_point(&cx, &cy);

        olivec_fill_circle(oc, cx, cy, CIRCLE_RADIUS, CIRCLE_COLOR);
    }
    {
        int cx = circle_x + circle_offset_x/2;
        int cy = circle_y + circle_offset_y;

        rotate_point(&cx, &cy);

        olivec_fill_circle(oc, cx, cy, CIRCLE_RADIUS, 0xAAFF12FF);
    }
    return pixels;
}

#define WASM_PLATFORM 0
#define SDL_PLATFORM 1
#define TERM_PLATFORM 2

#if PLATFORM == SDL_PLATFORM
#include <SDL3/SDL.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_CreateWindowAndRenderer("Hello, SDL3", WIDTH, HEIGHT, 0, &window, &renderer);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    uint64_t prev = SDL_GetTicks();
    bool quit = false;
    while (!quit) {
        uint64_t now = SDL_GetTicks();
        float dt = (now - prev)/1000.0f;
        prev = now;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) { quit = true; }
        }

        uint32_t *pixels = render(dt);
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH*sizeof(uint32_t));
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        /* SDL_SetRenderDrawColor(renderer, 20, 40, 100, 255); */
        /* SDL_RenderClear(renderer); */
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
#elif PLATFORM == TERM_PLATFORM

#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define SCALE_DOWN_FACTOR 20
static_assert(WIDTH%SCALE_DOWN_FACTOR == 0, "WIDTH must be devicible by the SCALE_DOWN_FACTOR");
#define SCALE_DOWN_WIDTH (WIDTH/SCALE_DOWN_FACTOR)
static_assert(HEIGHT%SCALE_DOWN_FACTOR == 0, "HEIGHT must be devicible by the SCALE_DOWN_FACTOR");
#define SCALE_DOWN_HEIGHT (HEIGHT/SCALE_DOWN_FACTOR)

char char_canvas[SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT];

char color_to_char(uint32_t pixel) {
    char table[] = " .:a@#";
    size_t n = sizeof(table) - 1;
    size_t r = (0x000000FF&pixel)>>(8*0);
    size_t g = (0x0000FF00&pixel)>>(8*1);
    size_t b = (0x00FF0000&pixel)>>(8*2);
    size_t bright = r;
    if (bright < g) bright = g;
    if (bright < b) bright = b;
    return table[bright*n/256];
}

uint32_t compress_pixel_chunk(Olivec_Canvas oc) {
    size_t r = 0;
    size_t g = 0;
    size_t b = 0;
    size_t a = 0;

    for (size_t x = 0; x < oc.width; ++x) {
        for (size_t y = 0; y < oc.height; ++y) {
            r += OLIVEC_RED(OLIVEC_PIXEL(oc, x, y));
            g += OLIVEC_GREEN(OLIVEC_PIXEL(oc, x, y));
            b += OLIVEC_BLUE(OLIVEC_PIXEL(oc, x, y));
            a += OLIVEC_ALPHA(OLIVEC_PIXEL(oc, x, y));
        }
    }

    r /= oc.width*oc.height;
    g /= oc.width*oc.height;
    b /= oc.width*oc.height;
    a /= oc.width*oc.height;

    return OLIVEC_RGBA(r, g, b, a);
}

void compress_pixel(uint32_t *pixel) {
    Olivec_Canvas oc = olivec_canvas(pixel, WIDTH, HEIGHT);

    for (size_t x = 0; x < SCALE_DOWN_WIDTH; ++x) {
        for (size_t y = 0; y < SCALE_DOWN_HEIGHT; ++y) {
            Olivec_Canvas soc = olivec_subcanvas(oc,
                                                 x*SCALE_DOWN_FACTOR,
                                                 y*SCALE_DOWN_FACTOR,
                                                 SCALE_DOWN_FACTOR,
                                                 SCALE_DOWN_FACTOR);
            char_canvas[y*SCALE_DOWN_WIDTH + x] = color_to_char(compress_pixel_chunk(soc));
        }
        putc('\n', stdout);
    }
}

size_t get_curr_ms(void) {
    struct timespec tp;
    if (clock_gettime(CLOCK_MONOTONIC, &tp) < 0) {
        fprintf(stderr, "ERROR: Could not get clock: %s", strerror(errno));
        exit(1);
    }
    return tp.tv_nsec/1000/1000 + tp.tv_nsec/1000;
}

int main(void) {
    for (;;) {
        // size_t curr_ms = get_curr_ms();
        // float delta_time = (curr_ms - prev_ms)*0.001f;
        // prev_ms = curr_ms;
        compress_pixel(render(1.f/60));
        for (size_t y = 0; y < SCALE_DOWN_HEIGHT; ++y) {
            for (size_t x = 0; x < SCALE_DOWN_WIDTH; ++x) {
                putc(char_canvas[y*SCALE_DOWN_WIDTH + x], stdout);
                putc(char_canvas[y*SCALE_DOWN_WIDTH + x], stdout);
            }
            putc('\n', stdout);
        }
        printf("\033[%dA", SCALE_DOWN_HEIGHT);
        printf("\033[%dD", SCALE_DOWN_WIDTH);

        usleep(1000*1000/60);
    }

    return 0;
}

#elif PLATFORM == WASM_PLATFORM
// Do Nothing
#else
#error "Unknown Platform"
#endif /* PLATFORM */
