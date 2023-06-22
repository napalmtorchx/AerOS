#include <hal/devices/vga.h>
#include <hal/ports.h>
console_char_t* vram = 0xB8000;
uint8_t vga_width = 80;
uint8_t vga_height = 25;
int console_width = 0;
int console_height = 0;
uint8_t fgcolor = vga_color_white;
uint8_t bgcolor = vga_color_black;
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}
void update_cursor(int x, int y)
{
	uint16_t pos = y * vga_width + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}
console_char_t* vga_entry(unsigned char uc, uint8_t color, size_t in) 
{
    vram[in].data = uc;
    vram[in].attrib.fg = color;
    vram[in].attrib.bg = bgcolor;
    return vram;
}
console_char_t* vga_entry_c(unsigned char uc, uint8_t fg, uint8_t bg, size_t in)
{
	vram[in].data = uc;
	vram[in].attrib.fg = fg;
	vram[in].attrib.bg = bg;
	return bg;
}
/// @brief init VGA Driver, clears with black background
void vga_init()
{
	for (size_t y = 0; y < vga_height; y++) {
		for (size_t x = 0; x < vga_width; x++) {
			const size_t index = (vga_width * x) + y;
			vga_entry_c(' ',fgcolor,bgcolor,index);
        }
    }
	enable_cursor(0,15);
}
/// @brief clean the screen with @param color
void clear(console_color_t color)
{
    for (size_t y = 0; y < vga_height; y++) {
		for (size_t x = 0; x < vga_width; x++) {
			const size_t index = (vga_width * x) + y;
			vga_entry(' ',color,index);
        }
    }
}
/// @brief  Put Char to the screen
/// @param c 
void putc(char c)
{
	switch (c)
	{
	case '\n':
		{
			console_width = 0;
			console_height ++;
			break;
		}
 
	default:
		{
			const size_t index = (vga_width * console_height) + console_width;
			vga_entry(c,vga_color_yellow,index);
			console_width ++;
			break;
		}
	}
 
	if (console_width >= vga_width)
	{
		console_width = 0;
		console_height ++;
	}
 
	if (console_height >= vga_height)
	{
		console_height = 0;
		console_width = 0;
	}
	update_cursor(console_width,console_height);
}
/// @brief Put characted with FG Color
/// @param c 
/// @param color 
void putc_color(char c, uint8_t color)
{
	switch (c)
	{
	case '\n':
		{
			console_width = 0;
			console_height ++;
			break;
		}
 
	default:
		{
			const size_t index = (vga_width * console_height) + console_width;
			vga_entry(c,color,index);
			console_width ++;
			break;
		}
	}
 
	if (console_width >= vga_width)
	{
		console_width = 0;
		console_height ++;
	}
 
	if (console_height >= vga_height)
	{
		console_height = 0;
		console_width = 0;
	}
	update_cursor(console_width,console_height);
} 
/// @brief  print string to screen
/// @param str 
void print(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i ++) // Keep placing characters until we hit the null-terminating character ('\0')
		putc(str[i]);
}

void printc(const char* str, uint8_t color)
{
	for (size_t i = 0; str[i] != '\0'; i ++) // Keep placing characters until we hit the null-terminating character ('\0')
		putc_color(str[i],color);
}