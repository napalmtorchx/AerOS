#pragma once
#include <lib/types.h>
#include <lib/numerics/point.h>

typedef struct image_t image_t;

typedef struct
{
    uint8_t magic[2];
    uint8_t mode;
    uint8_t charsz;
} attr_pack psf_hdr_t;

typedef struct
{
    psf_hdr_t* psf;
    image_t*   sprite;
    point_t    charsz;
    point_t    spacing;
} font_t;

font_t* font_create_psf(const psf_hdr_t* hdr, int sx, int sy);
font_t* font_create_spr(const image_t* sprite, int cw, int ch, int sx, int sy);
void font_free(font_t* font);
void font_init_default(void);
font_t* font_get_default(void);
point_t font_getsz(const font_t* font, bool spaced);

static const uint8_t FONT_DEFAULT_DATA[] = 
{
    0x36, 0x04, 0x02, 0x10, 0x00, 0x00, 0x00, 0x3E, 0x63, 0x5D, 0x7D, 0x7B, 0x77, 0x77, 0x7F, 0x77, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 
    0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x20, 
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x00, 
    0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x00, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x30, 0x10, 0x11, 0x12, 0x04, 0x08, 0x12, 0x26, 0x4A, 
    0x0F, 0x02, 0x02, 0x00, 0x00, 0x00, 0x10, 0x30, 0x10, 0x11, 0x12, 0x04, 0x08, 0x10, 0x26, 0x49, 
    0x02, 0x04, 0x0F, 0x00, 0x00, 0x00, 0x70, 0x08, 0x30, 0x09, 0x72, 0x04, 0x08, 0x12, 0x26, 0x4A, 
    0x0F, 0x02, 0x02, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x08, 0x08, 0x30, 0x00, 0x00, 0x07, 0x08, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0xA4, 0xA4, 0x48, 0x10, 0x10, 0x2A, 0x55, 0x55, 
    0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF1, 0x5B, 0x55, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 
    0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x20, 0x10, 0x08, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x04, 0x08, 0x10, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 
    0x24, 0x24, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 
    0x24, 0x24, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
    0x10, 0x10, 0x08, 0x00, 0x42, 0x3C, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x40, 0x4E, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x42, 0x3C, 0x00, 0x00, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x46, 0x3A, 
    0x02, 0x02, 0x42, 0x3C, 0x08, 0x08, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x30, 0x0C, 0x02, 0x42, 0x42, 
    0x3C, 0x08, 0x08, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x30, 0x0C, 0x02, 0x42, 
    0x3C, 0x08, 0x08, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x7E, 0x24, 0x24, 0x7E, 0x24, 0x24, 
    0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x49, 0x48, 0x48, 0x3E, 0x09, 0x09, 0x49, 
    0x3E, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x31, 0x4A, 0x4A, 0x34, 0x08, 0x08, 0x16, 0x29, 0x29, 
    0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x24, 0x18, 0x28, 0x45, 0x42, 0x46, 
    0x39, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 
    0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 
    0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x49, 0x2A, 0x1C, 0x2A, 0x49, 0x08, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 
    0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x4A, 0x52, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0x28, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x02, 0x1C, 0x02, 0x02, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0C, 0x14, 0x24, 0x44, 0x44, 0x7E, 0x04, 0x04, 
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x7C, 0x02, 0x02, 0x02, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x20, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x3C, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x04, 
    0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 0x08, 
    0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x7E, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x02, 0x04, 0x08, 0x08, 0x00, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x4A, 0x56, 0x52, 0x52, 0x52, 0x4E, 0x20, 
    0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42, 0x7C, 0x42, 0x42, 0x42, 0x42, 
    0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x40, 0x40, 0x40, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 
    0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x40, 
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x40, 0x4E, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x44, 
    0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x44, 0x48, 0x50, 0x60, 0x60, 0x50, 0x48, 0x44, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x63, 0x63, 0x55, 0x55, 0x49, 0x49, 0x41, 0x41, 
    0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x62, 0x62, 0x52, 0x52, 0x4A, 0x4A, 0x46, 0x46, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40, 
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x5A, 0x66, 
    0x3C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x42, 0x7C, 0x48, 0x44, 0x44, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x30, 0x0C, 0x02, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x22, 0x22, 0x22, 0x14, 0x14, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x49, 0x49, 0x55, 0x55, 0x63, 0x63, 
    0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18, 0x24, 0x24, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
    0x10, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x38, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x7F, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x02, 0x3E, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0x62, 
    0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x40, 0x40, 0x40, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x40, 0x40, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x10, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x46, 0x3A, 
    0x02, 0x02, 0x42, 0x3C, 0x00, 0x00, 0x40, 0x40, 0x40, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
    0x04, 0x04, 0x44, 0x38, 0x00, 0x00, 0x40, 0x40, 0x40, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 
    0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0x62, 
    0x5C, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x42, 0x46, 
    0x3A, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x62, 0x40, 0x40, 0x40, 0x40, 0x40, 
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x30, 0x0C, 0x02, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 
    0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x22, 0x22, 0x14, 0x14, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x49, 0x49, 0x49, 0x49, 0x49, 
    0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3A, 
    0x02, 0x02, 0x42, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x08, 0x08, 0x08, 0x08, 0x30, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08, 0x08, 0x08, 0x08, 0x06, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x30, 0x00, 0x00, 0x00, 0x31, 0x49, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x18, 0x24, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x18, 0x24, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x18, 0x24, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x18, 0x24, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x18, 0x24, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x24, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x28, 0x48, 0x48, 0x7E, 0x48, 0x48, 0x48, 0x48, 
    0x4F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x40, 0x40, 0x40, 0x42, 0x42, 
    0x3C, 0x08, 0x08, 0x30, 0x30, 0x0C, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x7E, 0x40, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x06, 0x18, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0xF2, 0x42, 0x42, 0x42, 0x44, 
    0x78, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x42, 0x62, 0x62, 0x52, 0x52, 0x4A, 0x4A, 0x46, 0x46, 
    0x42, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x14, 0x08, 0x14, 0x22, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3A, 0x44, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x62, 0x22, 
    0x5C, 0x40, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x06, 0x18, 0x00, 0x41, 0x41, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x7C, 0x40, 
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x4C, 0x50, 0x50, 0x4C, 0x42, 0x42, 0x52, 
    0x4C, 0x00, 0x00, 0x00, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 
    0xAA, 0x55, 0xAA, 0x55, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x49, 0x48, 0x48, 0x48, 0x48, 0x49, 
    0x3E, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x20, 0x20, 0x20, 0x78, 0x20, 0x20, 0x20, 0x22, 
    0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x11, 0x20, 0x7E, 0x20, 0x7C, 0x20, 0x11, 
    0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x22, 0x14, 0x08, 0x3E, 0x08, 0x3E, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x24, 0x18, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x30, 0x0C, 0x02, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x18, 0x24, 0x22, 0x22, 0x12, 0x0C, 0x02, 
    0x22, 0x1C, 0x00, 0x00, 0x00, 0x24, 0x18, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x30, 0x0C, 0x02, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x99, 0xA5, 0xA1, 0xA1, 0xA5, 0x99, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x3C, 0x00, 0x7C, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x24, 0x48, 0x24, 0x12, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x02, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x1C, 0x22, 0x22, 0x22, 0x1C, 0x22, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0xB9, 0xA5, 0xA5, 0xB9, 0xA9, 0xA5, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 0x00, 0x7F, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x18, 0x20, 0x40, 0x7C, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x38, 0x04, 0x44, 0x38, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x24, 0x18, 0x00, 0x7E, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x66, 
    0x5A, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x3E, 0x7A, 0x7A, 0x7A, 0x7A, 0x3A, 0x0A, 0x0A, 0x0A, 
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x18, 0x00, 0x00, 0x7E, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 
    0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x7C, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x24, 0x12, 0x24, 0x48, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x48, 0x48, 0x48, 0x4E, 0x48, 0x48, 0x48, 0x48, 
    0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x49, 0x49, 0x4F, 0x48, 0x48, 0x49, 
    0x36, 0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x41, 0x41, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x10, 0x10, 0x20, 0x40, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0F, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0F, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xFF, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xFF, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x08, 0x08, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 
    0x88, 0x22, 0x88, 0x22, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 
    0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x17, 0x14, 0x14, 
    0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x04, 0xF4, 0x14, 0x14, 
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x17, 0x10, 0x1F, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xF4, 0x04, 0xFC, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x17, 0x10, 0x17, 0x14, 0x14, 
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xF4, 0x04, 0xF4, 0x14, 0x14, 
    0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xF7, 0x14, 0x14, 
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xF7, 0x00, 0xFF, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xF7, 0x00, 0xF7, 0x14, 0x14, 
    0x14, 0x14, 0x14, 0x14, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x2A, 0x08, 0x08, 0x08, 0x08, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x2A, 0x1C, 0x08, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x7F, 0x20, 0x10, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0xFE, 0x04, 0x08, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x3C, 0x42, 0x02, 0x3E, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x3C, 0x42, 0x02, 0x3E, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x3C, 0x42, 0x02, 0x3E, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x00, 0x3C, 0x42, 0x02, 0x3E, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x3C, 0x42, 0x02, 0x3E, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x00, 0x3C, 0x42, 0x02, 0x3E, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x49, 0x09, 0x3F, 0x48, 0x48, 0x49, 
    0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x40, 0x40, 0x40, 0x40, 0x42, 
    0x3C, 0x08, 0x08, 0x30, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x40, 0x40, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x40, 0x40, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x40, 0x40, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x40, 0x40, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
    0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x18, 0x64, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x00, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x42, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x32, 0x4C, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x10, 
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3C, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x62, 
    0x3C, 0x40, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 
    0x3A, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3A, 
    0x02, 0x02, 0x42, 0x3C, 0x00, 0x00, 0x40, 0x40, 0x40, 0x5C, 0x62, 0x42, 0x42, 0x42, 0x42, 0x62, 
    0x5C, 0x40, 0x40, 0x40, 0x00, 0x24, 0x24, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3A, 
    0x02, 0x02, 0x42, 0x3C, 0xFD, 0xFF, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 0x60, 0x22, 0xFF, 0xFF, 
    0x64, 0x22, 0xFF, 0xFF, 0x65, 0x22, 0xFF, 0xFF, 0xA0, 0x25, 0xAC, 0x25, 0xAE, 0x25, 0xFC, 0x25, 
    0xFE, 0x25, 0x1B, 0x2B, 0x0E, 0x22, 0xFF, 0xFF, 0xC6, 0x25, 0x66, 0x26, 0x25, 0x2B, 0x27, 0x2B, 
    0xFF, 0xFF, 0xBC, 0x00, 0xFF, 0xFF, 0xBD, 0x00, 0xFF, 0xFF, 0xBE, 0x00, 0xFF, 0xFF, 0xA6, 0x00, 
    0xFF, 0xFF, 0xA8, 0x00, 0xFF, 0xFF, 0xB8, 0x00, 0xFF, 0xFF, 0x92, 0x01, 0xFF, 0xFF, 0x20, 0x20, 
    0xFF, 0xFF, 0x21, 0x20, 0xFF, 0xFF, 0x30, 0x20, 0xFF, 0xFF, 0x22, 0x21, 0xFF, 0xFF, 0x26, 0x20, 
    0xFF, 0xFF, 0x39, 0x20, 0xFF, 0xFF, 0x3A, 0x20, 0xFF, 0xFF, 0x1C, 0x20, 0x1F, 0x20, 0x36, 0x20, 
    0xFF, 0xFF, 0x1D, 0x20, 0xBA, 0x02, 0xDD, 0x02, 0xEE, 0x02, 0x33, 0x20, 0xFF, 0xFF, 0x1E, 0x20, 
    0xFF, 0xFF, 0x42, 0x2E, 0xFF, 0xFF, 0x41, 0x2E, 0xCE, 0x02, 0xFF, 0xFF, 0x1E, 0x01, 0xFF, 0xFF, 
    0x1F, 0x01, 0xFF, 0xFF, 0x30, 0x01, 0xFF, 0xFF, 0x31, 0x01, 0xFF, 0xFF, 0x5E, 0x01, 0xFF, 0xFF, 
    0x5F, 0x01, 0xFF, 0xFF, 0x20, 0x00, 0xA0, 0x00, 0x00, 0x20, 0x01, 0x20, 0x02, 0x20, 0x03, 0x20, 
    0x04, 0x20, 0x05, 0x20, 0x06, 0x20, 0x07, 0x20, 0x08, 0x20, 0x09, 0x20, 0x0A, 0x20, 0x2F, 0x20, 
    0x5F, 0x20, 0xFF, 0xFF, 0x21, 0x00, 0xFF, 0xFF, 0x22, 0x00, 0xFF, 0xFF, 0x23, 0x00, 0xFF, 0xFF, 
    0x24, 0x00, 0xFF, 0xFF, 0x25, 0x00, 0xFF, 0xFF, 0x26, 0x00, 0xFF, 0xFF, 0x27, 0x00, 0xB4, 0x00, 
    0xB9, 0x02, 0xBC, 0x02, 0xCA, 0x02, 0x19, 0x20, 0x32, 0x20, 0xFF, 0xFF, 0x28, 0x00, 0xFF, 0xFF, 
    0x29, 0x00, 0xFF, 0xFF, 0x2A, 0x00, 0x4E, 0x20, 0x17, 0x22, 0xFF, 0xFF, 0x2B, 0x00, 0xFF, 0xFF, 
    0x2C, 0x00, 0xCF, 0x02, 0x1A, 0x20, 0xFF, 0xFF, 0x2D, 0x00, 0xAD, 0x00, 0x10, 0x20, 0x11, 0x20, 
    0x12, 0x20, 0x13, 0x20, 0x43, 0x20, 0x12, 0x22, 0xFF, 0xFF, 0x2E, 0x00, 0x24, 0x20, 0xFF, 0xFF, 
    0x2F, 0x00, 0x44, 0x20, 0x15, 0x22, 0xFF, 0xFF, 0x30, 0x00, 0xFF, 0xFF, 0x31, 0x00, 0xFF, 0xFF, 
    0x32, 0x00, 0xFF, 0xFF, 0x33, 0x00, 0xFF, 0xFF, 0x34, 0x00, 0xFF, 0xFF, 0x35, 0x00, 0xFF, 0xFF, 
    0x36, 0x00, 0xFF, 0xFF, 0x37, 0x00, 0xFF, 0xFF, 0x38, 0x00, 0xFF, 0xFF, 0x39, 0x00, 0xFF, 0xFF, 
    0x3A, 0x00, 0x36, 0x22, 0xFF, 0xFF, 0x3B, 0x00, 0xFF, 0xFF, 0x3C, 0x00, 0xFF, 0xFF, 0x3D, 0x00, 
    0x40, 0x2E, 0xFF, 0xFF, 0x3E, 0x00, 0xFF, 0xFF, 0x3F, 0x00, 0xFF, 0xFF, 0x40, 0x00, 0xFF, 0xFF, 
    0x41, 0x00, 0xFF, 0xFF, 0x42, 0x00, 0xFF, 0xFF, 0x43, 0x00, 0xFF, 0xFF, 0x44, 0x00, 0xFF, 0xFF, 
    0x45, 0x00, 0xFF, 0xFF, 0x46, 0x00, 0xFF, 0xFF, 0x47, 0x00, 0xFF, 0xFF, 0x48, 0x00, 0xFF, 0xFF, 
    0x49, 0x00, 0xFF, 0xFF, 0x4A, 0x00, 0xFF, 0xFF, 0x4B, 0x00, 0x2A, 0x21, 0xFF, 0xFF, 0x4C, 0x00, 
    0xFF, 0xFF, 0x4D, 0x00, 0xFF, 0xFF, 0x4E, 0x00, 0xFF, 0xFF, 0x4F, 0x00, 0xFF, 0xFF, 0x50, 0x00, 
    0xFF, 0xFF, 0x51, 0x00, 0xFF, 0xFF, 0x52, 0x00, 0xFF, 0xFF, 0x53, 0x00, 0xFF, 0xFF, 0x54, 0x00, 
    0xFF, 0xFF, 0x55, 0x00, 0xFF, 0xFF, 0x56, 0x00, 0xFF, 0xFF, 0x57, 0x00, 0xFF, 0xFF, 0x58, 0x00, 
    0xFF, 0xFF, 0x59, 0x00, 0xFF, 0xFF, 0x5A, 0x00, 0xFF, 0xFF, 0x5B, 0x00, 0xFF, 0xFF, 0x5C, 0x00, 
    0xF5, 0x29, 0xFF, 0xFF, 0x5D, 0x00, 0xFF, 0xFF, 0x5E, 0x00, 0xC4, 0x02, 0xC6, 0x02, 0x03, 0x23, 
    0xFF, 0xFF, 0x5F, 0x00, 0xFF, 0xFF, 0x60, 0x00, 0xBB, 0x02, 0xBD, 0x02, 0xCB, 0x02, 0x18, 0x20, 
    0x1B, 0x20, 0x35, 0x20, 0xFF, 0xFF, 0x61, 0x00, 0xFF, 0xFF, 0x62, 0x00, 0xFF, 0xFF, 0x63, 0x00, 
    0xFF, 0xFF, 0x64, 0x00, 0xFF, 0xFF, 0x65, 0x00, 0xFF, 0xFF, 0x66, 0x00, 0xFF, 0xFF, 0x67, 0x00, 
    0xFF, 0xFF, 0x68, 0x00, 0xFF, 0xFF, 0x69, 0x00, 0xFF, 0xFF, 0x6A, 0x00, 0xFF, 0xFF, 0x6B, 0x00, 
    0xFF, 0xFF, 0x6C, 0x00, 0xFF, 0xFF, 0x6D, 0x00, 0xFF, 0xFF, 0x6E, 0x00, 0xFF, 0xFF, 0x6F, 0x00, 
    0xFF, 0xFF, 0x70, 0x00, 0xFF, 0xFF, 0x71, 0x00, 0xFF, 0xFF, 0x72, 0x00, 0xFF, 0xFF, 0x73, 0x00, 
    0xFF, 0xFF, 0x74, 0x00, 0xFF, 0xFF, 0x75, 0x00, 0xFF, 0xFF, 0x76, 0x00, 0xFF, 0xFF, 0x77, 0x00, 
    0xFF, 0xFF, 0x78, 0x00, 0xFF, 0xFF, 0x79, 0x00, 0xFF, 0xFF, 0x7A, 0x00, 0xFF, 0xFF, 0x7B, 0x00, 
    0xFF, 0xFF, 0x7C, 0x00, 0x23, 0x22, 0xFF, 0xFF, 0x7D, 0x00, 0xFF, 0xFF, 0x7E, 0x00, 0xDC, 0x02, 
    0xFF, 0xFF, 0x22, 0x20, 0x19, 0x22, 0xCF, 0x25, 0xFF, 0xFF, 0xC0, 0x00, 0xFF, 0xFF, 0xC1, 0x00, 
    0xFF, 0xFF, 0xC2, 0x00, 0xFF, 0xFF, 0xC3, 0x00, 0xFF, 0xFF, 0xC4, 0x00, 0xFF, 0xFF, 0xC5, 0x00, 
    0x2B, 0x21, 0xFF, 0xFF, 0xC6, 0x00, 0xFF, 0xFF, 0xC7, 0x00, 0xFF, 0xFF, 0xC8, 0x00, 0xFF, 0xFF, 
    0xC9, 0x00, 0xFF, 0xFF, 0xCA, 0x00, 0xFF, 0xFF, 0xCB, 0x00, 0xFF, 0xFF, 0xCC, 0x00, 0xFF, 0xFF, 
    0xCD, 0x00, 0xFF, 0xFF, 0xCE, 0x00, 0xFF, 0xFF, 0xCF, 0x00, 0xFF, 0xFF, 0xD0, 0x00, 0x10, 0x01, 
    0xFF, 0xFF, 0xD1, 0x00, 0xFF, 0xFF, 0xD2, 0x00, 0xFF, 0xFF, 0xD3, 0x00, 0xFF, 0xFF, 0xD4, 0x00, 
    0xFF, 0xFF, 0xD5, 0x00, 0xFF, 0xFF, 0xD6, 0x00, 0xFF, 0xFF, 0xD7, 0x00, 0xFF, 0xFF, 0xD8, 0x00, 
    0xFF, 0xFF, 0xD9, 0x00, 0xFF, 0xFF, 0xDA, 0x00, 0xFF, 0xFF, 0xDB, 0x00, 0xFF, 0xFF, 0xDC, 0x00, 
    0xFF, 0xFF, 0xDD, 0x00, 0xFF, 0xFF, 0xDE, 0x00, 0xFF, 0xFF, 0xDF, 0x00, 0xFF, 0xFF, 0x92, 0x25, 
    0xFF, 0xFF, 0xA1, 0x00, 0xFF, 0xFF, 0xA2, 0x00, 0xFF, 0xFF, 0xA3, 0x00, 0xFF, 0xFF, 0xAC, 0x20, 
    0xFF, 0xFF, 0xA5, 0x00, 0xFF, 0xFF, 0x60, 0x01, 0xFF, 0xFF, 0xA7, 0x00, 0xFF, 0xFF, 0x61, 0x01, 
    0xFF, 0xFF, 0xA9, 0x00, 0xFF, 0xFF, 0xAA, 0x00, 0xFF, 0xFF, 0xAB, 0x00, 0xFF, 0xFF, 0xAC, 0x00, 
    0xFF, 0xFF, 0xA4, 0x00, 0xFF, 0xFF, 0xAE, 0x00, 0xFF, 0xFF, 0xAF, 0x00, 0xC9, 0x02, 0xFF, 0xFF, 
    0xB0, 0x00, 0xDA, 0x02, 0xFF, 0xFF, 0xB1, 0x00, 0xFF, 0xFF, 0xB2, 0x00, 0xFF, 0xFF, 0xB3, 0x00, 
    0xFF, 0xFF, 0x7D, 0x01, 0xFF, 0xFF, 0xB5, 0x00, 0xBC, 0x03, 0xFF, 0xFF, 0xB6, 0x00, 0xFF, 0xFF, 
    0xB7, 0x00, 0x27, 0x20, 0xC5, 0x22, 0x31, 0x2E, 0xFF, 0xFF, 0x7E, 0x01, 0xFF, 0xFF, 0xB9, 0x00, 
    0xFF, 0xFF, 0xBA, 0x00, 0xFF, 0xFF, 0xBB, 0x00, 0xFF, 0xFF, 0x52, 0x01, 0xFF, 0xFF, 0x53, 0x01, 
    0xFF, 0xFF, 0x78, 0x01, 0xFF, 0xFF, 0xBF, 0x00, 0xFF, 0xFF, 0x00, 0x25, 0x14, 0x20, 0x15, 0x20, 
    0xAF, 0x23, 0xFF, 0xFF, 0x02, 0x25, 0xFF, 0xFF, 0x0C, 0x25, 0x6D, 0x25, 0xFF, 0xFF, 0x10, 0x25, 
    0x6E, 0x25, 0xFF, 0xFF, 0x14, 0x25, 0x70, 0x25, 0xFF, 0xFF, 0x18, 0x25, 0x6F, 0x25, 0xFF, 0xFF, 
    0x1C, 0x25, 0xFF, 0xFF, 0x24, 0x25, 0xFF, 0xFF, 0x2C, 0x25, 0xFF, 0xFF, 0x34, 0x25, 0xFF, 0xFF, 
    0x3C, 0x25, 0xFF, 0xFF, 0x91, 0x25, 0xFF, 0xFF, 0xBA, 0x23, 0x3E, 0x20, 0xFF, 0xFF, 0xBB, 0x23, 
    0xFF, 0xFF, 0xBC, 0x23, 0xFF, 0xFF, 0xBD, 0x23, 0xFF, 0xFF, 0x50, 0x25, 0x01, 0x25, 0xFF, 0xFF, 
    0x51, 0x25, 0x03, 0x25, 0xFF, 0xFF, 0x54, 0x25, 0x0F, 0x25, 0xFF, 0xFF, 0x57, 0x25, 0x13, 0x25, 
    0xFF, 0xFF, 0x5A, 0x25, 0x17, 0x25, 0xFF, 0xFF, 0x5D, 0x25, 0x1B, 0x25, 0xFF, 0xFF, 0x60, 0x25, 
    0x23, 0x25, 0xFF, 0xFF, 0x63, 0x25, 0x2B, 0x25, 0xFF, 0xFF, 0x66, 0x25, 0x33, 0x25, 0xFF, 0xFF, 
    0x69, 0x25, 0x3B, 0x25, 0xFF, 0xFF, 0x6C, 0x25, 0x4B, 0x25, 0xFF, 0xFF, 0x88, 0x25, 0xFF, 0xFF, 
    0x91, 0x21, 0xFF, 0xFF, 0x93, 0x21, 0xFF, 0xFF, 0x90, 0x21, 0xFF, 0xFF, 0x92, 0x21, 0xFF, 0xFF, 
    0xE0, 0x00, 0xFF, 0xFF, 0xE1, 0x00, 0xFF, 0xFF, 0xE2, 0x00, 0xFF, 0xFF, 0xE3, 0x00, 0xFF, 0xFF, 
    0xE4, 0x00, 0xFF, 0xFF, 0xE5, 0x00, 0xFF, 0xFF, 0xE6, 0x00, 0xFF, 0xFF, 0xE7, 0x00, 0xFF, 0xFF, 
    0xE8, 0x00, 0xFF, 0xFF, 0xE9, 0x00, 0xFF, 0xFF, 0xEA, 0x00, 0xFF, 0xFF, 0xEB, 0x00, 0xFF, 0xFF, 
    0xEC, 0x00, 0xFF, 0xFF, 0xED, 0x00, 0xFF, 0xFF, 0xEE, 0x00, 0xFF, 0xFF, 0xEF, 0x00, 0xFF, 0xFF, 
    0xF0, 0x00, 0xFF, 0xFF, 0xF1, 0x00, 0xFF, 0xFF, 0xF2, 0x00, 0xFF, 0xFF, 0xF3, 0x00, 0xFF, 0xFF, 
    0xF4, 0x00, 0xFF, 0xFF, 0xF5, 0x00, 0xFF, 0xFF, 0xF6, 0x00, 0xFF, 0xFF, 0xF7, 0x00, 0xFF, 0xFF, 
    0xF8, 0x00, 0xFF, 0xFF, 0xF9, 0x00, 0xFF, 0xFF, 0xFA, 0x00, 0xFF, 0xFF, 0xFB, 0x00, 0xFF, 0xFF, 
    0xFC, 0x00, 0xFF, 0xFF, 0xFD, 0x00, 0xFF, 0xFF, 0xFE, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF
};