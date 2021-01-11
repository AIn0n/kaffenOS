#include <stddef.h>
#include <stdint.h>
#include "desc_tabs.h"
#include "terminal.h"
#include "timer.h"
#include "keyboard.h"
#include "pshell.h"
#include "threads.h"

//DEBUG
semaphore_t semaphore = SEMAPHORE_INIT(1);

int foo(void * ptr)
{
    for(uint32_t counter = 0;counter < 100; ++counter)
    {
        semaphore_lock(semaphore);
        term_print_int_at(counter, 10, 20, 10);
        semaphore_unlock(semaphore);
        sleep(500);

    }
    return 0;
}

int boo(void * ptr)
{
    for(uint32_t counter = 0;counter < 100; ++counter)
    {
        semaphore_lock(semaphore);
        term_print_int_at(counter, 10, 20, 11);
        semaphore_unlock(semaphore);
        sleep(1000);
    }
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
    thread_create(boo);

    //-----------------------------------------shell----------------------------------------------
    psh_loop();
}