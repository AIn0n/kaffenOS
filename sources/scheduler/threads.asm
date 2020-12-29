;inspired by: https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial

extern state_saver
global save_state

save_state:
    pusha
    pushf
    call state_saver
    popa
    popf
    ret