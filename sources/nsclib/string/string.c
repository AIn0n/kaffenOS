#include "string.h"
#include "terminal.h" //only for DEBUG

int32_t
strlen(const char *str)
{
    if(str == NULL) return -1;
    int i = 0;
    while(str[i]) ++i;
    return i;
}

char* 
strcpy(char *dest, const char *src)
{
    if(dest == NULL || src == NULL) return NULL;
    int32_t src_len = strlen(src);
    if(src_len == -1) return NULL;
    for(int32_t i = 0; i < src_len; ++i) dest[i] = src[i];
    return dest;
}

int8_t 
strcmp(const char *str, const char *str2)
{
    if(str == NULL || str2 == NULL) return 0;
    int32_t str_len = strlen(str), str2_len = strlen(str2);
    int32_t len = (str_len > str2_len) ? str2_len : str_len;
    for(int32_t i = 0; i < len; ++i)
    {
        if(str[i] != str2[i])
            return (str[i] < str2[i]) ? -1 : 1;
    }
    return 0;
}

void*
memset(void *ptr, uint8_t val, uint32_t size)
{
    if(ptr == NULL) return NULL;
    uint8_t *byte_ptr = ptr;
    for(int32_t i = 0; i < size; ++i) byte_ptr[i] = val;
    return ptr;
}

//this func returns pointer to first whitespace 
//char index from string
//in case of wrong data it returns NULL
char *
strwhitespace(char *str)
{
    if(str == NULL) return NULL;
    int32_t size = strlen(str);
    if(size == -1) return NULL;
    for(int32_t i = 0; i < size; ++i)
    {
        if(str[i] == ' ' || str[i] == '\t' || str[i] == '\n') return (str + i);
    }
    return NULL;
}