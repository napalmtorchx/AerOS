#pragma once
#include <lib/types.h>
#include <graphics/color.h>
#include <graphics/font.h>

typedef struct image_t image_t;
struct image_t
{
    int w, h;
    COLOR* buffer;
};

image_t image_create(int w, int h);
void image_clear(image_t* img, COLOR color);
void image_fill(image_t* img, int x, int y, int w, int h, COLOR color);
void image_drawstr(image_t* img, int x, int y, const char* str, COLOR fg, COLOR bg, const font_t* font);