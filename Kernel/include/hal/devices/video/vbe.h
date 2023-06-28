#pragma once
#include <lib/types.h>
#include <lib/numerics/point.h>
#include <graphics/color.h>
#include <core/multiboot.h>
#include <hal/device.h>

typedef struct
{
    device_t    base;
    uint16_t    w, h;
    uint32_t*   fbptr;
    uint32_t    pitch;
    vbe_mode_t* mode_hdr;
    vbe_ctrl_t* ctrl_hdr;
} vbe_device_t;

void vbe_init(void);
KRESULT vbe_start(vbe_device_t* dev, COLOR bg);
KRESULT vbe_stop(vbe_device_t* dev);
void vbe_probe();
bool vbe_setmode(int w, int h);
void vbe_swap(COLOR* src);
void vbe_clear(COLOR bg);
void vbe_blit(int x, int y, COLOR color);
void vbe_fill(int x, int y, int w, int h, COLOR color);

bool        vbe_checkmode(int w, int h);
vbe_mode_t* vbe_modes();
vbe_mode_t* vbe_mode_lowest();
vbe_mode_t* vbe_mode_highest();