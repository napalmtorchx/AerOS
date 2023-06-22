#include <lib/string.h>
#include <kernel.h>

size_t strlen(const char* str)
{
    if (str == NULL) { return 0; }

    size_t len = 0;
    while (str[len] != 0) { len++; }
    return len;
}

char* strchr(const char* str, int c)
{
    while (*str != '\0') 
    {
        if (*str == c) { return (char*)str; }
        str++;
    }
    return NULL;
}

char* strrchr(const char* str, int c)
{
    const char *last = NULL;
    while (*str != '\0') 
    {
        if (*str == c) { last = str; }
        str++;
    }
    if (*str == c) { return (char*)str; }
    return (char*)last;
}


int strcmp(const char* str1, const char* str2) 
{
    while (*str1 && (*str1 == *str2)) { str1++; str2++; }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

int strncmp(const char* str1, const char* str2, size_t n) 
{
    while (n && *str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
        n--;
    }
    if (n == 0) { return 0; }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

char* strcpy(char* dest, const char* src) 
{
    char* orig_dest = dest;
    while ((*dest++ = *src++) != '\0');
    return orig_dest;
}

char* strncpy(char* dest, const char* src, size_t n) 
{
    char* orig_dest = dest;
    while (n && (*dest++ = *src++) != '\0') { n--; }
    if (n > 0) { while (n--) { *dest++ = '\0'; } }
    return orig_dest;
}

char* strcat(char* dest, const char* src) 
{
    char* orig_dest = dest;
    while (*dest != '\0') { dest++; }
    while ((*dest++ = *src++) != '\0');
    return orig_dest;
}

char* stradd(char* str, char c)
{
    if (str == NULL) { return NULL; }
    uint64_t len = strlen(str);
    str[len] = (char)c;
    str[len + 1] = 0;
    return str;
}

void* memcpy(void* dest, const void* src, size_t n)
{
    uint32_t  num_dwords = n / 4;
    uint32_t  num_bytes  = n % 4;
    uint32_t* dest32     = (uint32_t*)dest;
    uint32_t* src32      = (uint32_t*)src;
    uint8_t*  dest8      = ((uint8_t*)dest) + num_dwords * 4;
    uint8_t*  src8       = ((uint8_t*)src) + num_dwords * 4;
    inline_asm("cld;rep movsl" : "+D"(dest32), "+S"(src32), "+c"(num_dwords) : : "memory");
    inline_asm("cld;rep movsb" : "+D"(dest8),  "+S"(src8),  "+c"(num_bytes)  : : "memory");
    return dest;
}

void* memset(void* ptr, int c, size_t n)
{
    uint32_t  num_dwords = n / 4;
    uint32_t  num_bytes  = n % 4;
    uint32_t* dest32     = (uint32_t*)ptr;
    uint8_t*  dest8      = ((uint8_t*)ptr) + num_dwords * 4;
    inline_asm("cld;rep stosl" : "+D"(dest32), "+c"(num_dwords) : "a"(c) : "memory");
    inline_asm("cld;rep stosb" : "+D"(dest8),  "+c"(num_bytes)  : "a"(c) : "memory");
    return ptr;
}

void* memset16(void* ptr, uint16_t v, size_t n)
{
    return (uint16_t*)memset(ptr, (v << 16) | v, n);
}

int memcmp(const void* ptr1, const void* ptr2, size_t n)
{
    const uint8_t* p1 = (const uint8_t*)ptr1;
    const uint8_t* p2 = (const uint8_t*)ptr2;
    while (n--) 
    {
        if (*p1 != *p2) { return (*p1 < *p2) ? -1 : 1; }
        p1++;
        p2++;
    }
    return 0;
}

void* memmove(void* dest, const void* src, size_t n)
{
    
}

void* memchr(const void* ptr, int c, size_t n)
{
    const uint8_t* p = (const uint8_t*)ptr;
    uint8_t v = (uint8_t)c;
    while (n--) 
    {
        if (*p == v) { return (void*)p; }
        p++;
    }
    return NULL;
}

uint32_t memalign(uint32_t value, uint32_t align)
{
    uint32_t out = value;
    out &= (0xFFFFFFFF - (align - 1));
    if (out < value) { out += align; }
    return out;
}