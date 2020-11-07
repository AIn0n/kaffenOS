
;mulitboot config
MBOOT_PAGE_ALIGN equ 1<<0
MBOOT_MEM_INFO equ 1<<1

MBOOT_HEADER_MAGIC equ 0x1BADB002
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

MBOOT_CHECKSUM equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]

[GLOBAL multiboot]
[EXTERN text]
[EXTERN bss]
[EXTERN end]

multiboot:
    dd MBOOT_HEADER_MAGIC

    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM

    dd multiboot
    dd text
    dd bss
    dd end
    dd start

[GLOBAL start]
[EXTERN main]

start:
    push ebx    ;load multiboot header

    ;exec the kernel
    cli
    call main
    jmp $