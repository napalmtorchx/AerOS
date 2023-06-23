#include <kernel.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uint32_t _stack_top;
extern uint32_t _stack_bottom;

static multiboot_t* _multiboot;
static ramfs_t      _bootfs;
static heap_t       _kernel_heap;
static font_t*      _sysfont;
static image_t      _framebuffer;

void kernel_main(multiboot_t* mboot)
{
    _multiboot = mboot;
    
    kernel_boot();
    kernel_loop();
}

void kernel_boot()
{
    debug_init();
    debug_print("Starting AerOS v2.0...\nBootloader:");
    debug_println(_multiboot->bootloader_name);

    gdt_init();
    idt_init();
    memmgr_init();
    _kernel_heap = init_kernel_heap();
    devmgr_init();
    virtfs_init();    

    fpu_init();

    // attempt to load system font
    FILE* file = fopen("A:/unifont.sfn", "r");
    if (file == NULL) { debug_log("%s Failed to locate file 'A:/unifont.sfn'\n", DEBUG_ERROR); }
    else
    {
        ssfn_font_t* filedata = (ssfn_font_t*)malloc(file->sz);
        fread(filedata, file->sz, 1, file);
        fclose(file);
        _sysfont = font_create_ssfn(filedata, 0, 0);
    }

    // initialize vbe framebuffer
    vbe_device_t* vbe = devmgr_from_name("vbe_controller");
    _framebuffer = image_create(vbe->w, vbe->h);
}

void kernel_loop()
{
    debug_log("%s Entered kernel main\n", DEBUG_INFO);
    while (true)
    {
        image_clear(&_framebuffer, COLOR_DARKCYAN);
        image_drawstr(&_framebuffer, 0, 0, "AerOS Framebuffer Test\nUnicode Test:äüöäüöäüö\n", COLOR_YELLOW, COLOR_DARKBLUE, _sysfont);
        vbe_swap(_framebuffer.buffer);
    }   
}

uintptr_t kernel_addr_start() { return (uint32_t)&_kernel_start; }

uintptr_t kernel_addr_end() { return (uint32_t)&_kernel_end; }

size_t kernel_size() { return kernel_addr_end() - kernel_addr_start(); }

uintptr_t kernel_stack_start() { return (uint32_t)&_stack_bottom; }

uintptr_t kernel_stack_end() { return (uint32_t)&_stack_top; }

size_t kernel_stack_size() { return kernel_stack_end() - kernel_stack_start(); }

multiboot_t* mboot_get() { return _multiboot; }

ramfs_t* bootfs_get(void) { return &_bootfs; }

font_t* sysfont_get() { return _sysfont; }

heap_t* kernel_heap_ref() { return &_kernel_heap; }