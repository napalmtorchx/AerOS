#include <kernel.h>

void kernel_main(void* mboot)
{
    vga_init();
    debug_init();
    debug_println("Hello world\nTesting 123");

    while (true)
    {
        
    }
}