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

    // RAMFS TESTING
    FILE* file = fopen("A:/hello.txt", "r");
    if (file == NULL) { debug_error("Failed to locate file 'A:/hello.txt'"); }
    else
    {
        char* filedata = malloc(file->sz);
        fread(filedata, file->sz, 1, file);
        fclose(file);

        debug_log("File contents:\n%s\n", filedata);
    }

    // CONSOLE TESTING
    vbe_device_t* vbe = devmgr_try_from_name("vbe_controller");
    if (vbe != NULL)
    {
        font_t* font           = font_create_psf((const psf_hdr_t*)FONT_DEFAULT_DATA, 1, 0);
        image_t console_buffer = image_create(vbe->w, vbe->h);
        console_t console      = console_create(console_buffer, &font, COLOR_YELLOW, COLOR_DARKRED);   

        image_fill(&console.img, 0, 0, vbe->w, vbe->h, COLOR_DARKMAGENTA);
        image_drawchar(&console.img, 32, 32, 'X', COLOR_YELLOW, COLOR_BLACK, font);
        memcpy(vbe->fbptr, console.img.buffer, vbe->w * vbe->h * 4);
    }
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