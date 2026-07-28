#ifdef SDL_PLATFORM
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <math.h>
#endif /* ifdef SDL_PLATFORM */
#include <stddef.h>
#include <stdint.h>
#define OLIVEC_NO_STDLIB
#define OLIVEC_IMPLEMENTATION
#include "olive.c"

float sqrtf(float x);
float atan2f(float y, float x);
float cosf(float x);
float sinf(float x);

// #define RES_FACTOR 6
// #define WIDTH 240*RES_FACTOR
// #define HEIGHT 135*RES_FACTOR
#define WIDTH 800
#define HEIGHT 600
#define PI 3.1415926535897932384626433
#define CIRCLE_RADIUS 6
#define CIRCLE_COLOR 0xFFAA2020
#define GRID_COUNT 10
#define GRID_PAD 0.5/GRID_COUNT
#define GRID_SIZE ((GRID_COUNT - 1)*GRID_PAD)
#define Z_START 0.34

uint32_t circle_colors[] = {
    RED_COLOR,
    GREEN_COLOR,
    BLUE_COLOR,
    YELLOW_COLOR,
    0xFFFF00FF,
    0xFF00FFFF,
    0xFF0000FF,
};
#define CIRCLE_COLORS_COUNT (sizeof(circle_colors)/sizeof(circle_colors[0]))

uint32_t get_width() { return WIDTH; }
uint32_t get_height() { return HEIGHT; }
static uint32_t pixels[WIDTH*HEIGHT];
static float angle = 0;

uint32_t *render(float dt) {
    angle += 0.3*PI*dt;

    Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT);
    olivec_fill(oc, BACKGROUND_COLOR);

    for (int ix = 0; ix < GRID_COUNT; ++ix) {
        for (int iy = 0; iy < GRID_COUNT; ++iy) {
            for (int iz = 0; iz < GRID_COUNT; ++iz) {
                float x = ix*GRID_PAD - GRID_SIZE/2;
                float y = iy*GRID_PAD - GRID_SIZE/2;
                float z = Z_START + iz*GRID_PAD;

                float cx = 0.0;
                float cz = Z_START + GRID_SIZE/2;

                float dx = x - cx;
                float dz = z - cz;

                float ang = atan2f(dz, dx);
                float mag = sqrtf(dx*dx + dz*dz);

                dx = cosf(ang + angle)*mag;
                dz = sinf(ang + angle)*mag;

                x = dx + cx;
                z = dz + cz;

                x /= z;
                y /= z;

                uint32_t r = ix*255/GRID_COUNT;
                uint32_t g = iy*255/GRID_COUNT;
                uint32_t b = iz*255/GRID_COUNT;
                uint32_t colors = 0xFF000000 | (r<<0*8) | (g<<1*8) | (b<<2*8);
                /* uint32_t colors = circle_colors[(ix + iy + iz)%CIRCLE_COLORS_COUNT]; */
                olivec_fill_circle(oc, (x + 1)/2*WIDTH, (y + 1)/2*HEIGHT, CIRCLE_RADIUS, colors);
            }
        }
    }

    size_t size = 16;
    olivec_text(oc, "lol",
                50, HEIGHT - 50 - default_font.height*size, default_font, size,
                FOREGROUND_COLOR);

    return pixels;
}

#define WASM_PLATFORM 0
#define SDL_PLATFORM 1
#define TERM_PLATFORM 2

#if PLATFORM == SDL_PLATFORM

#include <stdio.h>
#include <SDL3/SDL.h>

#define RETURN_DEFER(value) do { result = (value); goto defer; } while (0)

int main(void) {
    int result = 0;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    {
        if (!SDL_Init(SDL_INIT_VIDEO)) RETURN_DEFER(1);

        /* SDL_CreateWindowAndRenderer("Hello, SDL3", WIDTH, HEIGHT, 0, &window, &renderer); */
        window = SDL_CreateWindow("Hello, SDL3", WIDTH, HEIGHT, 0);
        if (window == NULL) RETURN_DEFER(1);

        renderer = SDL_CreateRenderer(window, NULL);
        if (renderer == NULL) RETURN_DEFER(1);

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
        if (texture == NULL) RETURN_DEFER(1);

        Uint32 prev = SDL_GetTicks();
        for (;;) {
            uint64_t curr = SDL_GetTicks();
            float dt = (curr - prev)/1000.f;
            prev = curr;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) RETURN_DEFER(0);
            }

            SDL_Rect window_rect = { 0, 0, WIDTH, HEIGHT };
            uint32_t *pixels = render(dt);

            void *pixels_dst;
            int pitch;
            if (!SDL_LockTexture(texture, &window_rect, &pixels_dst, &pitch)) RETURN_DEFER(0);
            for (size_t y = 0; y < HEIGHT; ++y) {
                memcpy(pixels_dst + y*pitch, pixels + y*WIDTH, WIDTH*sizeof(uint32_t));
            }
            SDL_UnlockTexture(texture);

            /* SDL_UpdateTexture(texture, NULL, pixels, WIDTH*sizeof(uint32_t)); */
            /* SDL_RenderTexture(renderer, texture, NULL, NULL); */

            if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0)) RETURN_DEFER(1);
            if (!SDL_RenderClear(renderer)) RETURN_DEFER(1);
            /* if (SDL_RenderTexture(renderer, texture, &window_rect, &window_rect)) RETURN_DEFER(1); */
            if (!SDL_RenderTexture(renderer, texture, NULL, NULL)) RETURN_DEFER(1);
            SDL_RenderPresent(renderer);
        }

    }

defer:
    switch (result) {
    case 0:
        printf("OK\n");
    break;
    default:
        fprintf(stderr, "SDL Error: %s\n", SDL_GetError());
    }
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();

    return result;
}

#elif PLATFORM == TERM_PLATFORM

#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define SCALE_DOWN_FACTOR 10
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
        usleep(1000*1000/60);

        printf("\033[%dA", SCALE_DOWN_HEIGHT);
        printf("\033[%dD", SCALE_DOWN_WIDTH);
    }

    return 0;
}

#elif PLATFORM == WASM_PLATFORM
// Do Nothing
#else
#error "Unknown Platform"
#endif /* ifdef SDL_PLATFORM */
