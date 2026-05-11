#define GREEN_COLOR 0xFF00FF00
#define RED_COLOR 0xFF0000FF
#define SKYBLUE_COLOR 0x00FFFF00
#define BLUE_COLOR 0x00AA2020
#define YELLOW_COLOR 0x0000FFFF
#define GRAY_COLOR 0x00808080
#define TERCOISE_COLOR 0x80808000

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0x6495EDFF

typedef int Errno;

#ifndef OLIVECDEF
#define OLIVECDEF static inline
#endif

typedef struct Olivec_Canvas Olivec_Canvas;

void olivec_fill(Olivec_Canvas oc, uint32_t color);
void olivec_fill_rect(Olivec_Canvas oc, int x0, int y0, size_t w, size_t h, uint32_t color);
OLIVECDEF void olivec_blend_color(uint32_t *c1, uint32_t c2);
OLIVECDEF void olivec_fill_circle(Olivec_Canvas oc, int cx, int cy, int r, uint32_t color);
void olivec_draw_line(Olivec_Canvas oc, int x1, int y1, int x2, int y2, uint32_t color);
void olivec_draw_triangle(Olivec_Canvas oc,
                          int x1, int y1,
                          int x2, int y2,
                          int x3, int y3,
                          uint32_t color);
Errno olivec_save_to_ppm_file(Olivec_Canvas oc, const char *file_path);
// abstract dependency
void olivec_sort_tri_points_by_y(int *x1, int *y1,
                                 int *x2, int *y2,
                                 int *x3, int *y3);
void swap_int(int *a, int *b);

