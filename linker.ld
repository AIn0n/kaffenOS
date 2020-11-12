ENTRY(start)

SECTIONS
{
    /*kernel is loaded after 1M of memory*/
    . = 1M;

    /*everything aligned to multiples of 4kB */
    /*exec code*/
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        text = .;
        *(.text)
    }

    
    .data BLOCK(4K) : ALIGN(4K)
    {
        /* Read-write data (init) */
        *(.data)
        /* read only data */
        *(.rodata)
    }

    /*Read-write data (uninit) */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        bss = .;
        *(COMMON)
        *(.bss)
    }
    end = .;
}