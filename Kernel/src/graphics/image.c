#include <graphics/image.h>
#include <graphics/ssfn.h>
#include <kernel.h>

static char unicode_buff[64];

image_t image_create(int w, int h, COLORDEPTH bpp, COLORORDER color_order)
{   
    if (w == 0 || h == 0) { debug_error("image_create(%d, %d, %d, %d) - Invalid size", w, h, bpp, color_order); return NULL_IMAGE; }

    image_t img;
    img.sz          = (point_t){ w, h };
    img.bpp         = bpp;
    img.color_order = color_order;
    img.is_malloc   = true;

    size_t bpp_mult = color_bpp_multiplier(img.bpp);
    if (bpp_mult == 0) { debug_error("image_create(%d, %d, %d, %d) - Invalid size", w, h, bpp, color_order); return NULL_IMAGE; }
    size_t imgsz = w * h * bpp_mult;

    img.pitch  = img.sz.x * bpp_mult;
    img.buffer = malloc(imgsz);
    memset(img.buffer, 0, imgsz);
    return img;
}

image_t image_create_from(int w, int h, void* data, COLORDEPTH bpp, COLORORDER color_order)
{
    image_t img;
    img.sz          = (point_t){ w, h };
    img.bpp         = bpp;
    img.color_order = color_order;
    img.is_malloc   = false;
    img.buffer      = data;
    img.pitch       = img.sz.x * color_bpp_multiplier(img.bpp);
    return img;
}

void image_free(image_t* img)
{
    if (img == NULL) { debug_error("image_free(%p) - Null image pointer"); return; }
    if (img->is_malloc && img->buffer != NULL) { free(img->buffer); }
}

void image_clear(image_t* img, argb_t color)
{
    if (img == NULL) { return; }
    if (img->bpp == COLORDEPTH_4  || img->bpp == COLORDEPTH_8 || 
        img->bpp == COLORDEPTH_16 || img->bpp == COLORDEPTH_24) { debug_error("image_clear(%p, %p) - Unsupported color depth", img, color.value); return; }

    if (img->bpp == COLORDEPTH_32) 
    { 
        COLOR value = 0;
        if (img->color_order == COLORORDER_ARGB)      { value = color_from_argb(color); }
        else if (img->color_order == COLORORDER_ABGR) { value = color_from_abgr(color); }
        else if (img->color_order == COLORORDER_RGBA) { value = color_from_rgba(color); }
        else if (img->color_order == COLORORDER_BGRA) { value = color_from_bgra(color); }
        memset_sse(img->buffer, value, img->sz.x * img->sz.y * 4); 
    }
}

void image_blit(image_t* img, int x, int y, argb_t color)
{
    if (img == NULL) { return; }
    if ((uint32_t)x >= (uint32_t)img->sz.x || (uint32_t)y >= (uint32_t)img->sz.y) { return; }
    if (img->bpp == COLORDEPTH_4  || img->bpp == COLORDEPTH_8 || 
        img->bpp == COLORDEPTH_16 || img->bpp == COLORDEPTH_24) { debug_error("image_blit(%p, %d, %d, %p) - Unsupported color depth", img, x, y, color.value); return; }

    if (img->bpp == COLORDEPTH_32) 
    { 
        COLOR value = 0;
        if (img->color_order == COLORORDER_ARGB) { value = color_from_argb(color); }
        else if (img->color_order == COLORORDER_ABGR) { value = color_from_abgr(color); }
        else if (img->color_order == COLORORDER_RGBA) { value = color_from_rgba(color); }
        else if (img->color_order == COLORORDER_BGRA) { value = color_from_bgra(color); }
        if (color.desc.a != 255) { value = color_blend(value, color_from_argb(image_getpixel(img, x, y)), (float)color.desc.a / 256.0f); }

        ((uint32_t*)img->buffer)[y * img->sz.x + x] = value;
    }
}

void image_fill_rect(image_t* img, int x, int y, int w, int h, argb_t color)
{
    if (img == NULL) { return; }
    if (img->bpp == COLORDEPTH_4  || img->bpp == COLORDEPTH_8 || 
        img->bpp == COLORDEPTH_16 || img->bpp == COLORDEPTH_24) { debug_error("image_fill_rect(%p, %d, %d, %d, %d, %p) - Unsupported color depth", img, x, y, w, h, color.value); return; }

    if (img->bpp == COLORDEPTH_32)
    {
        COLOR value = 0;
        if (img->color_order == COLORORDER_ARGB)      { value = color_from_argb(color); }
        else if (img->color_order == COLORORDER_ABGR) { value = color_from_abgr(color); }
        else if (img->color_order == COLORORDER_RGBA) { value = color_from_rgba(color); }
        else if (img->color_order == COLORORDER_BGRA) { value = color_from_bgra(color); }
        if (color.desc.a != 255) { value = color_blend(value, color_from_argb(image_getpixel(img, x, y)), (float)color.desc.a / 255.0f); }

        int xx, yy, i;
        for (i = 0; i < w * h; i++)
        {
            yy = y + (i / w);
            if (yy >= img->sz.y) { return; }

            xx = x + (i % w);
            if (xx >= img->sz.x) { continue; }

            ((uint32_t*)img->buffer)[yy * img->sz.x + xx] = value;
        }   
    }
}

void image_fill_rectg(image_t* img, int x, int y, int w, int h, GRADIENT_TYPE type, argb_t c1, argb_t c2)
{
    if (img == NULL) { return; }
    if (img->bpp == COLORDEPTH_4  || img->bpp == COLORDEPTH_8 || 
        img->bpp == COLORDEPTH_16 || img->bpp == COLORDEPTH_24) { debug_error("image_fill_rectg(%p, %d, %d, %d, %d, %p, %p) - Unsupported color depth", img, x, y, w, h, c1.value, c2.value); return; }

    if (img->bpp == COLORDEPTH_32)
    {
        COLOR  color1 = color_from_argb(c1);
        COLOR  color2 = color_from_argb(c2);
        argb_t cvals;

        int xx, yy;
        if (type == GRADIENT_XLINEAR)
        {
            for (yy = 0; yy < h; yy++) 
            {
                if (y + yy >= img->sz.y) { break; }
                for (xx = 0; xx < w; xx++) 
                {
                    if (x + xx >= img->sz.x) { continue; }
                    
                    cvals.desc.r = (c1.desc.r * (w - xx) + c2.desc.r * xx) / w;
                    cvals.desc.g = (c1.desc.g * (w - xx) + c2.desc.g * xx) / w;
                    cvals.desc.b = (c1.desc.b * (w - xx) + c2.desc.b * xx) / w;
                    cvals.desc.a = (c1.desc.a * (w - xx) + c2.desc.a * xx) / w;

                    COLOR color = color_from_argb(cvals);
                    if (cvals.desc.a != 255) { color = color_blend(color, color_from_argb(image_getpixel(gfx_backbuffer(), x + xx, y + yy)), (float)cvals.desc.a / 255.0f); }
                    ((uint32_t*)img->buffer)[(y + yy) * img->sz.x + (x + xx)] = color;
                }
            }
        }
        else if (type == GRADIENT_YLINEAR)
        {
            bool done = false;
            for (xx = 0; xx < w; xx++) 
            {
                if (done) { break; }
                if (x + xx >= img->sz.x) { continue; }

                for (yy = 0; yy < h; yy++) 
                {
                    if (y + yy >= img->sz.y) { done = true; break; }
                    
                    cvals.desc.r = (c1.desc.r * (h - yy) + c2.desc.r * yy) / h;
                    cvals.desc.g = (c1.desc.g * (h - yy) + c2.desc.g * yy) / h;
                    cvals.desc.b = (c1.desc.b * (h - yy) + c2.desc.b * yy) / h;
                    cvals.desc.a = (c1.desc.a * (h - yy) + c2.desc.a * yy) / h;

                    COLOR color = color_from_argb(cvals);
                    if (cvals.desc.a != 255) { color = color_blend(color, color_from_argb(image_getpixel(gfx_backbuffer(), x + xx, y + yy)), (float)cvals.desc.a / 255.0f); }
                    ((uint32_t*)img->buffer)[(y + yy) * img->sz.x + (x + xx)] = color;
                }
            }
        }
    }
}

void image_draw_rect(image_t* img, int x, int y, int w, int h, int thickness, argb_t color)
{
    if (img == NULL) { return; }
    if (img->bpp == COLORDEPTH_4  || img->bpp == COLORDEPTH_8 || 
        img->bpp == COLORDEPTH_16 || img->bpp == COLORDEPTH_24) { debug_error("image_draw_rect(%p, %d, %d, %d, %d, %d, %p) - Unsupported color depth", img, x, y, w, h, thickness, color.value); return; }  
}

void image_drawchar(image_t* img, int x, int y, char c, argb_t fg, argb_t bg, const font_t* font)
{
    if (img == NULL) { return; }
    if (img->bpp == COLORDEPTH_4  || img->bpp == COLORDEPTH_8 || 
        img->bpp == COLORDEPTH_16 || img->bpp == COLORDEPTH_24) { debug_error("image_drawchar(%p, %d, %d, %c, %p, %p, %p) - Unsupported color depth", img, x, y, c, fg.value, bg.value, font); return; }

    if (img->bpp == COLORDEPTH_32)
    {
        COLOR fgv = 0, bgv = 0;
        if (img->color_order == COLORORDER_ARGB)      { fgv = color_from_argb(fg); bgv = color_from_argb(bg); }
        else if (img->color_order == COLORORDER_ABGR) { fgv = color_from_abgr(fg); bgv = color_from_abgr(bg); }
        else if (img->color_order == COLORORDER_RGBA) { fgv = color_from_rgba(fg); bgv = color_from_rgba(bg); }
        else if (img->color_order == COLORORDER_BGRA) { fgv = color_from_bgra(fg); bgv = color_from_bgra(bg); }

        ssfn_src     = font->ssfn;
        ssfn_dst.ptr = (uint8_t*)img->buffer;
        ssfn_dst.p   = img->pitch;
        ssfn_dst.fg  = fgv;
        ssfn_dst.bg  = bgv;
        ssfn_dst.x   = y;
        ssfn_dst.y   = x;
        if (c != 0) { ssfn_putc(c, true); }
    }
}

void image_drawstr(image_t* img, int x, int y, const char* str, argb_t fg, argb_t bg, const font_t* font)
{
    if (img == NULL) { return; }
    if (img->bpp == COLORDEPTH_4  || img->bpp == COLORDEPTH_8 || 
        img->bpp == COLORDEPTH_16 || img->bpp == COLORDEPTH_24) { debug_error("image_drawstr(%p, %d, %d, %s, %p, %p, %p) - Unsupported color depth", img, x, y, str, fg.value, bg.value, font); return; }

    if (img->bpp == COLORDEPTH_32)
    {
        COLOR fgv = 0, bgv = 0;
        if (img->color_order == COLORORDER_ARGB)      { fgv = color_from_argb(fg); bgv = color_from_argb(bg); }
        else if (img->color_order == COLORORDER_ABGR) { fgv = color_from_abgr(fg); bgv = color_from_abgr(bg); }
        else if (img->color_order == COLORORDER_RGBA) { fgv = color_from_rgba(fg); bgv = color_from_rgba(bg); }
        else if (img->color_order == COLORORDER_BGRA) { fgv = color_from_bgra(fg); bgv = color_from_bgra(bg); }

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
                ssfn_dst.ptr = (uint8_t*)img->buffer;
                ssfn_dst.p   = img->pitch;
                ssfn_dst.fg  = fgv;
                ssfn_dst.bg  = bgv;
                ssfn_dst.x   = xx;
                ssfn_dst.y   = yy;

                uint32_t unicode;
                const char* cstr = &str[i];
                while ((*cstr & 0xC0) == 0x80) { ++cstr; }
                size_t seq_len = cstr - &str[i];
                
                unicode = ssfn_utf8(&cstr);
                if (unicode != 0) { ssfn_putc(unicode, true); }

                xx += font->charsz.x + font->spacing.x;
                i += seq_len;
            }
        }
    }
}

argb_t image_getpixel(image_t* img, int x, int y)
{
    if (img == NULL) { return; }
    if ((uint32_t)x >= (uint32_t)img->sz.x || (uint32_t)y >= (uint32_t)img->sz.y) { return (argb_t){ 0 }; }
    if (img->bpp == COLORDEPTH_4  || img->bpp == COLORDEPTH_8 || 
        img->bpp == COLORDEPTH_16 || img->bpp == COLORDEPTH_24) { debug_error("image_getpixel(%p, %d, %d) - Unsupported color depth", img, x, y); return (argb_t){ 0 }; }

    if (img->bpp == COLORDEPTH_32) 
    {
        if (img->color_order == COLORORDER_ARGB) { return color_to_argb(((uint32_t*)img->buffer)[y * (img->pitch / 4) + x]); }
        if (img->color_order == COLORORDER_ABGR) { return color_to_abgr(((uint32_t*)img->buffer)[y * (img->pitch / 4) + x]); }
        if (img->color_order == COLORORDER_RGBA) { return color_to_rgba(((uint32_t*)img->buffer)[y * (img->pitch / 4) + x]); }
        if (img->color_order == COLORORDER_BGRA) { return color_to_bgra(((uint32_t*)img->buffer)[y * (img->pitch / 4) + x]); }
        return (argb_t){ 0 };
    }
}