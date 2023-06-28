#include <services/gfxhost.h>
#include <kernel.h>

static thread_t*     _thread;
static uint32_t      _fps, _frames, _sec;
static char          _dbgbuff[256];
static vbe_device_t* _vbe;
static image_t       _backbuffer;
static bool          _running;

KRESULT gfx_main(int argc, char** argv)
{
    _running = true;
    while (true)
    {        
        lock();

        gfx_update();
        if (!_running) { taskmgr_schedule(true); break; }

        gfx_render();
        unlock();
    }

    image_free(&_backbuffer);
    cmdhost_toggle(true); 
    console_clear(kconsole_get());
    cmdhost_prompt();
    return KRESULT_SUCCESS;
}

void gfx_init()
{
    _running     = false;
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
    int secs = time(NULL).second;
    if (_sec != secs)
    {
        _sec    = secs;
        _fps    = _frames;
        _frames = 0;

        memset(_dbgbuff, 0, sizeof(_dbgbuff));
        sprintf(_dbgbuff, "FPS:%u\n", _fps);
        debug_log("FPS:%u\n", _fps);
    }

    if (kbd_keydown(KEY_ESCAPE)) 
    { 
        _running = false; 
        return;
    }
}

void gfx_render()
{
    image_clear(&_backbuffer, color_to_argb(0xFF3F003F));
    image_drawstr(&_backbuffer, 0, 0, _dbgbuff, color_to_argb(COLOR_WHITE), color_to_argb(0), sysfont_get());


    image_fill_rect(&_backbuffer, 100, 100, 100, 100, color_to_argb(0xFFFFFF00));

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            image_blit(&_backbuffer, 50 + i, 50 + j, color_to_argb(0x40FF0000));
        }
    }


    vbe_swap(_backbuffer.buffer);
    _frames++;
}

void gfx_rescale()
{
    if (_vbe == NULL) { _vbe = devmgr_from_name("vbe_controller"); }
    image_free(&_backbuffer);
    _backbuffer = image_create(_vbe->w, _vbe->h, COLORDEPTH_32, COLORORDER_ARGB);
    debug_log("%s Rescaled backbuffer to %dx%d\n", DEBUG_INFO, _backbuffer.sz.x, _backbuffer.sz.y);
}

image_t* gfx_backbuffer() { return &_backbuffer; }