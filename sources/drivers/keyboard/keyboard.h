#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

//VALUES

//--------------stack-----------------
#define KBD_BUFF_SIZE 512

//PORTS
#define PS2_DATA 0x60
#define PS2_STATS_CMD 0x64

//STRUCTS
//--------------messege--------------

//struct which in future will tranfer data to userspace programs
typedef struct
{
    uint8_t ascii_char;
    uint8_t flag;
} kbd_msg_t;

//--------------stack----------------

//FILO stack similar to circular buffer
typedef struct
{
    uint8_t     array[KBD_BUFF_SIZE];
    uint8_t     is_full;
    uint16_t    begin;
    uint16_t    end;
    uint16_t    size;
} kbd_stack_t;

//FUNCS

//--------------messege--------------
uint8_t getchar(void);

//-------------low level-------------
uint8_t PS2_init(void);
#endif 