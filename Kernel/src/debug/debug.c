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