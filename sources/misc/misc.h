#ifndef _MISC_H_
#define _MISC_H_
#include <stdint.h>

#define SET_BYTE(x, y) ((x)|=(1<<(y)))
#define B_MASK 0xFFFF   //2 byte mask
#define S_MASK 0xFF     //1 byte mask

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif