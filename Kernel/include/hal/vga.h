#pragma once
#include <stdint.h>
#include <stddef.h>
typedef enum
{
  vga_color_black,
  vga_color_blue,
  vga_color_green,
  vga_color_cyan,
  vga_color_red,
  vga_color_magenta,
  vga_color_brown,
  vga_color_light_gray,
  vga_color_dark_gray,
  vga_color_light_blue,
  vga_color_light_green,
  vga_color_light_cyan,
  vga_color_light_red,
  vga_color_light_magenta,
  vga_color_yellow,
  vga_color_white
} console_color_t;

typedef struct
{
  console_color_t fg : 4;
  console_color_t bg : 4;
} __attribute__((packed)) attribute_t;
typedef struct
{
  char data;
  attribute_t attrib;
} __attribute__((packed)) console_char_t;
void vga_init();
void print(const char* str);
void printc(const char* str, uint8_t color);