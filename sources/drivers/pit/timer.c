#include "timer.h"
#include "isr.h"
#include "terminal.h"
#include "asm_c_funcs.h"

#define PIT_IN_FREQ 1193180
#define PIT_WRITE_PORT 0x43
#define PIT_CHNL_0 0x40

uint32_t tick = 0;

static void timer_callback(registers_t regs)
{
    ++tick;
    term_print_int32(tick);     //DEBUG
    term_print("\n");           //DEBUG
}

void init_timer(uint32_t freq, uint8_t channel, uint8_t access, uint8_t mode)
{
    if(channel > 2 || access > 3 || mode > 5) return;
    if(channel == 0)register_interrupt_handlers(IRQ0, &timer_callback);

    uint16_t divisor = PIT_IN_FREQ / freq;
    uint8_t command = mode<<1 | access<<4 | channel<<6;

    outb(PIT_WRITE_PORT, command);

    //divisor is splitted for two bytes
    uint8_t l = (uint8_t)(divisor & 0xFF);          //low part
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);   //high part

    //sending divisor to initialized channel
    outb((PIT_CHNL_0 + channel), l);
    outb((PIT_CHNL_0 + channel), h);
}