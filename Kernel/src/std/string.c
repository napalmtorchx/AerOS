#include <std/string.h>
int strlen(char* str)
{
    int i=0;
    while(str != '\0')
    {
        i+=1;
        str++;
    }
    return i;
}