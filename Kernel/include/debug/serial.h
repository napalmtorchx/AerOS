#pragma once
#include <stdint.h>
#include <stdbool.h>
#define COM_PORT 0x3F8
void serial_init();
void serial_string(char* str);
void write_serial(char a);

