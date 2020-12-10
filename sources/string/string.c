#include "string.h"

int32_t
strlen(const char *str)
{
    if(str == NULL) return -1;
    int i = 0;
    while(str[i]) ++i;
    return i;
}

char* 
my_strcpy(char *dest, const char *src)
{
    if(dest == NULL || src == NULL) return NULL;
    for(int32_t i = 0;src[i]; ++i) dest[i] = src[i];
    return dest;
}