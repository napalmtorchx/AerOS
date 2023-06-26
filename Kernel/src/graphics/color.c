#include <graphics/color.h>
#include <kernel.h>

static const color4_t COLOR4_PALETTE[] = 
{
    { 0x0, 0x00, 0x00, 0x00 },  
    { 0x1, 0x00, 0x00, 0x7F },
    { 0x2, 0x00, 0x7F, 0x00 },  
    { 0x3, 0x00, 0x7F, 0x7F },
    { 0x4, 0x7F, 0x00, 0x00 },  
    { 0x5, 0x7F, 0x00, 0x7F },
    { 0x6, 0x7F, 0x7F, 0x00 },  
    { 0x7, 0x7F, 0x7F, 0x7F },
    { 0x8, 0x7F, 0x7F, 0x7F }, 
    { 0x9, 0x7F, 0x7F, 0xFF },
    { 0xA, 0x7F, 0xFF, 0x7F }, 
    { 0xB, 0x7F, 0xFF, 0xFF },
    { 0xC, 0xFF, 0x7F, 0x7F }, 
    { 0xD, 0xFF, 0x7F, 0xFF },
    { 0xE, 0xFF, 0xFF, 0x7F }, 
    { 0xF, 0xFF, 0xFF, 0xFF },
};

COLOR color_create(uint8_t a, uint8_t r, uint8_t g, uint8_t b) { return (a << 24) | (r << 16) | (g << 8) | b; }

COLOR color_from_argb(argb_t color) { return ((uint32_t)color.desc.a << 24) | ((uint32_t)color.desc.r << 16) | ((uint32_t)color.desc.g << 8) | color.desc.b; }

COLOR color_from_abgr(argb_t color) { return ((uint32_t)color.desc.a << 24) | ((uint32_t)color.desc.b << 16) | ((uint32_t)color.desc.g << 8) | color.desc.r; }

COLOR color_from_rgba(argb_t color) { return ((uint32_t)color.desc.r << 24) | ((uint32_t)color.desc.g << 16) | ((uint32_t)color.desc.b << 8) | color.desc.a; }

COLOR color_from_bgra(argb_t color) { return ((uint32_t)color.desc.b << 24) | ((uint32_t)color.desc.g << 16) | ((uint32_t)color.desc.r << 8) | color.desc.a; }

argb_t color_to_argb(COLOR color) { return (argb_t){ (color & 0xFF000000) >> 24, (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, color & 0xFF }; }

argb_t color_to_abgr(COLOR color) { return (argb_t){ (color & 0xFF000000) >> 24, color & 0xFF, (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8 }; }

argb_t color_to_rgba(COLOR color) { return (argb_t){ (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, color & 0xFF, (color & 0xFF000000) >> 24 }; }

argb_t color_to_bgra(COLOR color) { return (argb_t){ color & 0xFF, (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, (color & 0xFF000000) >> 24 }; }

uint32_t color_dist_squared(argb_t argb, color4_t col4)
{
    int diffR = argb.desc.r - col4.r;
    int diffG = argb.desc.g - col4.g;
    int diffB = argb.desc.b - col4.b;

    return (diffR * diffR) + (diffG * diffG) + (diffB * diffB);
}

color4_t color4_from_argb(argb_t color)
{
    color4_t closest = COLOR4_PALETTE[0];
    unsigned int min = color_dist_squared(color, closest);

    for (int i = 0; i < sizeof(COLOR4_PALETTE) / sizeof(color4_t); i++) 
    {
        uint32_t distance = color_dist_squared(color, COLOR4_PALETTE[i]);        
        if (distance < min) { min = distance; closest = COLOR4_PALETTE[i]; }
    }
    return closest;
}

uint8_t color4_bg_index(color4_t color)
{
    if (color.index == VGA_DARKGRAY) { color.index = 0; }
    return 0;
}

uint32_t color_bpp_multiplier(COLORDEPTH bpp)
{
    size_t bpp_mult = 1;
    switch (bpp)
    {
        default: { return 0; }
        case COLORDEPTH_16: { bpp_mult = 2; break; }
        case COLORDEPTH_24: { bpp_mult = 3; break; }
        case COLORDEPTH_32: { bpp_mult = 4; break; }
    }
    return bpp_mult;
}