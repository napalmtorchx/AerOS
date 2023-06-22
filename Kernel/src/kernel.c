#include <kernel.h>

void kernel_main(void* mboot)
{
    uint8_t* vram = (uint8_t*)0xB8000;
    for (size_t i = 0; i < 4000; i += 2)
    {
        vram[i] = 'X';
        vram[i + 1] = 0x1E;
    }

    while (true)
    {
        
    }
}