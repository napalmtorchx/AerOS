#include <debug/debug.h>
#include <kernel.h>
#define PRINT_BUFFSZ 0x10000
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
    d_vprintf(str, args);
    va_end(args);
}

void debug_error(const char* str, ...)
{
    uart_writeln(str);
    inline_asm("cli; hlt");
}