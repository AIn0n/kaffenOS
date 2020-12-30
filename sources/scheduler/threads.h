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
    thread_state thr_state;
    uint8_t priority;

} thread_control_block;

#define THREAD_QUEUE_SIZE 256

typedef struct 
{
    thread_control_block list[THREAD_QUEUE_SIZE];
    uint32_t current_thread_idx;
} thread_queue_t;


//-------------------------------------FUNCS in C--------------------------------------------

void scheduler();
void multitasking_init();

//-------------------------------------FUNCS in assembly-------------------------------------

extern void switch_task();

#endif