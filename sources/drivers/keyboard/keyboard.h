#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

//PORTS
#define PS2_DATA 0x60
#define PS2_STATS_CMD 0x64

//VALUES

//--------------stack-----------------
#define KBD_QUEUE_SIZE 512

//STRUCTS

//--------------stack----------------

//FILO stack similar to circular buffer
typedef struct
{
    uint8_t     array[KBD_QUEUE_SIZE];
    uint8_t     is_full;
    uint16_t    begin;
    uint16_t    end;
    uint16_t    size;
} kbd_stack_t;

//FUNCS

//--------------primitive IO--------------
uint8_t kbd_getchar(uint8_t *flags);

//-------------low level-------------
uint8_t PS2_init(void);
#endif 