#include <graphics/image.h>
#include <kernel.h>

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

void image_drawchar(image_t* img, int x, int y, char c, COLOR fg, COLOR bg, const font_t* font)
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
}