#include <lib/stdlib.h>
#include <kernel.h>

void* malloc(size_t size)
{
    return heap_alloc(kernel_heap_ref(),
                      size,
                      HEAP_ALLOC_TYPE_DIRECT,
                      HEAP_ALLOC_DATA_TYPE_OTHER).offset_start + kernel_heap_ref()->base;
}

void* calloc(size_t nmemb, size_t size)
{
    return heap_alloc(kernel_heap_ref(),
                      nmemb * size,
                      HEAP_ALLOC_TYPE_DIRECT,
                      HEAP_ALLOC_DATA_TYPE_OTHER).offset_start + kernel_heap_ref()->base;
                      
}

void* realloc(void* ptr, size_t size)
{
    // read entry info
    alloc_entry_t entry = heap_get_alloc_info(kernel_heap_ref(), (uintptr_t)ptr - kernel_heap_ref()->base);
    size_t sz = entry.offset_end - entry.offset_start;
    
    // allocate a new entry
    void *data = malloc(size);
    memset(data, 0, size);
    memcpy(data, ptr, sz);

    free(ptr);
    return data;
}
    

void free(void* ptr)
{
    heap_free(kernel_heap_ref(),
              heap_get_alloc_info(kernel_heap_ref(), (uintptr_t)ptr - kernel_heap_ref()->base));
}

char* itoa_rev(char *buffer, int i, int j)
{
    while (i < j) { strswap(&buffer[i++], &buffer[j--]); }
    return buffer;
}

void ultoa(unsigned long value, char* result, int base)
{
    unsigned char index;
    char buffer[32];
    index = 32;
    do 
    {
        buffer[--index] = '0' + (value % base);
        if ( buffer[index] > '9') { buffer[index] += 'A' - ':'; }
        value /= base;
    } while (value != 0);

    do { *result++ = buffer[index++]; } while (index < 32);
    *result = 0;
}

void ftoa_rev(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) 
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int ftoa_conv(int x, char str[], int d)
{
    int i = 0;
    while (x) { str[i++] = (x % 10) + '0'; x = x / 10; }

    while (i < d) { str[i++] = '0'; }

    ftoa_rev(str, i);
    str[i] = '\0';
    return i;
}

int atoi(const char* str) { return (int)atoll(str); }

long atol(const char* str) { return (long)atoll(str); }

long long atoll(const char* str)
{
    long long result = 0;
    int       sign   = 1;

    while (*str == ' ' || *str == '\t' || *str == '\n') { ++str; }

    if (*str == '-')
    {
        sign = -1;
        ++str;
    }
    else if (*str == '+') { ++str; }

    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        ++str;
    }
    return sign * result;
}


char* itoa(int num, char* buff, int base)
{
    if (base < 2 || base > 32) { return buff; }

    int i = 0, n = abs(num); 
    while (n)
    {
        int r = n % base;
        if (r >= 10) { buff[i++] = 65 + (r - 10); }
        else { buff[i++] = 48 + r; }
        n = n / base;
    }

    if (i == 0) { buff[i++] = '0'; }
    if (num < 0 && base == 10) { buff[i++] = '-'; }
    buff[i] = '\0';
    return itoa_rev(buff, 0, i - 1);
}

char* ltoa(uint32_t num, char* buff, int base)
{
    ultoa(num, buff, base); 
    return buff;
}

char* ftoa(float num, char* buff, int afterdot)
{
    int ipart = (int)num;
    float fpart = num - (float)ipart;
    int i = ftoa_conv(ipart, buff, 0);

    if (afterdot != 0) 
    {
        buff[i] = '.';
        fpart = fpart * powf(10, afterdot);
        ftoa_conv((int)fpart, buff + i + 1, afterdot);
    }
    return buff;
}

char* xtoa(uint32_t num, char* buff, uint8_t sz)
{
    static const char hexvals[] = "0123456789ABCDEF";

    char* start = buff;
    start[0] = 0;

    if (sz == 1)
    {
        *buff = hexvals[(uint8_t)((num & 0xF0) >> 4)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x0F))];
    }
    else if (sz == 2)
    {
        *buff = hexvals[(uint8_t)((num & 0xF000) >> 12)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x0F00) >> 8)];  buff++;
        *buff = hexvals[(uint8_t)((num & 0x00F0) >> 4)];  buff++;
        *buff = hexvals[(uint8_t)((num & 0x000F))]; 
    }
    else if (sz == 4)
    {
        *buff = hexvals[(uint8_t)((num & 0xF0000000) >> 28)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x0F000000) >> 24)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x00F00000) >> 20)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x000F0000) >> 16)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x0000F000) >> 12)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x00000F00) >> 8)];  buff++;
        *buff = hexvals[(uint8_t)((num & 0x000000F0) >> 4)];  buff++;
        *buff = hexvals[(uint8_t)((num & 0x0000000F))];
    }
    return start;
}

double strtod(const char* str, char** endptr) { return (double)strtold(str, endptr); }

float strtof(const char* str, char** endptr) { return (float)strtold(str, endptr); }

long double strtold(const char* str, char** endptr)
{
    long double result = 0.0;
    int sign = 1, found_digits = 0, exp_sign = 1, exponent = 0, dec_point = 0;

    while (isspace((unsigned char)*str))
        ++str;

    if (*str == '-') { sign = -1; ++str; }
    else if (*str == '+') { ++str; }

    while (isdigit((unsigned char)*str))
    {
        result = result * 10.0 + (*str - '0');
        ++str;
        found_digits = 1;
    }

    if (*str == '.')
    {
        ++str;
        dec_point = 1;

        while (isdigit((unsigned char)*str))
        {
            result = result * 10.0 + (*str - '0');
            ++str;
            found_digits = 1;
            dec_point *= 10;
        }
    }

    if (*str == 'e' || *str == 'E')
    {
        ++str;

        if (*str == '-') { exp_sign = -1; ++str; }
        else if (*str == '+') { ++str; }

        while (isdigit((unsigned char)*str))
        {
            exponent = exponent * 10 + (*str - '0');
            ++str;
        }
    }

    if (dec_point != 0) { result /= dec_point; }
    if (exponent != 0) { result *= powl(10.0, exp_sign * exponent); }
    if (endptr != NULL) { *endptr = (char *)str; }
    return sign * result;
}

long strtol(const char* str, char** endptr, int base) { return (long)strtoull(str, endptr, base); }

long long strtoll(const char* str, char** endptr, int base) { return (long long)strtoull(str, endptr, base); }

unsigned long strtoul(const char* str, char** endptr, int base) { return (unsigned long)strtoull(str, endptr, base); }

unsigned long long strtoull(const char* str, char** endptr, int base)
{
    unsigned long long result = 0;
    int sign = 1, found_digits = 0, digit;

    // Skip leading white space
    while (isspace((unsigned char)*str)) { ++str; }

    if (*str == '-') { sign = -1; ++str; }
    else if (*str == '+') { ++str; }

    if (base == 0)
    {
        if (*str == '0')
        {
            if (tolower((unsigned char)str[1]) == 'x') { base = 16; str += 2; }
            else { base = 8; ++str; }
        }
        else { base = 10; }
    }
    else if (base == 16)
    {
        if (*str == '0' && tolower((unsigned char)str[1]) == 'x')
            str += 2;
    }

    while (isxdigit((unsigned char)*str))
    {
        if (isdigit((unsigned char)*str)) { digit = *str - '0'; }
        else { digit = tolower((unsigned char)*str) - 'a' + 10; }

        if (result > ULLONG_MAX / base || (result == ULLONG_MAX / base && digit > ULLONG_MAX % base)) { result = ULLONG_MAX; break; }

        result = result * base + digit;
        ++str;
        found_digits = 1;
    }

    if (endptr != NULL) { *endptr = (char*) (found_digits ? str : NULL); }

    return sign * result;
}

int abs(int j) { return j < 0 ? -j : j; }

long labs(long j) { return j < 0 ? -j : j; }

long long llabs(long long j) { return j < 0 ? -j : j; }

div_t div(int numer, int denom)
{
    div_t result;
    result.quot = numer / denom;
    result.rem = numer % denom;
    return result;
}

ldiv_t ldiv(long numer, long denom)
{
    ldiv_t result;
    result.quot = numer / denom;
    result.rem = numer % denom;
    return result;
}

lldiv_t lldiv(long long numer, long long denom)
{
    lldiv_t result;
    result.quot = numer / denom;
    result.rem = numer % denom;
    return result;
}
