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

char* strstr(const char* haystack, const char* needle) {
    if (*needle == '\0') {
        return (char*)haystack;  // Empty needle, return haystack
    }

    char* h = (char*)haystack;
    while (*h) {
        char* start = h;
        char* n = (char*)needle;

        while (*h && *n && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return start;  // Needle found, return the start of the match
        }

        h = start + 1;  // Start looking at the next character
    }

    return NULL;  // Needle not found
}
void strswap(char* x, char* y)
{
    char t = *x; 
    *x  = *y; 
    *y = t;
}

char* strback(char* str)
{
    if (str == NULL) { return NULL; }
    str[strlen(str) - 1] = 0;
    return str;
}

char* strdup(const char* str)
{
    if (str == NULL) { return NULL; }
    char* dup = (char*)malloc(strlen(str) + 1);
    memset(dup, 0, strlen(str) + 1);
    strcpy(dup, str);
    return dup;
}

char* strupper(char* str)
{
    for (int i = 0; i < strlen(str); i++) { str[i] = toupper(str[i]); }
    return str;
}

char* strlower(char* str)
{
    for (int i = 0; i < strlen(str); i++) { str[i] = tolower(str[i]); }
    return str;
}

char** strsplit(char* str, char delim, int* count)
{
    if (str == NULL) { return NULL; }
    if (strlen(str) == 0) { return NULL; }

    int len = strlen(str);
    uint32_t num_delimeters = 0;

    for(int i = 0; i < len - 1; i++) { if(str[i] == delim) { num_delimeters++; } }

    uint32_t arr_size = sizeof(char*) * (num_delimeters + 1);
    char** str_array = (char**)malloc(arr_size);
    memset(str_array, 0, arr_size);
    int str_offset = 0;

    int start = 0;
    int end = 0;
    while(end < len)
    {
        while(str[end] != delim && end < len) { end++; }

        char* substr = (char*)malloc(end - start + 1);
        memset(substr, 0, end - start + 1);
        memcpy(substr, str + start, end - start);
        start = end + 1;
        end++;
        str_array[str_offset] = substr;
        str_offset++;
    }
    *count = str_offset;
    return str_array;
}

char* strrev(char* str)
{
    if (str == NULL) { return NULL; }
    size_t sz = strlen(str);

    int i, j, temp;
    for (i = 0, j = sz - 1; i < j; i++, j--) 
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

//sse optimized memcpy
void* memcpy_sse(void* dest, const void* src, size_t n)
{
    void*       start = dest;
    const char* s     = (const char*)src;
    char*       d     = (char*)dest;
    size_t      blks  = n / 16, bytes = n % 16;

    for (size_t i = 0; i < blks; i++) 
    {
        asm volatile("movups (%0), %%xmm0\nmovntdq %%xmm0, (%1)\n" : : "r"(s), "r"(d) : "memory");
        s += 16;
        d += 16;
    }

    if (bytes > 0) { for (size_t i = 0; i < bytes; i++) { *d++ = *s++; } }
    return start;
}

void* memcpy(void* dest, const void* src, size_t n)
{
    bool aligned = ((uintptr_t)dest % 16 == 0) && ((uintptr_t)src % 16 == 0) && (n >= 16) && (n % 16 == 0);
    if (aligned && cpu_sse_enabled()) { return memcpy_sse(dest, src, n); }

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

void* memcpyt(void* dest, const void* src, size_t n)
{
    for (int i = 0; i < n; i++) {
        // load the value at src[i] into FPU's reg srack
        inline_asm("fldt %0" : : "m" (((long double*)src)[i]));
        // pop it back into the destination
        inline_asm("fstpt %0" : "=m" (((long double*)dest)[i]));
        // clear fpu exceptions
        inline_asm("fclex");
    }
}

void* memcpyq(void* dest, const void* src, size_t n)
{
    for (int i = 0; i < n; i++) {
        // load the value at src[i] into FPU's reg srack
        inline_asm("fildq %0" : : "m" (((uint64_t*)src)[i]));
        // pop it back into the destination
        inline_asm("fistpq %0" : "=m" (((uint64_t*)dest)[i]));
        // clear fpu exceptions
        inline_asm("fclex"); 
    }
}

void* memset(void* ptr, int c, size_t n)
{
    bool aligned = ((uintptr_t)ptr % 16 == 0) && (n >= 16) && (n % 16 == 0);
    if (aligned && kernel_booted() && cpu_sse_enabled()) { return memset_sse(ptr, c, n); }

    uint32_t  num_dwords = n / 4;
    uint32_t  num_bytes  = n % 4;
    uint32_t* dest32     = (uint32_t*)ptr;
    uint8_t*  dest8      = ((uint8_t*)ptr) + num_dwords * 4;
    inline_asm("cld;rep stosl" : "+D"(dest32), "+c"(num_dwords) : "a"(c) : "memory");
    inline_asm("cld;rep stosb" : "+D"(dest8),  "+c"(num_bytes)  : "a"(c) : "memory");
    return ptr;
}

void* memset_sse(void* ptr, int c, size_t n)
{
    uint8_t* dest = (uint8_t*)ptr;
    uint32_t values[4] = { c, c, c, c };

    for (size_t i = 0; i < n / 16; i++)
    {
        asm volatile("movups (%0), %%xmm0\n" "movntdq %%xmm0, (%1)\n"::"r"(values), "r"(dest) : "memory");
        dest += 16;
    }

    for (size_t i = 0; i < n % 16; i++) { *dest++ = (uint8_t)c; }
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