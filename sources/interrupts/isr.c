#include <stdint.h>
#include <stddef.h>
#include "isr.h"
#include "terminal.h"
#include "asm_c_funcs.h"

//irq
#define PIC1 0x20       //pic master adress
#define PIC2 0xA0       //pic slave adress

isr_t interrupt_handlers[256];

void register_interrupt_handlers(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t regs)
{
    term_print("unhandled interrupt");
}

void irq_handler(registers_t regs)
{
    //first, lets check PIC2
    if(regs.int_no >= 40)
    {
        outb(PIC2, 0x20);   //send EOI signal to slave
    }
    outb(PIC1, 0x20);   //send EOI signal to master

    //does handler exist?
    if(interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}