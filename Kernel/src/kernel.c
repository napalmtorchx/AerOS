#include <kernel.h>

void kernel_main(void* mboot)
{
    vga_init();
    debug_init();
    debug_print("HELLO WORLD\n");
    
    while (true)
    {
      
    }
}