#include <kernel.h>
#include <debug/serial.h>
void kernel_main(void* mboot)
{
<<<<<<< HEAD
    uint8_t* vram = (uint8_t*)0xB8000;
    for (size_t i = 0; i < 4000; i += 2)
    {
        vram[i] = 'X';
        vram[i + 1] = 0x1E;
    }

    while (true)
    {
        
    }
=======
    vga_init();
    debug_init();
    debug_println("Test");
    debug_print("Test 2\n");
    printc("Welcome to AerOS\n",vga_color_light_blue);
    printc("Test Build",vga_color_light_green);
>>>>>>> 547d9a2fcc70e0f6952a7a80c3f11ee636ad888d
}