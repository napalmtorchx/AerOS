#include <kernel.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uint32_t _stack_top;
extern uint32_t _stack_bottom;

static multiboot_t* _multiboot;
static ramfs_t      _bootfs;

void kernel_main(multiboot_t* mboot)
{
    _multiboot = mboot;
    
    kernel_boot();
    while (true) { kernel_loop(); }
}

void kernel_boot()
{
    debug_init();
    debug_print("Starting AerOS v2.0...\nBootloader:");
    debug_println(_multiboot->bootloader_name);

    gdt_init();
    idt_init();
    memmgr_init();
    heap_init();
    devmgr_init();
    virtfs_init();    

    font_t* test_font = NULL;

    FILE* file = fopen("A:/unifont.sfn", "r");
    if (file == NULL) { debug_log("%s Failed to locate file 'A:/unifont.sfn'\n", DEBUG_ERROR); }
    else
    {
        ssfn_font_t* filedata = (ssfn_font_t*)malloc(file->sz);
        fread(filedata, file->sz, 1, file);
        fclose(file);
        test_font = font_create_ssfn(filedata, 0, 0);
    }

    vbe_device_t* vbe = devmgr_from_name("vbe_controller");
    image_t framebuffer = image_create(vbe->w, vbe->h);
    image_fill(&framebuffer, 0, 0, vbe->w, vbe->h, COLOR_DARKCYAN);
    image_drawchar(&framebuffer, 0, 0, 'X', COLOR_WHITE, COLOR_BLACK, test_font);
    image_drawstr(&framebuffer, 32, 32, "Hi äüö", COLOR_WHITE, COLOR_DARKRED, test_font);

    memcpy(vbe->fbptr, framebuffer.buffer, vbe->w * vbe->h * 4);
}

void kernel_loop()
{
    
}


uintptr_t kernel_addr_start() { return (uint32_t)&_kernel_start; }

uintptr_t kernel_addr_end() { return (uint32_t)&_kernel_end; }

size_t kernel_size() { return kernel_addr_end() - kernel_addr_start(); }

uintptr_t kernel_stack_start() { return (uint32_t)&_stack_bottom; }

uintptr_t kernel_stack_end() { return (uint32_t)&_stack_top; }

size_t kernel_stack_size() { return kernel_stack_end() - kernel_stack_start(); }

multiboot_t* mboot_get() { return _multiboot; }

ramfs_t* bootfs_get(void) { return &_bootfs; }