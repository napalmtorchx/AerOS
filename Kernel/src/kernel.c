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
    debug_print("Staring AerOS v2.0...\nBootloader:");
    debug_println(_multiboot->bootloader_name);

    devmgr_init();
}

void kernel_loop()
{
    
}


uintptr_t kernel_addr_start() { return (uint32_t)&_kernel_start; }

uintptr_t kernel_addr_end() { return (uint32_t)&_kernel_end; }

size_t kernel_size() { return krnl_get_addr_end() - krnl_get_addr(); }

uintptr_t kernel_stack_start() { return (uint32_t)&_stack_bottom; }

uintptr_t kernel_stack_end() { return (uint32_t)&_stack_top; }

size_t kernel_stack_size() { return krnl_get_stk_top() - krnl_get_stk_btm(); }

multiboot_t* mboot_get() { return _multiboot; }