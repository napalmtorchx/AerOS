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

void image_clear(image_t* img, COLOR color)
{
    if (img == NULL || img->buffer == NULL) { return; }   
    memset(img->buffer, color, img->w * img->h * 4);
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