#include <graphics/console.h>
#include <graphics/ssfn.h>
#include <kernel.h>

console_t kconsole_init()
{
    vbe_device_t* vbe = devmgr_from_name("vbe_controller");

    image_t   console_img = image_create_from(vbe->w, vbe->h, vbe->fbptr, COLORDEPTH_32, COLORORDER_ARGB);
    console_t console     = console_create(console_img, sysfont_get(), color_to_argb(COLOR_WHITE), color_to_argb(COLOR_BLACK), 64 * KILOBYTE);
    return console;
}

console_t console_create(image_t img, font_t* font, argb_t fg, argb_t bg, size_t printbuff_sz)
{
    if (img.buffer == NULL) { debug_error("console_create(%p, %p, %p, %p) - Null buffer pointer", img, font, fg, bg); return (console_t){ 0 }; }

    point_t fontsz = font_getsz(font, true);

    console_t console = (console_t)
    {
        .img    = img,
        .font   = font,
        .size   = (point_t){ img.sz.x / fontsz.x, img.sz.y / fontsz.y },
        .cursor = (point_t){ 0, 0 },
        .fg     = fg,
        .bg     = bg,
    };

    console.printbuff_sz = printbuff_sz;
    if (printbuff_sz == 0) { console.printbuff = NULL; }
    else { console.printbuff = (char*)malloc(console.printbuff_sz); }

    return console;
}

void console_free(console_t* console)
{
    if (console == NULL) { debug_error("console_free(%p) - Null pointer", console); return; }
    if (console->printbuff != NULL) { free(console->printbuff); }
}

void console_drawstr(console_t* console, int x, int y, const char* str, argb_t fg, argb_t bg)
{
    if (console == NULL) { return; }    

    point_t fontsz = font_getsz(console->font, true);
    int xx = console->cursor.x * fontsz.x;
    int yy = console->cursor.y * fontsz.y;
    if ((uint32_t)xx >= console->img.sz.x || (uint32_t)yy >= console->img.sz.y) { return; }
    image_drawstr(&console->img, xx, yy, str, fg, bg, console->font);
}

void console_clear(console_t* console)
{
    if (console == NULL) { return; }    
    image_clear(&console->img, console->bg);
    console_setpos(console, (point_t){ 0, 0 });
}

void console_clearc(console_t* console, argb_t color)
{
    if (console == NULL) { return; }    
    console->bg = color;
    image_clear(&console->img, color);
    console_setpos(console, (point_t){ 0, 0 });
}

void console_write(console_t* console, const char* str)
{
    if (console == NULL) { return; }

    point_t fontsz = font_getsz(console->font, true);
    int xx = console->cursor.x * fontsz.x;
    int yy = console->cursor.y * fontsz.y;
    argb_t ofg = console->fg, obg = console->bg;

    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i)
    {
        if (str[i] == '\n')
        {
            console_newline(console);
            xx = console->cursor.x * fontsz.x;
            yy = console->cursor.y * fontsz.y;
        }
        else if (str[i] == '\x1b')
        {
            i++;
            if (str[i] == '[' && (str[i + 2] == 'm' || str[i + 3] == 'm' || str[i + 4] == 'm'))
            {
                i++;
                char code[4] = { 0, 0, 0, 0 };
                int j = 0;
                while (str[i] != 'm')
                {
                    code[j++] = str[i++];
                }

                uint32_t c = atol(code);
                if (c == 0) { console->fg = ofg; console->bg = obg; }
                else if ((c >= 30 && c <= 37) || (c >= 90  && c <= 97))  { console->fg = color_to_argb(ansi_fg_to_color(c, color_from_argb(ofg))); }
                else if ((c >= 40 && c <= 47) || (c >= 100 && c <= 107)) { console->bg = color_to_argb(ansi_fg_to_color(c, color_from_argb(obg))); }
            }
        }
        else
        {
            ssfn_src     = console->font->ssfn;
            ssfn_dst.ptr = (uint32_t*)console->img.buffer;
            ssfn_dst.p   = (uint16_t)console->img.sz.x * 4;
            ssfn_dst.fg  = color_from_argb(console->fg);
            ssfn_dst.bg  = color_from_argb(console->bg);
            ssfn_dst.x   = xx;
            ssfn_dst.y   = yy;

            uint32_t unicode;
            const char* cstr = &str[i];
            while ((*cstr & 0xC0) == 0x80) { ++cstr; }
            size_t seq_len = cstr - &str[i];
            
            unicode = ssfn_utf8(&cstr);
            if (unicode != 0) { ssfn_putc(unicode); }

            console->cursor.x++;
            xx += fontsz.x;
            i  += seq_len;
        }
    }

    console->fg = ofg;
    console->bg = obg;
}

void console_writeln(console_t* console, const char* str)
{
    if (console == NULL) { return; }
    console_write(console, str);
    console_newline(console);
}

void console_vprintf(console_t* console, const char* fmt, va_list args)
{
    if (console == NULL || console->printbuff == 0) { return; }
    memset(console->printbuff, 0, console->printbuff_sz);
    vsprintf(console->printbuff, fmt, args);
    console_write(console, console->printbuff);
}

void console_printf(console_t* console, const char* fmt, ...)
{
    if (console == NULL) { return; }
    va_list args;
    va_start(args, fmt);
    console_vprintf(console, fmt, args);
    va_end(args);
}

void console_newline(console_t* console)
{
    if (console == NULL) { return; }
    console->cursor.x = 0;
    console->cursor.y++;

    if (console->cursor.y >= console->size.y)
    {
        console_scroll(console, 1);
        console->cursor.x = 0;
        console->cursor.y = console->size.y - 1;
    }
}

void console_delete(console_t* console, int chars_to_del)
{
    while (chars_to_del > 0)
    {
        if (console->cursor.x > 0)
        {
            console->cursor.x--;
            console_drawstr(console, console->cursor.x, console->cursor.y, " ", console->fg, console->bg);
        }
        else if (console->cursor.y > 0) 
        {
            console_setpos(console, (point_t){ console->size.x - 1, console->cursor.y - 1});
            console_drawstr(console, console->cursor.x, console->cursor.y, " ", console->fg, console->bg);
        }
        chars_to_del--;
    }
}

void console_scroll(console_t* console, int lines)
{
    int fh = font_getsz(console->font, true).y;
    uint32_t line = console->img.sz.x * fh * color_bpp_multiplier(console->img.bpp);
    uint32_t size = console->img.sz.x * console->img.sz.y * 4;
    memcpy(console->img.buffer, (uint32_t*)((uint32_t)console->img.buffer + line), size - line);
    image_fill_rect(&console->img, 0, console->img.sz.y - fh, console->img.sz.x, fh, console->bg);
    //memset((uint32_t*)((uint32_t)console->img.buffer + (size - line)), console->bg.value, line);
    console_setpos(console, (point_t){ 0, console->size.y - 1 });
}

void console_setpos(console_t* console, point_t pos)
{
    if (console == NULL) { return; }
    console->cursor = pos;
}

void console_setfg(console_t* console, argb_t fg)
{
    if (console == NULL) { return; }
    console->fg = fg;
}

void console_setbg(console_t* console, argb_t bg)
{
    if (console == NULL) { return; }
    console->bg = bg;
}

void console_setfont(console_t* console, const font_t* font)
{
    if (console == NULL) { return; }
    if (font == NULL) { debug_error("console_setfont(%p, %p) - Attempt to set console font to null", console, font); return; }
    console->font = font;
}

point_t console_getsz(console_t* console)
{
    if (console == NULL) { return (point_t){ 0, 0 }; }
    return console->size;
}

point_t console_getpos(console_t* console)
{
    if (console == NULL) { return (point_t){ 0, 0 }; }
    return console->cursor;
}

argb_t console_getfg(console_t* console)
{
    if (console == NULL) { return (argb_t){ 0 }; }
    return console->fg;
}

argb_t console_getbg(console_t* console)
{
    if (console == NULL) { return (argb_t){ 0 }; }
    return console->bg;
}

font_t* console_getfont(console_t* console)
{
    if (console == NULL) { return NULL; }
    return console->font;
}