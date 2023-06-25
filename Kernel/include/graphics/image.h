#pragma once
#include <lib/types.h>
#include <lib/numerics/point.h>
#include <graphics/color.h>
#include <graphics/font.h>

typedef struct image_t image_t;
struct image_t
{
    point_t    sz;
    void*      buffer;
    COLORDEPTH bpp;
    COLORORDER color_order;
    bool       is_malloc;
    uint32_t   pitch;
};

#define NULL_IMAGE (image_t){ { 0, 0 }, NULL, 0, 0, false, 0 }

image_t image_create(int w, int h, COLORDEPTH bpp, COLORORDER color_order);
image_t image_create_from(int w, int h, void* data, COLORDEPTH bpp, COLORORDER color_order);
void image_free(image_t* img);

void image_clear(image_t* img, argb_t color);
void image_blit(image_t* img, int x, int y, argb_t color);
void image_fill_rect(image_t* img, int x, int y, int w, int h, argb_t color);
void image_draw_rect(image_t* img, int x, int y, int w, int h, int thickness, argb_t color);
void image_drawchar(image_t* img, int x, int y, char c, argb_t fg, argb_t bg, const font_t* font);
void image_drawstr(image_t* img, int x, int y, const char* str, argb_t fg, argb_t bg, const font_t* font);
argb_t image_getpixel(image_t* img, int x, int y);