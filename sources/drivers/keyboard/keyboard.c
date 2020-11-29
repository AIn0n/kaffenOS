#include "keyboard.h"
#include "misc.h"
#include "terminal.h"   //only for DEBUG purposes
#include "isr.h"

/*this function creates proper configurtion byte for 8042 but it's little
bit overkill for now, I let her here for maybe futher development of os */
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


/*  func in case of reading data (used with 'r' flag ) from 8042 check
    if buffor is full. In case of writing data ('w' flag) checks if RAM
    inside 8042 is empty */
static
void PS2_ctrl_status_wait(char mode)
{
    for(uint16_t i = 0; i < 65535; ++i)
    {
        if(mode == 'r') { if(GET_BYTE(inb(PS2_STATS_CMD), 0)) return; }
        else            { if(!GET_BYTE(inb(PS2_STATS_CMD),1)) return; }
    }
}

/* this func write command to command port of 8042 */
static
void PS2_ctrl_write_cmd(uint8_t cmd)
{
    PS2_ctrl_status_wait('w');
    outb(PS2_STATS_CMD, cmd);
}

/* this one is for reading data port */
static 
uint8_t PS2_ctrl_read_data(void)
{
    PS2_ctrl_status_wait('r');
    return inb(PS2_DATA);
}

static 
void PS2_handler(registers_t regs)
{
    term_print("Pressed: ");    //DEBUG
    term_print_int32(PS2_ctrl_read_data());
    term_print("\n");           //DEBUG too
}

uint8_t PS2_init(void)
{    
    //turning off devices
    PS2_ctrl_write_cmd(0xAD);   //0xAD - disable first PS2 device 
    PS2_ctrl_write_cmd(0xA7);   //0xA7 - disable second PS2 device
    
    //flushing buffer
    PS2_ctrl_read_data();

    //setting controller config byte
    PS2_ctrl_write_cmd(0x60);                   //0x60 - write byte from data port to internal ram        
    PS2_ctrl_status_wait('w');
    outb(PS2_DATA, PS2_ctrl_config_byte(TRUE    ,FALSE  ,TRUE    ,TRUE   ,FALSE  ,FALSE));
   
    //controller selftest
    PS2_ctrl_write_cmd(0xAA);                  //0xAA - test PS2 controller
    if(PS2_ctrl_read_data() != 0x55) return 1; //if everything is OK we should get 0x55

    //first port test
    PS2_ctrl_write_cmd(0xAB);                   //0xAB - test first PS/2 port
    if(PS2_ctrl_read_data() != 0x00) return 1;  //if everything is OK we should get 0x00

    //enable first device
    PS2_ctrl_write_cmd(0xAE);                   //0xAE - enable first PS/2 port

    //add interrupt handler for keyboard
    register_interrupt_handlers(IRQ1, &PS2_handler);

    return 0;
}