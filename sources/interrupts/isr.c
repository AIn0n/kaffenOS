#include <stdint.h>
#include "isr.h"
#include "terminal.h"

void isr_handler(registers_t regs)
{
    term_print("unhandled interrupt");
}