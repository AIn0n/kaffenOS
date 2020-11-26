#include "keyboard.h"
#include "misc.h"
#include "terminal.h"   //only for DEBUG purposes

void test_PS2_controller()
{
    outb(PS2_STATUS, 0xAD);
    outb(PS2_STATUS, 0xAA);    
    term_print("PS2 controller status ");
    uint8_t status = inb(PS2_DATA);
    term_print_int32(status);
    term_print("\n");
}