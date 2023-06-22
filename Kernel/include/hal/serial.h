#pragma once
#include <lib/types.h>

#define COM_PORT 0x03F8L

void debug_init();
void debug_print(char* str);
void debug_println(char* str);
