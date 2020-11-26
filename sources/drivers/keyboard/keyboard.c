#include "keyboard.h"
#include "misc.h"
#include "terminal.h"   //only for DEBUG purposes

uint8_t PS2_ctrl_config_byte(   uint8_t intr_1,         //first port interrupts
                                uint8_t intr_2,         //second port interrupts
                                uint8_t sys_flag,       //system flag
                                uint8_t clk_1,          //first port clock
                                uint8_t clk_2,          //second port clock
                                uint8_t translation)    //translation on first port
{
    uint8_t control_byte = intr_1;
}

uint8_t PS2_ctrl_test(void)
{
    outb(PS2_STATUS, 0xAD);         //0xAD is command - disable first PS2 port
    outb(PS2_STATUS, 0xAA);         //0xAA is command - test PS2 controller
    
    uint8_t status = inb(PS2_DATA); //if everything is OK we should get 0x55
    
    term_print("PS2 controller status ");   //DEBUG
    term_print_int32(status);               //DEBUG
    term_print("\n");                       //DEBUG

    return status;
}

void PS2_ctrl_flush_out_buff(void)
{
    outb(PS2_STATUS, 0xAD);                         //0xAD is command - disable first PS2 port
    for(uint8_t i = 0; i < 8; ++i) inb(PS2_DATA);   //reading data from PS2 to clean output buffer
}