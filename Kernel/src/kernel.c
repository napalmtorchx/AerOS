#include <kernel.h>

static multiboot_t* _multiboot;

void kernel_main(multiboot_t* mboot)
{
    _multiboot = mboot;
    
    kernel_boot();
    while (true) { kernel_loop(); }
}

void kernel_boot(void)
{
    debug_init();
    debug_print("Staring AerOS v2.0...\nBootloader:");
    debug_println(_multiboot->bootloader_name);

    devmgr_init();
}

void kernel_loop(void)
{
    
}

multiboot_t* mboot_get(void) { return _multiboot; }