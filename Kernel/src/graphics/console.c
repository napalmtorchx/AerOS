#include <graphics/console.h>
#include <kernel.h>

console_t console_create(image_t img, font_t* font, COLOR fg, COLOR bg)
{
    if (img.buffer == NULL) { debug_error("console_create(%p, %p, %p, %p) - Null buffer pointer", img, font, fg, bg); return (console_t){ 0 }; }

    point_t fontsz = font_getsz(font, true);

    console_t console = (console_t)
    {
        .img    = img,
        .font   = font,
        .size   = (point_t){ img.w / fontsz.x, img.h / fontsz.y },
        .cursor = (point_t){ 0, 0 },
        .fg     = fg,
        .bg     = bg,
    };

    return console;
}