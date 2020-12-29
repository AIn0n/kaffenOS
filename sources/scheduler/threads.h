#ifndef _THREADS_H_
#define _THREADS_H_
#include <stdint.h>
#include "terminal.h"

//----------------------------------STRUCTS FOR THREAD------------------------------------

typedef struct
{
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t eflags;
   
} cpu_state_t;

typedef enum {
    TERMINATED = 0,
    WAITING = 1,
    RUNNING = 2,
    RUNNABLE = 3
} thread_state;

#define STACK_SIZE 4096

typedef struct {
    cpu_state_t cpu_state;
    uint8_t stack[STACK_SIZE];
    thread_state state; //current state of task
    uint32_t prior;

} thread_control_block;

extern void save_state();

#endif