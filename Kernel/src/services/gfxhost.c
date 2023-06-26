#include <services/gfxhost.h>
#include <kernel.h>

static thread_t*     _thread;
static uint32_t      _fps, _frames, _sec;
static char          _dbgbuff[256];
static vbe_device_t* _vbe;
static image_t       _backbuffer;

KRESULT gfx_main(int argc, char** argv)
{
    while (true)
    {
        lock();
        gfx_update();
        gfx_render();
        unlock();
    }
    return KRESULT_SUCCESS;
}

void gfx_init()
{
    _sec         = 0;
    _fps         = 0;
    _frames      = 0;
    _vbe         = devmgr_from_name("vbe_controller");
    _backbuffer  = image_create(_vbe->w, _vbe->h, COLORDEPTH_32, COLORORDER_ARGB);
    memset(_dbgbuff, 0, sizeof(_dbgbuff));

    _thread = thread_create("gfxhost", 512 * KILOBYTE, gfx_main, THREAD_PRIORITY_NORMAL, 0, NULL);
    taskmgr_load(_thread);
    taskmgr_start(_thread);

    debug_log("%s Initialized graphics management host\n", DEBUG_OK);
}

void gfx_update()
{
    time_t t = time(NULL);
    if (_sec != t.second)
    {
        _sec    = t.second;
        _fps    = _frames;
        _frames = 0;

        memset(_dbgbuff, 0, sizeof(_dbgbuff));
        sprintf(_dbgbuff, "FPS:%u\n", _fps);
        debug_log("FPS:%u\n", _fps);
    }
}

void gfx_render()
{
    image_clear(&_backbuffer, color_to_argb(0xFF3F003F));
    image_drawstr(&_backbuffer, 0, 0, _dbgbuff, color_to_argb(COLOR_WHITE), color_to_argb(0), sysfont_get());

    vbe_swap(_backbuffer.buffer);
    _frames++;
}

image_t* gfx_backbuffer() { return &_backbuffer; }