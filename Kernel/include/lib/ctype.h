#pragma once
#include <lib/types.h>

/// @brief Checks if a character is alphanumeric. @param c Character to be checked. @return Non-zero value if the character is alphanumeric, zero otherwise.
int isalnum(int c);

/// @brief Checks if a character is alphabetic. @param c Character to be checked. @return Non-zero value if the character is alphabetic, zero otherwise.
int isalpha(int c);

/// @brief Checks if a character is a control character. @param c Character to be checked. @return Non-zero value if the character is a control character, zero otherwise.
int iscntrl(int c);

/// @brief Checks if a character is a digit. @param c Character to be checked. @return Non-zero value if the character is a digit, zero otherwise.
int isdigit(int c);

/// @brief Checks if a character has a graphical representation (excluding space). @param c Character to be checked. @return Non-zero value if the character has a graphical representation, zero otherwise.
int isgraph(int c);

/// @brief Checks if a character is a lowercase letter. @param c Character to be checked. @return Non-zero value if the character is a lowercase letter, zero otherwise.
int islower(int c);

/// @brief Checks if a character is printable (including space). @param c Character to be checked. @return Non-zero value if the character is printable, zero otherwise.
int isprint(int c);

/// @brief Checks if a character is a punctuation character. @param c Character to be checked. @return Non-zero value if the character is a punctuation character, zero otherwise.
int ispunct(int c);

/// @brief Checks if a character is a white-space character. @param c Character to be checked. @return Non-zero value if the character is a white-space character, zero otherwise.
int isspace(int c);

/// @brief Checks if a character is an uppercase letter. @param c Character to be checked. @return Non-zero value if the character is an uppercase letter, zero otherwise.
int isupper(int c);

/// @brief Checks if a character is a hexadecimal digit. @param c Character to be checked. @return Non-zero value if the character is a hexadecimal digit, zero otherwise.
int isxdigit(int c);

/// @brief Converts a character to lowercase. @param c Character to be converted. @return Lowercase equivalent of the character if it is an uppercase letter, otherwise returns the character itself.
int tolower(int c);

/// @brief Converts a character to uppercase. @param c Character to be converted. @return Uppercase equivalent of the character if it is a lowercase letter, otherwise returns the character itself.
int toupper(int c);