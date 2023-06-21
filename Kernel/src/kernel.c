#include <kernel.h>
#include <debug/serial.h>
void kernel_main(void* mboot)
{
    uint8_t* vram = (uint8_t*)0xB8000;
    for (size_t i = 0; i < 4000; i += 2)
    {
        vram[i] = 'X';
        vram[i + 1] = 0x1E;
    }
    serial_init();
    serial_string("Test\n");
    serial_string("Test 2");
}