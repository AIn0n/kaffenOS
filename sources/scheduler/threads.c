#include "threads.h"
#include "terminal.h"

//--------------------------------FIRST THREAD FUNCS------------------------------

cpu_state_t fthread_cpu;    //fthread cpu state

void state_saver(cpu_state_t cpu)
{
    fthread_cpu.edi = cpu.edi;
    fthread_cpu.esi = cpu.esi;
    fthread_cpu.ebp = cpu.ebp;
    fthread_cpu.esp = cpu.esp;
    fthread_cpu.ebx = cpu.ebx;
    fthread_cpu.edx = cpu.edx;
    fthread_cpu.ecx = cpu.ecx;
    fthread_cpu.eax = cpu.eax;
    fthread_cpu.eflags = cpu.eflags;
}