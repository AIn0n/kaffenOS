;inspired by: https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial

global switch_task
extern current_task_esp
extern scheduler


switch_task:

    pushad
    pushfd

    mov [current_task_esp + 40], esp
    call scheduler
    mov esp, [current_task_esp + 40]

    popfd
    popad

    ret