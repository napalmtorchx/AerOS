#include <debug/debug.h>
#include <kernel.h>

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
    uart_write(str);
}

void debug_error(const char* str, ...)
{
    uart_writeln(str);
    inline_asm("cli; hlt");
}