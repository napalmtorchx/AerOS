#include <lib/ansi.h>
#include <kernel.h>

uint8_t ansi_fg_to_vga(uint8_t ansi_code, uint8_t def_result)
{
    switch (ansi_code)
    {
        default: { return def_result; }
        case 30: { return VGA_BLACK; }
        case 31: { return VGA_DARKRED; }
        case 32: { return VGA_DARKGREEN; }
        case 33: { return VGA_DARKYELLOW; }
        case 34: { return VGA_DARKBLUE; }
        case 35: { return VGA_DARKMAGENTA; }
        case 36: { return VGA_DARKCYAN; }
        case 37: { return VGA_WHITE; }
        case 90: { return VGA_GRAY; }
        case 91: { return VGA_RED; }
        case 92: { return VGA_GREEN; }
        case 93: { return VGA_YELLOW; }
        case 94: { return VGA_BLUE; }
        case 95: { return VGA_MAGENTA; }
        case 96: { return VGA_CYAN; }
        case 97: { return VGA_WHITE; }
    }
}

uint8_t ansi_bg_to_vga(uint8_t ansi_code, uint8_t def_result)
{
    switch (ansi_code)
    {
        default:  { return def_result; }
        case 40:  { return VGA_BLACK; }
        case 41:  { return VGA_DARKRED; }
        case 42:  { return VGA_DARKGREEN; }
        case 43:  { return VGA_DARKYELLOW; }
        case 44:  { return VGA_DARKBLUE; }
        case 45:  { return VGA_DARKMAGENTA; }
        case 46:  { return VGA_DARKCYAN; }
        case 47:  { return VGA_WHITE; }
        case 100: { return VGA_GRAY; }
        case 101: { return VGA_RED; }
        case 102: { return VGA_GREEN; }
        case 103: { return VGA_YELLOW; }
        case 104: { return VGA_BLUE; }
        case 105: { return VGA_MAGENTA; }
        case 106: { return VGA_CYAN; }
        case 107: { return VGA_WHITE; }
    }
}

COLOR ansi_fg_to_color(uint8_t ansi_code, COLOR def_result)
{
    switch (ansi_code)
    {
        default: { return def_result; }
        case 30: { return COLOR_BLACK; }
        case 31: { return COLOR_DARKRED; }
        case 32: { return COLOR_DARKGREEN; }
        case 33: { return COLOR_DARKYELLOW; }
        case 34: { return COLOR_DARKBLUE; }
        case 35: { return COLOR_DARKMAGENTA; }
        case 36: { return COLOR_DARKCYAN; }
        case 37: { return COLOR_WHITE; }
        case 90: { return COLOR_GRAY; }
        case 91: { return COLOR_RED; }
        case 92: { return COLOR_GREEN; }
        case 93: { return COLOR_YELLOW; }
        case 94: { return COLOR_BLUE; }
        case 95: { return COLOR_MAGENTA; }
        case 96: { return COLOR_CYAN; }
        case 97: { return COLOR_WHITE; }
    }
}

COLOR ansi_bg_to_color(uint8_t ansi_code, COLOR def_result)
{
    switch (ansi_code)
    {
        default:  { return def_result; }
        case 40:  { return COLOR_BLACK; }
        case 41:  { return COLOR_DARKRED; }
        case 42:  { return COLOR_DARKGREEN; }
        case 43:  { return COLOR_DARKYELLOW; }
        case 44:  { return COLOR_DARKBLUE; }
        case 45:  { return COLOR_DARKMAGENTA; }
        case 46:  { return COLOR_DARKCYAN; }
        case 47:  { return COLOR_WHITE; }
        case 100: { return COLOR_GRAY; }
        case 101: { return COLOR_RED; }
        case 102: { return COLOR_GREEN; }
        case 103: { return COLOR_YELLOW; }
        case 104: { return COLOR_BLUE; }
        case 105: { return COLOR_MAGENTA; }
        case 106: { return COLOR_CYAN; }
        case 107: { return COLOR_WHITE; }
    }
}