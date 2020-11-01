.set MB_MAGIC, 0x1BADB002   //magic number for multiboot
.set MB_FLAGS, (1 << 0) | (1 << 1)
.set MB_CHECKSUM, -(MB_MAGIC + MB_FLAGS)

.extern kernel_main //name of main C function
.global _start       //global to made it accesible from outside this file

//setting up multiboot header
.section .multiboot
    .align 4
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

.section .bss
    .align 16
    stack_bottom:
        .skip 16384  //reserved 16kb for stack
    stack_top:

.section .text
    _start:
        mov $stack_top, %esp    //setting up stack
        call kernel_main        //running kernel

        hang:
            cli
            hlt
            jmp hang
