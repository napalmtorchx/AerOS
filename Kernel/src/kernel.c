#include <kernel.h>

void kernel_main(void* mboot)
{
    vga_init();
    debug_init();
    debug_print("HELLO WORLD\n");
    debug_println("\033[0;31mTest\033[0m");
    
    while (true)
    {
      
    }
}