#include <graphics/font.h>
#include <graphics/ssfn.h>
#include <kernel.h>

static font_t* _default_fnt;

font_t* font_create_psf(const psf_hdr_t* hdr, int sx, int sy)
{
    if (hdr == NULL) { debug_error("font_create_psf(%p, %d, %d) - Null pointer", hdr, sx, sy); return NULL; }

    font_t* font  = (font_t*)malloc(sizeof(font_t));
    font->psf     = hdr;
    font->charsz  = (point_t){ 8, hdr->charsz };
    font->spacing = (point_t){ sx, sy };
    font->sprite  = NULL;
    font->ssfn    = NULL;
    debug_log("%s Created font - Type:PSF Size:%dx%d Spacing:%dx%d\n", DEBUG_OK, font->charsz.x, font->charsz.y, sx, sy);
    return font;
}

font_t* font_create_spr(const image_t* sprite, int cw, int ch, int sx, int sy)
{
    if (sprite == NULL) { debug_error("font_create_psf(%p, %d, %d) - Null pointer", sprite, sx, sy); return NULL; }

    font_t* font  = (font_t*)malloc(sizeof(font_t));
    font->sprite  = sprite;
    font->charsz  = (point_t){ cw, ch };
    font->spacing = (point_t){ sx, sy };
    font->psf     = NULL;
    font->ssfn    = NULL;
    debug_log("%s Created font - Type:SPR Size:%dx%d Spacing:%dx%d\n", DEBUG_OK, font->charsz.x, font->charsz.y, sx, sy);
    return font;
}

font_t* font_create_ssfn(const ssfn_font_t* hdr, int sx, int sy)
{
    if (hdr == NULL) { debug_error("font_create_ssfn(%p, %d, %d) - Null font pointer", hdr, sx, sy); return NULL; }

    uint32_t charactersTableOffset = hdr->characters_offs;
    uint8_t* charactersTable = (uint8_t*)hdr + charactersTableOffset;
    uint8_t* characterData = charactersTable;
    uint8_t width = characterData[2];

    font_t* font  = (font_t*)malloc(sizeof(font_t));
    font->ssfn    = hdr;
    font->charsz  = (point_t){ width, hdr->height };
    font->spacing = (point_t){ sx, sy };
    font->sprite  = NULL;
    font->psf     = NULL;
    debug_log("%s Created font - Type:SSFN Size:%dx%d Spacing:%dx%d\n", DEBUG_OK, font->charsz.x, font->charsz.y, sx, sy);
    return font;
}

void font_free(font_t* font)
{
    if (font == NULL) { debug_error("font_free(%p) - Null pointer"); return; }
    free(font);
    debug_log("%s Disposed font at %p\n", DEBUG_INFO, font);
}

void font_init_default(void)
{
    _default_fnt = font_create_psf((const psf_hdr_t*)FONT_DEFAULT_DATA, 1, 0);
}

font_t* font_get_default(void) { return _default_fnt; }

point_t font_getsz(const font_t* font, bool spaced)
{
    if (font == NULL) { return POINT_ZERO; }
    return (point_t){ spaced ? font->charsz.x + font->spacing.x : font->charsz.x, spaced ? font->charsz.y + font->spacing.y : font->charsz.y };
}