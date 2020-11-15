#include <stddef.h>
#include <stdint.h>
#include "desc_tabs.h"
#include "terminal.h"
#include "timer.h"


void main (void) 
{
    term_init();
    term_print("kaffenOS\nNIEDZIELA WIECZOR I HUMOR POPSUTY\n");
    init_descriptor_tables();
    term_print("idt, gdt initialized\n");
    asm volatile ("int $0x03");     //DEBUG
    init_timer(50, 0, 3, 3);        //timer init - not exactly working yet
    term_print_int32(2137);         //DEBUG - int32 print working 
}