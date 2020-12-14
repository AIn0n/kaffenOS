#include <stddef.h>
#include <stdint.h>
#include "desc_tabs.h"
#include "terminal.h"
#include "timer.h"
#include "keyboard.h"
#include "pshell.h"

void main (void) 
{
    term_init();
    term_print("kaffenOS\nNIEDZIELA WIECZOR I HUMOR POPSUTY\n");
    init_descriptor_tables();
    term_print("idt, gdt initialized\n");
    //asm volatile ("int $0x03");     //DEBUG
    //init_timer(50, 0, 3, 3);        //timer init only in DEBUG purposes
    if(!PS2_init()) term_print("PS/2 controller initialized\n");
    psh_loop();
}