#include <kernel.h>
#include <debug/serial.h>
void kernel_main(void* mboot)
{
    vga_init();
    debug_init();
    debug_println("Test");
    debug_print("Test 2");
}