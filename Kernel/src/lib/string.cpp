#include <lib/string.h>
#include <kernel.h>

#ifdef __cplusplus
extc {
#endif

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

void* memcpy(void* dest, const void* src, size_t n)
{
    
}

void* memset(void* ptr, int c, size_t n)
{
    uint8_t* p   = (uint8_t*)ptr;
    uint32_t v32 = (uint32_t)c;

    if (n >= 4) 
    {
        unsigned int rep = (v32 << 24) | (v32 << 16) | (v32 << 8) | v32;
        size_t count32 = n / 4;

        asm volatile(
            "cld\n\t"
            "rep stosl\n\t"
            : "=D"(p), "=c"(count32)
            : "D"(p), "a"(rep), "c"(count32)
            : "memory"
        );

        n %= 4;
    }
    while (n--) { *p++ = (uint8_t)v32; }
    return ptr;
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

#ifdef __cplusplus
}
#endif