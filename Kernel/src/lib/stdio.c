#include <lib/types.h>
#include <kernel.h>

#define PRINT_BUFFSZ 0x10000
#define PRINT_WORKSZ 0x2000

static char _buffer[PRINT_BUFFSZ];
static char _workbuff[PRINT_WORKSZ];

int vsprintf(char* buff, const char* fmt, va_list args)
{
    if (buff == NULL || strlen(fmt) == 0) { return; }
    while (*fmt != 0)
    {
        if (*fmt == '%')
        {
            fmt++;
            if (*fmt == '%') { stradd(buff, '%'); fmt++; }
            else if (*fmt == 'c') { stradd(buff, va_arg(args, int)); }
            else if (*fmt == 'd')
            {
                memset(_workbuff, 0, PRINT_WORKSZ);
                strcat(buff, itoa(va_arg(args, int), _workbuff, 10));
            }
            else if (*fmt == 'u')
            {
                memset(_workbuff, 0, PRINT_WORKSZ);
                strcat(buff, ltoa(va_arg(args, uint32_t), _workbuff, 10));
            }
            else if (*fmt == 'x')
            {
                memset(_workbuff, 0, PRINT_WORKSZ);
                strcat(buff, ltoa(va_arg(args, uint32_t), _workbuff, 16));
            }
            else if (*fmt == '2')
            {
                fmt++;
                if (*fmt != 'x' && *fmt != 'X') { stradd(buff, *fmt); fmt++; continue; }
                memset(_workbuff, 0, PRINT_WORKSZ);
                strcat(buff, xtoa(va_arg(args, uint32_t), _workbuff, 1));
            }
            else if (*fmt == '4')
            {
                fmt++;
                if (*fmt != 'x' && *fmt != 'X') { stradd(buff, *fmt); fmt++; continue; }
                memset(_workbuff, 0, PRINT_WORKSZ);
                strcat(buff, xtoa(va_arg(args, uint32_t), _workbuff, 2));
            }
            else if (*fmt == '8')
            {
                fmt++;
                if (*fmt != 'x' && *fmt != 'X') { stradd(buff, *fmt); fmt++; continue; }
                memset(_workbuff, 0, PRINT_WORKSZ);
                strcat(buff, xtoa(va_arg(args, uint32_t), _workbuff, 4));
            }
            else if (*fmt == 'p')
            {
                memset(_workbuff, 0, PRINT_WORKSZ);
                strcat(buff, xtoa(va_arg(args, uint32_t), _workbuff, 4));
            }
            else if (*fmt == 'a')
            {
                memset(_workbuff, 0, PRINT_WORKSZ);

                uint32_t v = va_arg(args, uint32_t);
                if (v < KILOBYTE)                       { strcat(buff, ltoa(v, _workbuff, 10)); strcat(buff, "B"); }
                else if (v >= KILOBYTE && v < MEGABYTE) { strcat(buff, ltoa(v / KILOBYTE, _workbuff, 10)); strcat(buff, "KB"); }
                else if (v >= MEGABYTE && v < GIGABYTE) { strcat(buff, ltoa(v / MEGABYTE, _workbuff, 10)); strcat(buff, "MB"); }
                else if (v >= GIGABYTE)                 { strcat(buff, ltoa(v / GIGABYTE, _workbuff, 10)); strcat(buff, "GB"); }
            }
            else if (*fmt == 'f')
            {
                memset(_workbuff, 0, PRINT_WORKSZ);
                strcat(buff, ftoa(va_arg(args, float), _workbuff, 3));
            }
            else if (*fmt == 's') 
            { 
                char* txt = va_arg(args, char*);
                if (txt == NULL) { strcat(buff, "(null)"); }
                else { strcat(buff, txt); }
            }
            else { stradd(buff, *fmt); }
        }
        else { stradd(buff, *fmt); }
        fmt++;
    }
}

int sprintf(char* buff, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(buff, fmt, args);
    va_end(args);
}

int vprintf(const char* fmt, va_list args)
{
    memset(_buffer, 0, PRINT_BUFFSZ);
    vsprintf(_buffer, fmt, args);
    if (kconsole_get() != NULL) { console_write(kconsole_get(), _buffer); }
    vga_write(_buffer);
    uart_write(_buffer);
}

int printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

FILE *fopen(const char *filename, const char *mode) { return virtfs_fopen(filename, mode); }

int fclose(FILE* stream) { return virtfs_fclose(stream); }

size_t fread(void *ptr, size_t size, size_t count, FILE *stream) { return virtfs_fread(ptr, size, count, stream); }
