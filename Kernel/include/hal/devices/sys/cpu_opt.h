#pragma once
#include <lib/types.h>

void get_cpu_name();
int probe_sse();
void enable_sse();
bool is_qemu();
void cpuid(uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx);
bool cpu_sse_enabled();