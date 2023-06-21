#pragma once
#include <lib/types.h>

#ifdef __cplusplus
extc {
#endif

/// @brief Calculates the length of a string. @param str Pointer to the null-terminated string. @return Length of the string.
size_t strlen(const char* str);

/// @brief Locates the first occurrence of a character in a string. @param str Pointer to the null-terminated string. @param c Character to be located. @return Pointer to the located character or NULL if not found.
char* strchr(const char* str, int c);

/// @brief Locates the last occurrence of a character in a string. @param str Pointer to the null-terminated string. @param c Character to be located. @return Pointer to the located character or NULL if not found.
char* strrchr(const char* str, int c);

/// @brief Compares two strings. @param str1 Pointer to the first null-terminated string. @param str2 Pointer to the second null-terminated string. @return Integer less than, equal to, or greater than zero if str1 is found to be less than, equal to, or greater than str2, respectively.
int strcmp(const char* str1, const char* str2);

/// @brief Compares at most a certain number of characters of two strings. @param str1 Pointer to the first null-terminated string. @param str2 Pointer to the second null-terminated string. @param n Maximum number of characters to compare. @return Integer less than, equal to, or greater than zero if str1 is found to be less than, equal to, or greater than str2, respectively.
int strncmp(const char* str1, const char* str2, size_t n);

/// @brief Copies a string. @param dest Pointer to the destination null-terminated string. @param src Pointer to the source null-terminated string. @return Pointer to the destination string.
char* strcpy(char* dest, const char* src);

/// @brief Copies at most a certain number of characters from a string. @param dest Pointer to the destination null-terminated string. @param src Pointer to the source null-terminated string. @param n Maximum number of characters to copy. @return Pointer to the destination string.
char* strncpy(char* dest, const char* src, size_t n);

/// @brief Concatenates two strings. @param dest Pointer to the destination null-terminated string. @param src Pointer to the source null-terminated string. @return Pointer to the destination string.
char* strcat(char* dest, const char* src);

/// @brief Copies a block of memory. @param dest Pointer to the destination memory block. @param src Pointer to the source memory block. @param n Number of bytes to copy. @return Pointer to the destination memory block.
void* memcpy(void* dest, const void* src, size_t n);

/// @brief Fills a block of memory with a constant byte. @param str Pointer to the memory block to fill. @param c Value to be set (as an int). @param n Number of bytes to fill. @return Pointer to the filled memory block.
void* memset(void* ptr, int c, size_t n);

/// @brief Compares two blocks of memory. @param str1 Pointer to the first memory block to compare. @param str2 Pointer to the second memory block to compare. @param n Number of bytes to compare. @return Integer less than, equal to, or greater than zero if str1 is found to be less than, equal to, or greater than str2, respectively.
int memcmp(const void* ptr1, const void* ptr2, size_t n);

/// @brief Moves a block of memory, handling overlap. @param dest Pointer to the destination memory block. @param src Pointer to the source memory block. @param n Number of bytes to move. @return Pointer to the destination memory block.
void* memmove(void* dest, const void* src, size_t n);

/// @brief Locates the first occurrence of a character in a block of memory. @param str Pointer to the memory block to search. @param c Character to be located. @param n Number of bytes to search. @return Pointer to the located character or NULL if not found.
void* memchr(const void* ptr, int c, size_t n);

#ifdef __cplusplus
}
#endif