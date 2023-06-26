#include <hal/devices/sys/cpu_opt.h>
#include <kernel.h>

static bool _sse = false;

int probe_sse()
{
    int result = 0;
    asm volatile(
        "mov $1, %%eax\n"
        "cpuid\n"
        "mov %%edx, %0\n"
        : "=r"(result)
        :
        : "%eax", "%ebx", "%ecx", "%edx"
    );
    return result & (1 << 25);
}

void enable_sse()
{
    int cr0 = 0;
    asm volatile(
        "mov %%cr0, %0\n"
        : "=r"(cr0)
        :
        :
    );
    cr0 |= (1 << 2);
    asm volatile(
        "mov %0, %%cr0\n"
        :
        : "r"(cr0)
        :
    );
    int cr4 = 0;
    asm volatile(
        "mov %%cr4, %0\n"
        : "=r"(cr4)
        :
        :
    );
    cr4 |= (1 << 9);
    asm volatile(
        "mov %0, %%cr4\n"
        :
        : "r"(cr4)
        :
    );
    int cr0_check = 0;
    int cr4_check = 0;
    asm volatile(
        "mov %%cr0, %0\n"
        : "=r"(cr0_check)
        :
        :
    );
    asm volatile(
        "mov %%cr4, %0\n"
        : "=r"(cr4_check)
        :
        :
    );
    if ((cr0_check & (1 << 2)) && (cr4_check & (1 << 9))) { _sse = true; }
}
int check_sse_support() {
    int sse_supported = 0;

    // Check CPU feature flags for SSE support
    unsigned int eax, ebx, ecx, edx;
    __asm__ __volatile__ (
        "cpuid\n\t"
        : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a" (1) // EAX=1 for feature flags
        : );

    if (edx & (1 << 25)) {
        sse_supported = 1;
    }

    return sse_supported;
}
void cpuid(uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    asm volatile(
        "cpuid"
        : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
        : "a"(*eax)
        : "memory"
    );
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

bool cpu_sse_enabled() { return _sse; }