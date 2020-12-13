#ifndef _STRING_H_
#define _STRING_H_
#include <stdint.h>
#include "misc.h"

int32_t strlen(const char *str);
char* strcpy(char *dest, const char *src);
void* memset(void *ptr, uint8_t val, uint32_t size);
int8_t strcmp(const char *str, const char *str2);
int32_t strwhitespace(const char *str);

#endif