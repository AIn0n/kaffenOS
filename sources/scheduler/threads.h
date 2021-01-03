#ifndef _THREADS_H_
#define _THREADS_H_
#include <stdint.h>

//----------------------------------STRUCTS FOR THREAD------------------------------------

typedef enum {
    TERMINATED = 0,
    WAITING = 1,
    RUNNING = 2,
    RUNNABLE = 3
} thread_state;

#define STACK_SIZE 4096

typedef struct {
    uint8_t stack[STACK_SIZE];
    uint32_t esp;
    thread_state state;
    uint8_t priority;

} thread_control_block;

#define THREAD_QUEUE_SIZE 256

typedef struct 
{
    thread_control_block list[THREAD_QUEUE_SIZE];
    uint32_t curr_idx;
    uint32_t begin;
    uint32_t end;
} thread_queue_t;


//-------------------------------------FUNCS in C--------------------------------------------

void scheduler();
void multitasking_init();
uint8_t thread_create(int (*eip) (void *));
void sti();
void cli();


//-------------------------------------FUNCS in assembly-------------------------------------

extern void switch_task();

#endif