;inspired by: https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial

global switch_task
extern current_task_esp
extern scheduler


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