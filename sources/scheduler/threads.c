#include "threads.h"
#include "terminal.h"
#include "isr.h"

uint32_t current_task_esp;
thread_queue_t thrd_queue;

extern void switch_task();

void scheduler(registers_t *curr)
{
    //TODO
}

void multitasking_init()
{
    for(int32_t i = 0; i < THREAD_QUEUE_SIZE; ++i)
    {
        thrd_queue.list[i].thr_state = TERMINATED;
        thrd_queue.list[i].priority = 0;
    }
    thrd_queue.current_thread_idx = 0;
}