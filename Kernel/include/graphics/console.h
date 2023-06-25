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
    argb_t  fg, bg;
    char*   printbuff;
    size_t  printbuff_sz;
} console_t;

console_t kconsole_init();
console_t console_create(image_t img, font_t* font, argb_t fg, argb_t bg, size_t printbuff_sz);
void console_free(console_t* console);

void console_drawstr(console_t* console, int x, int y, const char* str, argb_t fg, argb_t bg);

void console_clear(console_t* console);
void console_clearc(console_t* console, argb_t color);
void console_write(console_t* console, const char* str);
void console_writeln(console_t* console, const char* str);
void console_vprintf(console_t* console, const char* fmt, va_list args);
void console_printf(console_t* console, const char* fmt, ...);

void console_newline(console_t* console);
void console_delete(console_t* console, int chars_to_del);
void console_scroll(console_t* console, int lines);
void console_setpos(console_t* console, point_t pos);
void console_setfg(console_t* console, argb_t fg);
void console_setbg(console_t* console, argb_t bg);
void console_setfont(console_t* console, const font_t* font);

point_t console_getsz(console_t* console);
point_t console_getpos(console_t* console);
argb_t console_getfg(console_t* console);
argb_t console_getbg(console_t* console);
font_t* console_getfont(console_t* console);