#pragma once
#include <lib/types.h>

typedef struct {
  uint8_t magic1;             // must be zero
  uint8_t colormap;           // must be zero
  uint8_t encoding;           // must be 2
  uint16_t cmaporig, cmaplen; // must be zero
  uint8_t cmapent;            // must be zero
  uint16_t x;                 // must be zero
  uint16_t y;                 // image's height
  uint16_t h;                 // image's height
  uint16_t w;                 // image's width
  uint8_t bpp;                // must be 32
  uint8_t pixeltype;          // must be 40
} attr_pack tga_header_t;

uint32_t* tga_parse(uint8_t *ptr, int size);