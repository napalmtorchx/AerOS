#include <hal/devices/video/vbe.h>
#include <graphics/ssfn.h>
#include <kernel.h>

#define VBE_MAX_MODES 32
#define VBE_CTRL_PTR 0x80000
#define LNG_PTR(seg, off) ((seg << 4) | off)
#define REAL_PTR(arr) LNG_PTR(arr[1], arr[0])
#define SEG(addr) (((uint32_t)addr >> 4) & 0xF000)
#define OFF(addr) ((uint32_t)addr & 0xFFFF)

static vbe_device_t _vbe;
static vbe_mode_t   _modes[VBE_MAX_MODES];

void vbe_init(void)
{
    _vbe = (vbe_device_t)
    {
        .base  = (device_t){ "vbe_controller", NULL_DUID, vbe_start, vbe_stop, false },
        .w     = 0,
        .h     = 0,
        .fbptr = NULL,
        .pitch = 0,
    };

    memset(_modes, 0, sizeof(_modes));

    devmgr_register(&_vbe);
    devmgr_start(_vbe.base.uid, 0xFF000000);
}

KRESULT vbe_start(vbe_device_t* dev, COLOR bg)
{
    vbe_probe();

    vbe_mode_t* mode = vbe_mode_highest();
    debug_log("%s Highest VBE mode: %dx%dx%d\n", DEBUG_INFO, mode->res_x, mode->res_y, mode->bpp);
    if (mode->res_x > 1024 && mode->res_y > 768) { vbe_setmode(1024, 768); }
    else { vbe_setmode(mode->res_x, mode->res_y); }
    vbe_clear(bg);
    
    return KRESULT_SUCCESS;
}

KRESULT vbe_stop(vbe_device_t* dev)
{
    irq_context16_t regs;
    memset(&regs, 0, sizeof(irq_context16_t));
    regs.ax = 0x4F02;
    regs.bx = 0x0003;
    int32(0x10, &regs);
    return KRESULT_SUCCESS;
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

void vbe_probe()
{
    vbe_getheaders();
    vbe_mode_t* minfo = (vbe_mode_t*)(VBE_CTRL_PTR + sizeof(vbe_ctrl_t) + 512);
    uint16_t*   modes = (uint16_t*)REAL_PTR(_vbe.ctrl_hdr->videomode);
    
    int i = 0, j = 0;
    while (modes[i] != 0xFFFF)
    {
        irq_context16_t regs;
        memset(&regs, 0, sizeof(irq_context16_t));
        regs.ax = 0x4F01;
        regs.cx = modes[i];
        regs.es = SEG(minfo);
        regs.di = OFF(minfo);
        int32(0x10, &regs);
        if (minfo != NULL && minfo->bpp == 32) { _modes[j] = *minfo; j++; }
        i++;
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
            _vbe.pitch    = minfo->pitch;
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
    int xx, yy, i;
    for (i = 0; i < w * h; i++)
    {
        yy = y + (i / w);
        if (yy >= _vbe.h) { return; }

        xx = x + (i % w);
        if (xx >= _vbe.w) { continue; }

        _vbe.fbptr[yy * _vbe.w + xx] = color;
    }
}

uint32_t vbe_getpixel(int x, int y)
{
    return _vbe.fbptr[y * _vbe.w + x];
}

vbe_mode_t* vbe_modes() { return _modes; }

vbe_mode_t* vbe_mode_lowest()
{
    int w = UINT16_MAX, h = UINT16_MAX;

    vbe_mode_t* lowest = NULL;
    for (size_t i = 0; i < VBE_MAX_MODES; i++)
    {
        if (_modes[i].res_x < w && _modes[i].res_y < h) { lowest = &_modes[i]; w = _modes[i].res_x; h = _modes[i].res_y; }
    }
    return lowest;
}  

vbe_mode_t* vbe_mode_highest()
{
    int w = 0, h = 0;

    vbe_mode_t* highest = NULL;
    for (size_t i = 0; i < VBE_MAX_MODES; i++)
    {
        if (_modes[i].res_x >= w && _modes[i].res_y >= h) { highest = &_modes[i]; w = _modes[i].res_x; h = _modes[i].res_y; }
    }
    return highest;
}

/*
/// @brief List all available VBE resolutions, separated by newline it needs @param buff which should be atleast a 1kb char array buffer and needs to be freed after use.
char* vbe_available_resolutions(char* buff)
{
    vbe_getheaders();
    vbe_mode_t* minfo = (vbe_mode_t*)(VBE_CTRL_PTR + sizeof(vbe_ctrl_t) + 512);
    uint16_t*   modes = (uint16_t*)REAL_PTR(_vbe.ctrl_hdr->videomode);
    
    int i = 0;

    while (modes[i] != 0xFFFF)
    {
        irq_context16_t regs;
        memset(&regs, 0, sizeof(irq_context16_t));
        regs.ax = 0x4F01;
        regs.cx = modes[i];
        regs.es = SEG(minfo);
        regs.di = OFF(minfo);
        int32(0x10, &regs);

        if (minfo->bpp == 32) buff += sprintf(buff, "%dx%d\n", minfo->res_x, minfo->res_y);
        i++;
    }
    return buff;
}

uint32_t alpha_blend(uint32_t color1, uint32_t color2, float alpha)
{
    float a = sqrtf(alpha / 255.0f);
    float r = (1.0f - a) * ((color1 >> 16) & 0xFF) + a * ((color2 >> 16) & 0xFF);
    float g = (1.0f - a) * ((color1 >> 8) & 0xFF) + a * ((color2 >> 8) & 0xFF);
    float b = (1.0f - a) * (color1 & 0xFF) + a * (color2 & 0xFF);
    return (uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b;
}

void transparent_rect(int x, int y, int w, int h, COLOR src)
{
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++) vbe_blit(x + i, y + j, alpha_blend(src, vbe_getpixel(x + i, y + j), 50));
    }
}
void vbe_gradient_box(int x, int y, int w, int h, COLOR c1, COLOR c2)
{
    //we want to draw a gradient box from c1 to c2 without the use of math.h or alpha blending

    //first we need to get the difference between the two colors
    int r = (c2 >> 16) & 0xFF - (c1 >> 16) & 0xFF;
    int g = (c2 >> 8) & 0xFF - (c1 >> 8) & 0xFF;
    int b = c2 & 0xFF - c1 & 0xFF;
    //then we need to get the step size for each color
    float rstep = (float)r / (float)w;
    float gstep = (float)g / (float)w;
    float bstep = (float)b / (float)w;
    //now we can draw the gradient
    for (int i = 0; i < w; i++)
    {
        //we need to convert the float to an int
        int r = (int)(rstep * i);
        int g = (int)(gstep * i);
        int b = (int)(bstep * i);
        //then we need to add the step to the starting color
        COLOR c = (c1 & 0xFF000000) | ((r + ((c1 >> 16) & 0xFF)) << 16) | ((g + ((c1 >> 8) & 0xFF)) << 8) | (b + (c1 & 0xFF));
        //now we can the pixels
        for (int j = 0; j < h; j++) vbe_blit(x + i, y + j, c);
    }
}
*/