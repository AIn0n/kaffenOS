#include <stddef.h>
#include_next <stdint.h>
#include "desc_tabs.h"
#include "terminal.h"



void main (void) 
{
    term_init();
    term_print("kaffenOS\nNIEDZIELA WIECZOR I HUMOR POPSUTY\n");
    init_descriptor_tables();
    term_print("gdt initialized");
}