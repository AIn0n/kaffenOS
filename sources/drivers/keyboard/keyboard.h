#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#define PS2_DATA 0x60
#define PS2_STATS_CMD 0x64

uint8_t PS2_init(void);
#endif 