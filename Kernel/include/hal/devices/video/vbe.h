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
    vbe_mode_t* mode_hdr;
    vbe_ctrl_t* ctrl_hdr;
} vbe_device_t;

void vbe_init(void);
bool vbe_start(vbe_device_t* dev, COLOR bg);
int  vbe_stop(vbe_device_t* dev);
void vbe_disable();
char* vbe_available_resolutions(char* buff);
bool vbe_setmode(int w, int h);
void vbe_swap(COLOR* src);
void vbe_clear(COLOR bg);
void vbe_blit(int x, int y, COLOR color);
void vbe_fill(int x, int y, int w, int h, COLOR color);