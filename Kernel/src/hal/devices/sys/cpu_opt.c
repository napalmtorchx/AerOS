#include <lib/types.h>
#include <kernel.h>
#include <hal/devices/sys/cpu_opt.h>
void sse_enable()
{
    unsigned int eax, cr0, cr4;
    inline_asm("mov %%cr0, %0" : "=r"(eax));
    inline_asm("andl $0xFFFB, %0" : "=a"(eax));
    inline_asm("orl $0x2, %0" : "=a"(eax));
    inline_asm("mov %0, %%cr0" : : "r"(eax));
    inline_asm("mov %%cr4, %0" : "=r"(eax));
    inline_asm("orl $0x600, %0" : "=a"(eax));
    inline_asm("mov %0, %%cr4" : : "r"(eax));
    inline_asm("mov %%cr0, %0" : "=r"(eax));
    inline_asm("orl $0x40000000, %0" : "=a"(eax));
    inline_asm("mov %0, %%cr0" : : "r"(eax));

}
void avx_enable()
{
    //we need to check if sse is enabled
    unsigned int eax, ebx, ecx, edx;
    inline_asm("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(1));
    if (ecx & (1 << 26))
    {
        inline_asm("mov %%cr4, %0" : "=r"(eax));
        inline_asm("orl $0x200, %0" : "=a"(eax));
        inline_asm("mov %0, %%cr4" : : "r"(eax));
        //check if sse is enabled
        inline_asm("mov %%cr4, %0" : "=r"(eax));
        if (eax & (1 << 9))
        {
            //now we need to check if avx is supported
            inline_asm("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(1));
            if (ecx & (1 << 28))
            {
                //now we need to check if avx is enabled
                inline_asm("mov %%cr4, %0" : "=r"(eax));
                if (eax & (1 << 10))
                {
                    printf("%s AVX enabled\n", DEBUG_INFO);
                }
                else
                {
                    printf("%s AVX not enabled\n", DEBUG_ERROR);
                }
            }
            else
            {
                printf("%s AVX not supported\n", DEBUG_ERROR);
            }
        }
        else
        {
            printf("%s SSE not enabled\n", DEBUG_ERROR);
        }
    }
}
void enable_optimized_sse()
{
    unsigned int eax, ebx, ecx, edx;
    inline_asm("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(1));
    if (ecx & (1 << 26))
    {
        inline_asm("mov %%cr4, %0" : "=r"(eax));
        inline_asm("orl $0x200, %0" : "=a"(eax));
        inline_asm("mov %0, %%cr4" : : "r"(eax));
        //check if sse is enabled
        inline_asm("mov %%cr4, %0" : "=r"(eax));
        if (eax & (1 << 9))
        {
            printf("%s SSE enabled\n", DEBUG_INFO);
        }
        else
        {
            printf("%s SSE not enabled\n", DEBUG_ERROR);
        }
    }
    else
    {
        printf("%s SSE not supported\n", DEBUG_ERROR);
    }
}
void get_cpu_name()
{
//get the full cpu name and vendor
    unsigned int eax, ebx, ecx, edx;
    inline_asm("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(0));
    char vendor[13];
    memcpy(vendor, &ebx, 4);
    memcpy(vendor + 4, &edx, 4);
    memcpy(vendor + 8, &ecx, 4);
    vendor[12] = 0;
    printf("%s CPU Vendor: %s\n", DEBUG_INFO, vendor);
    inline_asm("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(0x80000000));
    if (eax >= 0x80000004)
    {
        char name[49];
        inline_asm("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(0x80000002));
        memcpy(name, &eax, 4);
        memcpy(name + 4, &ebx, 4);
        memcpy(name + 8, &ecx, 4);
        memcpy(name + 12, &edx, 4);
        inline_asm("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(0x80000003));
        memcpy(name + 16, &eax, 4);
        memcpy(name + 20, &ebx, 4);
        memcpy(name + 24, &ecx, 4);
        memcpy(name + 28, &edx, 4);
        inline_asm("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(0x80000004));
        memcpy(name + 32, &eax, 4);
        memcpy(name + 36, &ebx, 4);
        memcpy(name + 40, &ecx, 4);
        memcpy(name + 44, &edx, 4);
        name[48] = 0;
        printf("%s CPU Name: %s\n", DEBUG_INFO, name);
    }
}
