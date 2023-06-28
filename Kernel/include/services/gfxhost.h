#pragma once
#include <lib/types.h>
#include <lib/kresult.h>
#include <graphics/image.h>

void gfx_init();
void gfx_update();
void gfx_render();
void gfx_rescale();
image_t* gfx_backbuffer();