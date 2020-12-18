#ifndef _THREADS_H_
#define _THREADS_H_
#include <stdint.h>

typedef enum {
    SLEEP = 0,
    WORK = 1
} threat_state;

typedef struct {
    uint32_t esp;   //top of stack
    uint32_t ds;    //data segment
    void * next_task;   //currently void, it may changes
    threat_state state; //cussent state of task
    uint32_t prior;

} threat_control_block;

#endif