.extern kernel_main //name of main C function
.global start       //global to made it accesible from outside this file

.set MB_MAGIC, 0x1BADB002   //magic number for multiboot
.set MB_FLAGS, (1 << 0) | (1 << 1)
.set MB_CHECKSUM, (0 - (MB_MAGIC - MB_FLAGS))

//setting up multiboot header
.section .multiboot
    .align 4
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

.section .bss
    .align 16
    stack_bottom:
        .skip 4096  //reserved 4kb for stack
    stack_top:

.section .text
    start:
        mov $stack_top, %esp    //setting up stack
        call kernel_main        //running kernel

        hang:
            cli
            hlt
            jmp hang
