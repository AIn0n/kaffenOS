#include <stddef.h>
#include <stdint.h>
#include "desc_tabs.h"
#include "terminal.h"
#include "timer.h"
#include "keyboard.h"


void main (void) 
{
    term_init();
    term_print("kaffenOS\nNIEDZIELA WIECZOR I HUMOR POPSUTY\n");
    init_descriptor_tables();
    term_print("idt, gdt initialized\n");
    asm volatile ("int $0x03");     //DEBUG
    //init_timer(50, 0, 3, 3);        //timer init only in DEBUG purposes
    PS2_ctrl_test();
    PS2_ctrl_flush_out_buff();
    term_print("PS2 output buffer flushed\n");
}