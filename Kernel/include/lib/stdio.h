#pragma once
#include <lib/types.h>

typedef struct
{
    char*  name;
    size_t sz;
} attr_pack file_t;
typedef file_t FILE;

/// @brief Prints formatted data to the standard output stream. @param format The format string. @return The number of characters printed.
int printf(const char *format, ...);

/// @brief Prints formatted data to the specified output stream. @param stream The output stream. @param format The format string. @return The number of characters printed.
int fprintf(FILE *stream, const char *format, ...);

/// @brief Stores formatted data in a string. @param str The destination string. @param format The format string. @return The number of characters stored.
int sprintf(char *str, const char *format, ...);

/// @brief Stores formatted data in a sized buffer. @param str The destination buffer. @param size The size of the buffer. @param format The format string. @return The number of characters that would have been written, excluding the null terminator.
int snprintf(char *str, size_t size, const char *format, ...);

/// @brief Writes formatted data to a character string. @param str The output character string. @param format The format string. @param ap The variable argument list. @return The number of characters written.
int vsprintf(char* str, const char* format, va_list ap);

/// @brief Writes formatted data to the standard output stream. @param format The format string. @param ap The variable argument list. @return The number of characters written.
int vprintf(const char* format, va_list ap);

/// @brief Reads formatted data from the standard input stream. @param format The format string. @return The number of input items successfully matched and assigned.
int scanf(const char *format, ...);

/// @brief Reads formatted data from the specified input stream. @param stream The input stream. @param format The format string. @return The number of input items successfully matched and assigned.
int fscanf(FILE *stream, const char *format, ...);

/// @brief Reads formatted data from a string. @param str The input string. @param format The format string. @return The number of input items successfully matched and assigned.
int sscanf(const char *str, const char *format, ...);

/// @brief Writes a character to the standard output stream. @param c The character to be written. @return The character written as an unsigned char cast to an int or EOF on error.
int putchar(int c);

/// @brief Reads a character from the standard input stream. @return The character read as an unsigned char cast to an int or EOF on end-of-file or error.
int getchar();

/// @brief Reads a line from the standard input stream. @param str The destination string. @return The input string or NULL on error or end-of-file.
char *gets(char *str);

/// @brief Writes a string to the standard output stream. @param str The string to be written. @return A non-negative value on success or EOF on error.
int puts(const char *str);

/// @brief Writes a character to the specified output stream. @param c The character to be written. @param stream The output stream. @return The character written as an unsigned char cast to an int or EOF on error.
int fputc(int c, FILE *stream);

/// @brief Writes a string to the specified output stream. @param str The string to be written. @param stream The output stream. @return A non-negative value on success or EOF on error.
int fputs(const char *str, FILE *stream);

/// @brief Reads a character from the specified input stream. @param stream The input stream. @return The character read as an unsigned char cast to an int or EOF on end-of-file or error.
int getc(FILE *stream);

/// @brief Writes a character to the specified output stream. @param c The character to be written. @param stream The output stream. @return The character written as an unsigned char cast to an int or EOF on error.
int putc(int c, FILE *stream);

/// @brief Pushes a character back onto the specified input stream. @param c The character to be pushed back. @param stream The input stream. @return The character pushed back or EOF on error.
int ungetc(int c, FILE *stream);

/// @brief Deletes a file. @param filename The name of the file to be deleted. @return 0 on success or nonzero on failure.
int remove(const char *filename);

/// @brief Renames a file. @param oldname The current name of the file. @param newname The new name for the file. @return 0 on success or nonzero on failure.
int rename(const char *oldname, const char *newname);

/// @brief Opens a file and associates a stream with it. @param filename The name of the file to be opened. @param mode The file access mode. @return A pointer to the opened stream or NULL on failure.
FILE *fopen(const char *filename, const char *mode);

/// @brief Closes the specified file stream. @param stream The file stream to be closed. @return 0 on success or EOF on error.
int fclose(FILE *stream);

/// @brief Reads data from a file stream into a buffer. @param ptr The buffer to store the read data. @param size The size of each element to be read. @param count The number of elements to read. @param stream The input stream. @return The total number of elements successfully read.
size_t fread(void *ptr, size_t size, size_t count, FILE *stream);

/// @brief Writes data from a buffer to a file stream. @param ptr The buffer containing the data to be written. @param size The size of each element to be written. @param count The number of elements to write. @param stream The output stream. @return The total number of elements successfully written.
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);

/// @brief Sets the file position indicator for the specified stream. @param stream The file stream. @param offset The offset to set. @param origin The starting position. @return 0 on success or nonzero on failure.
int fseek(FILE *stream, long int offset, int origin);

/// @brief Returns the current value of the file position indicator for the specified stream. @param stream The file stream. @return The current file position or -1L on error.
long int ftell(FILE *stream);

/// @brief Sets the file position indicator to the beginning of the specified stream. @param stream The file stream.
void rewind(FILE *stream);

/// @brief Sets the buffer for the specified stream. @param stream The file stream. @param buffer The buffer to be used.
void setbuf(FILE *stream, char *buffer);

/// @brief Sets the buffer and buffering mode for the specified stream. @param stream The file stream. @param buffer The buffer to be used. @param mode The buffering mode. @param size The size of the buffer.
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);

/// @brief Clears the error and end-of-file indicators for the specified stream. @param stream The file stream.
void clearerr(FILE *stream);

/// @brief Checks if the end-of-file indicator is set for the specified stream. @param stream The file stream. @return Nonzero if the end-of-file indicator is set, otherwise 0.
int feof(FILE *stream);

/// @brief Checks if the error indicator is set for the specified stream. @param stream The file stream. @return Nonzero if the error indicator is set, otherwise 0.
int ferror(FILE *stream);

/// @brief Prints an error message to the standard error stream. @param str The error message to be printed.
void perror(const char *str);
