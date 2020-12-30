#include "threads.h"
#include "terminal.h"
#include "isr.h"

//--------------------------------FIRST THREAD FUNCS------------------------------


uint32_t current_task_esp;

extern void switch_task();

void scheduler(registers_t *curr)
{
    term_print("foo\n");
}