#include <kernel.h>
#include <hal/serial.h>

void kernel_main(void* mboot)
{
    vga_init();
    debug_init();
    debug_println("Test");
    debug_print("Test 2\n");
    printc("Welcome to AerOS\n",vga_color_light_blue);
    printc("Test Build",vga_color_light_green);

    while (true)
    {
        
    }
}