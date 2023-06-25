#include <hal/devices/sys/cpu_opt.h>
#include <kernel.h>

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

bool is_qemu()
{
    uint32_t eax, ebx, ecx, edx;
    eax = 0x40000000;
    cpuid(&eax, &ebx, &ecx, &edx);
    if(strstr("KVMKVMKVM",(char*)&ebx) != NULL) return true;
    if(strstr("TCGTCGTC",(char*)&ebx) != NULL) return true;
    if(strstr("KVMKVMKVM",(char*)&ecx) != NULL) return true;
    if(strstr("TCGTCGTC",(char*)&ecx) != NULL) return true;
    if(strstr("KVMKVMKVM",(char*)&edx) != NULL) return true;
    if(strstr("TCGTCGTC",(char*)&edx) != NULL) return true;
    return false;
}
void cpuid(uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    asm volatile("cpuid"
        : "=a" (*eax),
        "=b" (*ebx),
        "=c" (*ecx),
        "=d" (*edx)
        : "a" (*eax)
    );
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
