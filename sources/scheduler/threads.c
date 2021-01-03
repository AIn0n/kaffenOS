#include "threads.h"
#include "terminal.h"
#include "isr.h"

uint32_t current_task_esp;
thread_queue_t thrd_queue;

uint32_t cli_counter;

void cli()
{
    cli_counter++;
    asm volatile("cli");
}
void sti()
{
    cli_counter--;
    if(cli_counter == 0)
        asm volatile("sti");
}

extern void switch_task();

typedef struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;

    uint32_t eip1;
    uint32_t eip2;
    uint32_t eip3;
} context;

void scheduler()
{
    //something here is wrong and that's reason why it not working
    thrd_queue.list[thrd_queue.curr_idx].esp = current_task_esp;
    for(uint32_t curr = thrd_queue.begin; curr < thrd_queue.end; ++curr)
    {
        if(thrd_queue.list[curr].state == RUNNABLE && thrd_queue.curr_idx != curr)
        {
            thrd_queue.list[thrd_queue.curr_idx].state = RUNNABLE;
            thrd_queue.curr_idx = curr;
            thrd_queue.list[curr].state = RUNNING;
            current_task_esp = (uint32_t) thrd_queue.list[curr].esp;
            break;
        }
    }
}

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

//...or maybe here smth i crewed up, idk
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