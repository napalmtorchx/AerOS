#include <lib/stdlib.h>
#include <kernel.h>

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
