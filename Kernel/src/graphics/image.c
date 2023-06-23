#include <graphics/image.h>
#include <graphics/ssfn.h>
#include <kernel.h>

static char unicode_buff[64];

image_t image_create(int w, int h)
{
    if (w == 0 || h == 0) { debug_error("image_create(%u, %u) - Invalid size", w, h); return (image_t){ 0 }; }

    image_t img = (image_t){ w, h, (COLOR*)malloc(w * h * 4) };
    return img;
}

void image_fill(image_t* img, int x, int y, int w, int h, COLOR color)
{
    int xx, yy, i;
    for (i = 0; i < w * h; i++)
    {
        yy = y + (i / w);
        if (yy >= img->h) { return; }

        xx = x + (i % w);
        if (xx >= img->w) { continue; }

        img->buffer[yy * img->w + xx] = color;
    }
}

#define MAX_UTF8_LENGTH 5

void intToUtf8(int32_t codePoint, char** utf8Char, uint8_t* length) 
{
    static char utf8Buffer[MAX_UTF8_LENGTH + 1];

    if (codePoint <= 0x7F) {
        utf8Buffer[0] = (char)(codePoint & 0xFF);
        utf8Buffer[1] = '\0';
        *utf8Char = utf8Buffer;
        *length = 1;
    } else if (codePoint <= 0x7FF) {
        utf8Buffer[0] = (char)(((codePoint >> 6) & 0x1F) | 0xC0);
        utf8Buffer[1] = (char)((codePoint & 0x3F) | 0x80);
        utf8Buffer[2] = '\0';
        *utf8Char = utf8Buffer;
        *length = 2;
    } else if (codePoint <= 0xFFFF) {
        utf8Buffer[0] = (char)(((codePoint >> 12) & 0x0F) | 0xE0);
        utf8Buffer[1] = (char)(((codePoint >> 6) & 0x3F) | 0x80);
        utf8Buffer[2] = (char)((codePoint & 0x3F) | 0x80);
        utf8Buffer[3] = '\0';
        *utf8Char = utf8Buffer;
        *length = 3;
    } else if (codePoint <= 0x10FFFF) {
        utf8Buffer[0] = (char)(((codePoint >> 18) & 0x07) | 0xF0);
        utf8Buffer[1] = (char)(((codePoint >> 12) & 0x3F) | 0x80);
        utf8Buffer[2] = (char)(((codePoint >> 6) & 0x3F) | 0x80);
        utf8Buffer[3] = (char)((codePoint & 0x3F) | 0x80);
        utf8Buffer[4] = '\0';
        *utf8Char = utf8Buffer;
        *length = 4;
    } else {
        // Invalid code point
        *utf8Char = NULL;
        *length = 0;
    }
}

void image_drawchar(image_t* img, int x, int y, int c, COLOR fg, COLOR bg, const font_t* font)
{
    if (font->psf != NULL)
    {
        int fh = font_getsz(font, false).y;
        uint8_t* glyphs = (uint8_t*)((uintptr_t)font->psf + sizeof(psf_hdr_t));

        if (((bg & 0xFF000000) >> 24) > 0) { image_fill(img, x, y, font_getsz(font, true).x, font_getsz(font, true).y, bg); }

        int xx = x;
        for (int j = 0; j < fh; j++)
        {
            int glyph = glyphs[c * fh + j];
            for (int i = 0; i < 8; i++)
            {
                if ((glyph & 0x80) >= 1) { img->buffer[y * img->w + xx] = fg; }
                glyph <<= 1;
                xx++;
            }
            y++;
            xx = x;
        }
    }
    else if (font->ssfn != NULL)
    {
        char str[2] = { c, 0 };
        image_drawstr(img, x, y, str, fg, bg, font);
    }
}

void image_drawstr(image_t* img, int x, int y, const char* str, COLOR fg, COLOR bg, const font_t* font)
{
     int xx = x, yy = y;
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i)
    {
        if (str[i] == '\n')
        {
            xx = x;
            yy += font->charsz.y + font->spacing.y;
        }
        else
        {
            ssfn_src     = font->ssfn;
            ssfn_dst.ptr = (uint32_t*)img->buffer;
            ssfn_dst.p   = (uint16_t)img->w * 4;
            ssfn_dst.fg  = fg;
            ssfn_dst.bg  = bg;
            ssfn_dst.x   = xx;
            ssfn_dst.y   = yy;

            uint32_t unicode;
            const char* cstr = &str[i];
            while ((*cstr & 0xC0) == 0x80) { ++cstr; }
            size_t seq_len = cstr - &str[i];
            
            unicode = ssfn_utf8(&cstr);
            if (unicode != 0) { ssfn_putc(unicode); }

            xx += font->charsz.x + font->spacing.x;
            i += seq_len;
        }
    }
}