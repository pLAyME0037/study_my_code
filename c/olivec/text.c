#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define OLIVEC_NO_STDLIB
#define OLIVEC_IMPLEMENTATION
#include "olive.c"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./tpd/stb_image_write.h"

#define WIDTH 800
#define HEIGHT 600

static uint32_t pixels[WIDTH*HEIGHT];

int main(void) {
    Olivec_Canvas oc = Olivec_make_canvas(pixels, WIDTH, HEIGHT);
    olivec_fill(oc, BACKGROUND_COLOR);
    olivec_text(oc, "the quick brown fox jumbs over the lazy dog",
                0, 0, default_font, 4,
                FOREGROUND_COLOR);

    const char *file_path = "./sample_output/text.png";
    if (!stbi_write_png(file_path, WIDTH, HEIGHT, 4, pixels, WIDTH*sizeof(uint32_t))) {
        fprintf(stderr, "could not write file %s: %s\n", file_path, strerror(errno));
        return 1;
    }

    return 0;
}

