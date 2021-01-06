#ifndef _THREADS_H_
#define _THREADS_H_
#include <stdint.h>

//------------------------------------CONSTS---------------------------------------------
#define STACK_SIZE 4096
#define THREAD_QUEUE_SIZE 256

//----------------------------------STRUCTS FOR THREAD------------------------------------

typedef enum {
    TERMINATED = 0,
    WAITING = 1,
    RUNNING = 2,
    RUNNABLE = 3,
    PAUSED = 4
} thread_state;

typedef struct {
    uint8_t stack[STACK_SIZE];
    uint32_t esp;
    thread_state state;
    uint8_t priority;
    uint64_t wakeup_time;
} thread_control_block;

typedef struct 
{
    thread_control_block list[THREAD_QUEUE_SIZE];
    uint32_t curr_idx;
    uint32_t begin;
    uint32_t end;
} thread_queue_t;

typedef struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;

    uint32_t eip1;
    uint32_t eip2;
    uint32_t eip3;
} context;

//-------------------------------------FUNCS in C--------------------------------------------
    
    //----------scheduling, init, etc------------
    void scheduler();
    void multitasking_init();
    
    //------------blocking scheduler-------------
    void sti();
    void cli();
    
    //--------------task funcs-------------------
    uint8_t thread_create(int (*eip) (void *));
    void sleep(uint64_t ms);

//-------------------------------------FUNCS in assembly-------------------------------------
extern void switch_task();

#endif