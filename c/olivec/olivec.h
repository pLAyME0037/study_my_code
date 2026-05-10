typedef int Errno;

#define GREEN_COLOR 0xFF00FF00
#define RED_COLOR 0xFF0000FF
#define SKYBLUE_COLOR 0x00FFFF00
#define BLUE_COLOR 0x00AA2020
#define YELLOW_COLOR 0x0000FFFF
#define GRAY_COLOR 0x00808080
#define TERCOISE_COLOR 0x80808000

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0x6495EDFF

#ifndef OLIVECDEF
#define OLIVECDEF static inline
#endif

void olivec_fill(uint32_t *pixels, size_t width, size_t height, uint32_t color);
void olivec_fill_rect(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
                      int x0, int y0, size_t w, size_t h,
                      uint32_t color);

typedef struct Olivec_Canvas Olivec_Canvas;

OLIVECDEF void olivec_blend_color(uint32_t *c1, uint32_t c2);
OLIVECDEF void olivec_fill_circle(Olivec_Canvas oc,
                                  int cx, int cy, int r,
                                  uint32_t color);
void olivec_draw_line(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
                      int x1, int y1, int x2, int y2,
                      uint32_t color);
void olivec_draw_triangle(uint32_t *pixels, size_t width, size_t height,
                          int x1, int y1,
                          int x2, int y2,
                          int x3, int y3,
                          uint32_t color);
Errno olivec_save_to_ppm_file(uint32_t *pixels, size_t width, size_t height, const char *file_path);
// abstract dependency
void olivec_sort_tri_points_by_y(int *x1, int *y1,
                                 int *x2, int *y2,
                                 int *x3, int *y3);
void swap_int(int *a, int *b);

