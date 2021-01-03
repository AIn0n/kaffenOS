;inspired by: https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial

global switch_task          ;function from here (used in pit.c)
extern current_task_esp     ;current task stack
extern scheduler            ;scheduler written in C (file: threads.c)


switch_task:

    push ebx
    push esi
    push edi
    push ebp

    mov [current_task_esp], esp
    call scheduler
    mov esp, [current_task_esp]

    pop ebp
    pop edi
    pop esi
    pop ebx

    ret