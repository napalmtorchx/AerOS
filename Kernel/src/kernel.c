#include <kernel.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uint32_t _stack_top;
extern uint32_t _stack_bottom;

static multiboot_t* _multiboot;

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

    vbe_device_t* vbe = devmgr_try_from_name("vbe_controller");
    if (vbe != NULL)
    {
        font_t* font           = font_create_psf((const psf_hdr_t*)FONT_DEFAULT_DATA, 1, 0);
        image_t console_buffer = image_create(vbe->w, vbe->h);
        console_t console      = console_create(console_buffer, &font, COLOR_YELLOW, COLOR_DARKRED);   
    }
    vbe_clear(0xFF007F7F);
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