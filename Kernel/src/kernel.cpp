#include <kernel.h>

extc void kernel_main(void* mboot)
{
    aeros::Kernel::multiboot = (aeros::multiboot_t*)mboot;
    aeros::Kernel::boot();
    while (true) { aeros::Kernel::main(); }
}

namespace aeros
{
    multiboot_t* Kernel::multiboot;

    void Kernel::boot()
    {
        uint8_t* vram = (uint8_t*)0xB8000;
        for (size_t i = 0; i < 4000; i += 2)
        {
            vram[i] = 'X';
            vram[i + 1] = 0x1E;
        }
    }

    void Kernel::main()
    {

    }
}