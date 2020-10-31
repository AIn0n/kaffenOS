ENTRY(start)

SECTIONS
{
    /*kernel is loaded after 1M of memory*/
    . = 1M;

    /*everything aligned to multiples of 4kB */
    /*exec code*/
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
    }

    /* Read-only data*/
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Read-write data (init) */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /*Read-write data (uninit) */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}