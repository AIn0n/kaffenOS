#ifndef _MISC_H_
#define _MISC_H_
#include <stdint.h>

//addresses
#define NULL ((void *)0)

//values
#define TRUE 1
#define FALSE 0

//shorts funcs
#define SET_BYTE(var, n, bool)  ((var)|=((bool)<<(n)))
#define GET_BYTE(var, n)        (((var)&((0)|(1)<<(n))) ? 1 : 0)

//masks
#define B_MASK 0xFFFF   //2 byte mask
#define S_MASK 0xFF     //1 byte mask

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif