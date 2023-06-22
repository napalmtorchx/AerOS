#pragma once
#include <lib/types.h>

typedef struct 
{
    int quot;   // Quotient of the division
    int rem;    // Remainder of the division
} div_t;

typedef struct 
{
    long quot;   // Quotient of the division
    long rem;    // Remainder of the division
} ldiv_t;

typedef struct 
{
    long long quot;   // Quotient of the division
    long long rem;    // Remainder of the division
} lldiv_t;

/// @brief Allocates memory of given size. @param size The size of memory to allocate. @return A pointer to the allocated memory.
void* malloc(size_t size);

/// @brief Allocates memory for an array of elements. @param nmemb The number of elements in the array. @param size The size of each element. @return A pointer to the allocated memory.
void* calloc(size_t nmemb, size_t size);

/// @brief Changes the size of a memory block. @param ptr A pointer to the memory block to resize. @param size The new size of the memory block. @return A pointer to the resized memory block.
void* realloc(void* ptr, size_t size);

/// @brief Frees a memory block. @param ptr A pointer to the memory block to free.
void free(void* ptr);

/// @brief Converts a string to an integer. @param str The string to convert. @return The converted integer value.
int atoi(const char* str);

/// @brief Converts a string to a long integer. @param str The string to convert. @return The converted long integer value.
long atol(const char* str);

/// @brief Converts a string to a long long integer. @param str The string to convert. @return The converted long long integer value.
long long atoll(const char* str);

char* itoa(int num, char* buff, int base);

char* ltoa(uint32_t num, char* buff, int base);

char* ftoa(float num, char* buff, int afterdot);

char* xtoa(uint32_t num, char* buff, uint8_t sz);

/// @brief Generates a pseudo-random integer. @return The generated pseudo-random integer.
int rand();

/// @brief Seeds the random number generator. @param seed The seed value for the random number generator.
void srand(unsigned int seed);

/// @brief Allocates aligned memory of given size. @param alignment The desired alignment of the memory. @param size The size of memory to allocate. @return A pointer to the allocated memory.
void* aligned_alloc(size_t alignment, size_t size);

/// @brief Aborts the program.
void abort();

/// @brief Registers a function to be called at program exit. @param function A pointer to the function to be called at program exit. @return Zero if successful, non-zero if an error occurs.
int atexit(void (*function)());

/// @brief Terminates the program with a specified status code. @param status The exit status code.
void exit(int status);

/// @brief Retrieves the value of an environment variable. @param name The name of the environment variable. @return A pointer to the value of the environment variable, or NULL if it doesn't exist.
char* getenv(const char* name);

/// @brief Executes a command in the shell. @param command The command to be executed. @return The termination status of the executed command.
int system(const char* command);

/// @brief Converts a string to a double-precision floating-point number. @param str The string to convert. @param endptr A pointer to the character after the converted part of the string. @return The converted double-precision floating-point number.
double strtod(const char* str, char** endptr);

/// @brief Converts a string to a single-precision floating-point number. @param str The string to convert. @param endptr A pointer to the character after the converted part of the string. @return The converted single-precision floating-point number.
float strtof(const char* str, char** endptr);

/// @brief Converts a string to a long double-precision floating-point number. @param str The string to convert. @param endptr A pointer to the character after the converted part of the string. @return The converted long double-precision floating-point number.
long double strtold(const char* str, char** endptr);

/// @brief Converts a string to a long integer. @param str The string to convert. @param endptr A pointer to the character after the converted part of the string. @param base The number base used in the conversion. @return The converted long integer.
long strtol(const char* str, char** endptr, int base);

/// @brief Converts a string to a long long integer. @param str The string to convert. @param endptr A pointer to the character after the converted part of the string. @param base The number base used in the conversion. @return The converted long long integer.
long long strtoll(const char* str, char** endptr, int base);

/// @brief Converts a string to an unsigned long integer. @param str The string to convert. @param endptr A pointer to the character after the converted part of the string. @param base The number base used in the conversion. @return The converted unsigned long integer.
unsigned long strtoul(const char* str, char** endptr, int base);

/// @brief Converts a string to an unsigned long long integer. @param str The string to convert. @param endptr A pointer to the character after the converted part of the string. @param base The number base used in the conversion. @return The converted unsigned long long integer.
unsigned long long strtoull(const char* str, char** endptr, int base);

/// @brief Sorts an array using the quicksort algorithm. @param base The base address of the array to be sorted. @param nmemb The number of elements in the array. @param size The size of each element. @param compar The comparison function used to determine the order of elements.
void qsort(void* base, size_t nmemb, size_t size, int (*compar)(const void* , const void* ));

/// @brief Searches for a given element in a sorted array. @param key The element to search for. @param base The base address of the sorted array. @param nmemb The number of elements in the array. @param size The size of each element. @param compar The comparison function used to determine the order of elements. @return A pointer to the matching element, or NULL if not found.
void* bsearch(const void* key, const void* base, size_t nmemb, size_t size, int (*compar)(const void* , const void* ));

/// @brief Sets the value of an environment variable. @param string The string in the form "name=value" representing the environment variable. @return Zero if successful, non-zero if an error occurs.
int putenv(char* string);

/// @brief Sets the value of an environment variable or creates a new one. @param name The name of the environment variable. @param value The value to set for the environment variable. @param overwrite If non-zero, overwrites an existing variable with the same name. @return Zero if successful, non-zero if an error occurs.
int setenv(const char* name, const char* value, int overwrite);

/// @brief Unsets an environment variable. @param name The name of the environment variable to unset. @return Zero if successful, non-zero if an error occurs.
int unsetenv(const char* name);

/// @brief Computes the absolute value of an integer. @param j The input value. @return The absolute value of the input.
int abs(int j);

/// @brief Computes the absolute value of a long integer. @param j The input value. @return The absolute value of the input.
long labs(long j);

/// @brief Computes the absolute value of a long long integer. @param j The input value. @return The absolute value of the input.
long long llabs(long long j);

/// @brief Divides two integers and returns the quotient and remainder. @param numer The numerator. @param denom The denominator. @return The quotient and remainder as a structure of type div_t.
div_t div(int numer, int denom);

/// @brief Divides two long integers and returns the quotient and remainder. @param numer The numerator. @param denom The denominator. @return The quotient and remainder as a structure of type ldiv_t.
ldiv_t ldiv(long numer, long denom);

/// @brief Divides two long long integers and returns the quotient and remainder. @param numer The numerator. @param denom The denominator. @return The quotient and remainder as a structure of type lldiv_t.
lldiv_t lldiv(long long numer, long long denom);
