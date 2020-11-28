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
    SET_BYTE(control_byte, 1, (intr_2) ? 1 : 0);
    SET_BYTE(control_byte, 2, (sys_flag) ? 1 : 0);
    SET_BYTE(control_byte, 3, 0);
    SET_BYTE(control_byte, 4, (clk_1) ? 0 : 1);
    SET_BYTE(control_byte, 5, (clk_2) ? 0 : 1);
    SET_BYTE(control_byte, 6, (translation) ? 1 : 0);
    SET_BYTE(control_byte, 7, 0);
    return control_byte;
}

void PS2_ctrl_status_wait(void)
{
    uint8_t status_reg = inb(PS2_STATUS);
    for(uint16_t i = 0; i < 65535; ++i)
    {
        status_reg = inb(PS2_STATUS);
        if(GET_BYTE(status_reg, 1)) return;
    }
}

uint8_t PS2_ctrl_test(void)
{
    PS2_ctrl_status_wait();
    outb(PS2_STATUS, 0xAD);         //0xAD is command - disable first PS2 port
    PS2_ctrl_status_wait();
    outb(PS2_STATUS, 0xAA);         //0xAA is command - test PS2 controller
    
    uint8_t status = inb(PS2_DATA); //if everything is OK we should get 0x55

    return status;
}

void PS2_ctrl_flush_out_buff(void)
{
    outb(PS2_STATUS, 0xAD);                         //0xAD is command - disable first PS2 port
    for(uint8_t i = 0; i < 8; ++i) inb(PS2_DATA);   //reading data from PS2 to clean output buffer
}