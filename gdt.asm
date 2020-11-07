[GLOBAL gdt_flush]

gdt_flush:
    mov eax, [esp+4]    ;getting pointer from C func (which is parameter)
    lgdt [eax]          ;Loading new gdt pointer
    mov ax, 0x10        ;offset of our data segment
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ;offest of our code segment
.flush:
    ret