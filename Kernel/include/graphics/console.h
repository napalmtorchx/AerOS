#pragma once
#include <lib/types.h>
#include <lib/numerics/point.h>
#include <lib/numerics/rect.h>
#include <graphics/color.h>
#include <graphics/image.h>

typedef struct
{
    image_t img;
    font_t* font;
    point_t size;
    point_t cursor;
    COLOR   fg, bg;
} console_t;

console_t console_create(image_t img, font_t* font, COLOR fg, COLOR bg);