#include <kernel.h>

void kernel_main(void* mboot)
{
    debug_init();
    debug_println("Hello world\nTesting 123");

    while (true)
    {
        
    }
}