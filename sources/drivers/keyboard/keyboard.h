#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#define PS2_DATA 0x60
#define PS2_STATUS 0x64

uint8_t PS2_ctrl_test(void);
void PS2_ctrl_flush_out_buff(void);
uint8_t PS2_ctrl_config_byte(   uint8_t intr_1,         //first port interrupts
                                uint8_t intr_2,         //second port interrupts
                                uint8_t sys_flag,       //system flag
                                uint8_t clk_1,          //first port clock
                                uint8_t clk_2,          //second port clock
                                uint8_t translation);    //translation on first port
                                
#endif 