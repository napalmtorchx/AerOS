#pragma once
#include <stdint.h>
#include <stdbool.h>
#define COM_PORT 0x3F8
void debug_init();
void debug_print(char* str);
void debug_println(char* str);
