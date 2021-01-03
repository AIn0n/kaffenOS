#include <stddef.h>
#include <stdint.h>
#include "desc_tabs.h"
#include "terminal.h"
#include "timer.h"
#include "keyboard.h"
#include "pshell.h"
#include "threads.h"


//DEBUG
int foo(void * ptr)
{
    term_print("boo");
    return 0;
}

void main (void) 
{
    term_init();
    term_print("$11NIEDZIELA WIECZOR I $12HUMOR POPSUTY\n");
    term_print("  _             __    __                 ___    ___ \n | |__  __ _   / _|  / _|  ___   _ _    / _ \\  / __|\n | / / / _` | |  _| |  _| / -_) | ' \\  | (_) | \\__ \\\n |_\\_\\ \\__,_| |_|   |_|   \\___| |_||_|  \\___/  |___/\n");
    term_print("idt, gdt: ");
    init_descriptor_tables();
    term_print("$10OK!\n");
    //asm volatile ("int $0x03");     //DEBUG
    term_print("PS/2: ");
    if(!PS2_init()) term_print("$10OK!\n");
    init_timer(1, 0, 3, 3);        //timer init only in DEBUG purposes
    term_print("PIT:");
    term_print("$10OK\n");
    multitasking_init();
    thread_create(foo);

    psh_loop();
}