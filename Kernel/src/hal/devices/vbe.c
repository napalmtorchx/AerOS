#include <hal/devices/vbe.h>
#include <kernel.h>
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#define SSFN_CONSOLEBITMAP_CONTROL
#define _STDINT_H
#include <lib/ssfn.h>
#define VBE_CTRL_PTR 0x80000
#define LNG_PTR(seg, off) ((seg << 4) | off)
#define REAL_PTR(arr) LNG_PTR(arr[1], arr[0])
#define SEG(addr) (((uint32_t)addr >> 4) & 0xF000)
#define OFF(addr) ((uint32_t)addr & 0xFFFF)

static vbe_device_t _vbe;

void vbe_init(void)
{
    _vbe = (vbe_device_t)
    {
        .base  = (device_t){ "vbe_controller", NULL_DUID, vbe_start, vbe_stop, false },
        .w     = 0,
        .h     = 0,
        .fbptr = NULL,
    };
    devmgr_register(&_vbe);
    devmgr_start(_vbe.base.uid, 0xFF000000);
}

void vbe_getheaders(void)
{
    vbe_ctrl_t* ctrlinfo = (vbe_ctrl_t*)VBE_CTRL_PTR;

    irq_context16_t context;
    memset(&context, 0, sizeof(irq_context16_t));
    context.ax = 0x4F00;
    context.es = 0x8000;
    context.di = 0x0000;
    int32(0x10, &context);

    if (context.ax != 0x4F) { debug_error("vbe_getheaders() - VBE not supported on this machine"); return; }
    _vbe.ctrl_hdr = ctrlinfo;
    _vbe.mode_hdr = (vbe_mode_t*)mboot_get()->vbe_mode;
}

bool vbe_start(vbe_device_t* dev, COLOR bg)
{
    vbe_setmode(640, 480);
    vbe_clear(bg);
    return true;
}
void vbe_load_font(const char* path)
{
    FILE* file = fopen(path, "r");
    if (file == NULL) { debug_error("Failed to locate file '%s'",path); }
    else
    {
        ssfn_font_t* filedata = (ssfn_font_t*)malloc(file->sz);
        fread(filedata, file->sz, 1, file);
        fclose(file);
        ssfn_src = filedata;     /* the bitmap font to use */
        ssfn_dst.ptr = (uint32_t*)_vbe.fbptr;                  /* framebuffer address and bytes per line */
        ssfn_dst.p = (uint16_t)_vbe.mode_hdr->pitch;
        ssfn_dst.fg = 0xFFFFFFFF;                   /* colors, white on black */
        ssfn_dst.bg = 0;
        ssfn_dst.x = 0;                           /* coordinates to draw to */
        ssfn_dst.y = 0;
    }
}
int vbe_stop(vbe_device_t* dev)
{
    return true;
}
void vbe_string(char* str)
{
    for(int i=0; i < strlen(str); i++)
    {
        ssfn_putc(str[i]);
    }
}
bool vbe_setmode(int w, int h)
{
    vbe_getheaders();

    irq_context16_t regs;
    memset(&regs, 0, sizeof(irq_context16_t));
    vbe_mode_t* minfo = (vbe_mode_t*)(VBE_CTRL_PTR + sizeof(vbe_ctrl_t) + 512);
    uint16_t*   modes = (uint16_t*)REAL_PTR(_vbe.ctrl_hdr->videomode);
    uint16_t    mode;

    for (int i = 0; modes[i] != 0xFFFF; i++)
    {
        regs.ax = 0x4F01;
        regs.cx = modes[i];
        regs.es = SEG(minfo);
        regs.di = OFF(minfo);
        int32(0x10, &regs);

        if (minfo->res_x == w && minfo->res_y == h && minfo->bpp == 32)
        {
            mode = modes[i];
            regs.ax = 0x4F02;
            regs.bx = mode | 0x4000;
            int32(0x10, &regs);
            _vbe.mode_hdr = minfo;
            _vbe.fbptr    = minfo->physical_base;
            _vbe.w        = minfo->res_x;
            _vbe.h        = minfo->res_y;
            debug_log("%s Set VBE mode to %dx%dx32bpp\n", DEBUG_INFO, _vbe.mode_hdr->res_x, _vbe.mode_hdr->res_y);
            return true;
        }
    }

    return false;
}

void vbe_swap(COLOR* src)
{
    memcpy(_vbe.fbptr, src, _vbe.w * _vbe.h * 4);
}

void vbe_clear(COLOR bg)
{
    memset(_vbe.fbptr, bg, _vbe.w * _vbe.h * 4);
}

void vbe_blit(int x, int y, COLOR color)
{
    if ((uint32_t)x >= _vbe.w || (uint32_t)y >= _vbe.h) { return; }
    _vbe.fbptr[y * _vbe.w + x] = color;
}

void vbe_fill(int x, int y, int w, int h, COLOR color)
{
    for (int i = 0; i < w * h; i++) { vbe_blit(x + (i % w), y + (i / w), color); }
}