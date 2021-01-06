#include "threads.h"
#include "timer.h"

uint32_t current_task_esp;  //pointer to current task stack
thread_queue_t thrd_queue;  //queue with tasks

uint32_t cli_counter;       //this var stores number of cli calling

//---------------various tasks/ints blockers------------------------------
void cli()
{
    cli_counter++;
    asm volatile("cli");
}

void sti() {if(!(--cli_counter)) asm volatile("sti");}

void sleep(uint64_t ms)
{
    cli();
    uint32_t curr = thrd_queue.curr_idx;
    thrd_queue.list[curr].wakeup_time = (ms + get_time_since_boot());
    thrd_queue.list[curr].state = PAUSED;
    switch_task();
    sti();
}

//----------------------scheduler-----------------------------------------
void scheduler()
{
//wakeup some threads if wakeup_time < time_since_boot
    uint64_t curr_time = get_time_since_boot();
    for(uint32_t curr = thrd_queue.begin; curr < thrd_queue.end; ++curr)
    {
        if(thrd_queue.list[curr].state == PAUSED)
        {
            if(thrd_queue.list[curr].wakeup_time < curr_time)
            {
                thrd_queue.list[curr].wakeup_time = 0;
                thrd_queue.list[curr].state = RUNNABLE;
            }
        }
    }

//real scheduler
    thrd_queue.list[thrd_queue.curr_idx].esp = current_task_esp;
    for(uint32_t curr = thrd_queue.begin; curr < thrd_queue.end; ++curr)
    {
        if(thrd_queue.list[curr].state == RUNNABLE)
        {
            if(thrd_queue.list[thrd_queue.curr_idx].state == RUNNING)
                thrd_queue.list[thrd_queue.curr_idx].state = RUNNABLE;
            thrd_queue.curr_idx = curr;
            thrd_queue.list[curr].state = RUNNING;
            current_task_esp = (uint32_t) thrd_queue.list[curr].esp;
            break;
        }
    }
}

//--------------------------initialization-------------------------------
void multitasking_init()
{
    cli_counter = 0;
    cli();
    for(int32_t i = 0; i < THREAD_QUEUE_SIZE; ++i)
    {
        thrd_queue.list[i].state = TERMINATED;
        thrd_queue.list[i].priority = 0;
    }
    thrd_queue.list[0].state = RUNNING;
    thrd_queue.curr_idx = 0;
    thrd_queue.begin = 0;
    thrd_queue.end = 1;
    sti();
}

//------------------------------threads funcs--------------------------------------
void thread_kill(void)
{
    cli();
    thrd_queue.list[thrd_queue.curr_idx].state = TERMINATED;
    switch_task();
    sti();
}

uint8_t thread_create(int (*eip) (void *))
{
    if(thrd_queue.end > THREAD_QUEUE_SIZE) return 2;
    uint32_t curr = thrd_queue.end++;
    
    //manipulating with stack 
    thrd_queue.list[curr].esp =(uint32_t) thrd_queue.list[curr].stack;
    thrd_queue.list[curr].esp += STACK_SIZE;
    thrd_queue.list[curr].esp -= sizeof(context);

    //getting new context to the stack
    context *ctx = (context *) thrd_queue.list[curr].esp;
    ctx->edi = 0;
    ctx->esi = 0;
    ctx->ebx = 0;
    ctx->ebp = 0;
    ctx->eip1 = (uint32_t) sti;
    ctx->eip2 = (uint32_t) eip;
    ctx->eip3 = (uint32_t) thread_kill;
    thrd_queue.list[curr].state = RUNNABLE;
    return 0;
}