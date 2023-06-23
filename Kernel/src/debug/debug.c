#include <debug/debug.h>
#include <kernel.h>

#define PRINT_BUFFSZ 0x10000

static char _dbgbuff[PRINT_BUFFSZ];

void debug_init()
{
    uart_setport(UARTPORT_COM1);
}

void debug_print(const char* str)
{
    uart_write(str);
}

void debug_println(const char* str)
{
    uart_write(str);
    uart_putc('\n');
}

void debug_log(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    memset(_dbgbuff, 0, PRINT_BUFFSZ);
    vsprintf(_dbgbuff, str, args);
    va_end(args);

    uart_write(_dbgbuff);
    vga_write(_dbgbuff);
}

void debug_error(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    memset(_dbgbuff, 0, PRINT_BUFFSZ);
    vsprintf(_dbgbuff, str, args);
    va_end(args);
    
    static const char* halt_msg = "The system has been halted to prevent damage to your computer.";
    uart_write(DEBUG_ERROR);
    uart_putc(' ');
    uart_writeln(_dbgbuff);
    uart_writeln(halt_msg);

    inline_asm("cli; hlt");
}

void debug_hexdump(void* ptr, uintptr_t start, size_t sz)
{
    for (size_t i = 0; i < sz; i += 16)
    {
        debug_log("%s%p%s:%s%p%s ", ANSI_FG_CYAN, ((uintptr_t)ptr + start + i), ANSI_RESET, ANSI_FG_CYAN, ((uintptr_t)ptr + start + i + 15), ANSI_RESET);

        char ascii[32];
        memset(ascii, 0, sizeof(ascii));
        for (size_t j = 0; j < 16; j++)
        {
            if (i + j >= sz) { continue; }
            uint8_t value = ((uint8_t*)ptr)[start + i + j];
            
            if (value == 0) { debug_log("%s00%s ", ANSI_FG_RED, ANSI_RESET); }
            else { debug_log("%2x ", value); }

            if (value >= 32 && value <= 126) { stradd(ascii, value); }
            else { stradd(ascii, '.'); }
        }
        while (strlen(ascii) < 16) { stradd(ascii, '.'); }
        debug_log("%s%s%s\n", ANSI_FG_YELLOW, ascii, ANSI_RESET);
    }
}