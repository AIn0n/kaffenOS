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
    sleep(25000);
    term_print("boo");
    return 0;
}

void main (void) 
{
    //------------------------------------terminal init-------------------------------------------
    term_init();

    //---------------------------------stupid welcome screen--------------------------------------
    term_print("$11NIEDZIELA WIECZOR I $12HUMOR POPSUTY\n");
    term_print("  _             __    __                 ___    ___ \n | |__  __ _   / _|  / _|  ___   _ _    / _ \\  / __|\n | / / / _` | |  _| |  _| / -_) | ' \\  | (_) | \\__ \\\n |_\\_\\ \\__,_| |_|   |_|   \\___| |_||_|  \\___/  |___/\n");
    
    //------------------------------------descriptors, interrupts, etc.---------------------------
    term_print("idt, gdt: ");
    init_descriptor_tables();
    term_print("$10OK!\n");
    
    //--------------------------------------PS/2 controller init----------------------------------
    term_print("PS/2: ");
    if(!PS2_init()) term_print("$10OK!\n");

    //-------------------------------------PIT and contexts switching init------------------------
    multitasking_init();
    init_timer(1000, 0, 3, 3);
    term_print("PIT:");
    term_print("$10OK\n");

    //----------------------------------------code done before shell------------------------------
    thread_create(foo);

    //-----------------------------------------shell----------------------------------------------
    psh_loop();
}