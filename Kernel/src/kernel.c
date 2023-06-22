#include <kernel.h>

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uint32_t _stack_top;
extern uint32_t _stack_bottom;

static multiboot_t* _multiboot;
static ramfs_t      _bootfs;
static heap_t       _kernel_heap;

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
//  heap_init();
    _kernel_heap = heap_create(kernel_addr_end(), 
                               (uintptr_t)memmgr_amount_installed(0) - sizeof(alloc_entry_t));
    devmgr_init();
    virtfs_init();    

    vbe_load_font("A:/unifont.sfn");
    vbe_clear(0xFF007F7F);

    vbe_string("Hello world\nTesting 123");
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

heap_t* kernel_heap_ref() { return &_kernel_heap; }