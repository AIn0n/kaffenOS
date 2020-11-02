#include <stddef.h>
#include_next <stdint.h>

//GDT
struct gdt_entry_struct
{
    uint16_t limit_low; //lower 16bits of the limit
    uint16_t base_low; //lower 16bits of the base
    uint8_t base_mid; //middle byte of the base
    uint8_t access; //access flag to determine which ring can use this
    uint8_t granularity;
    uint8_t base_high; //last one byte of the base
}__attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

//granularity format (in bits)
//                  zero        one         indx
//granularity       (1 byte     1 kbyte)    7
//opperand size     (16bit      32bit)      6
//should always be zero                     5
//available for system(always zero)         4

struct gdt_ptr_struct
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

static void gdt_set_gate(   int         num, 
                            uint32_t    base, 
                            uint32_t    limit, 
                            uint8_t     access, 
                            uint8_t     gran)
{
    gdt_entries[num].base_low       = (base & 0xFFFF);
    gdt_entries[num].base_mid       = (base >> 16) & 0xFF;
    gdt_entries[num].base_high      = (base >> 24) & 0xFF;
    
    gdt_entries[num].limit_low      = (limit & 0xFFFF);

    gdt_entries[num].granularity    = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity    |= gran & 0xF0;

    gdt_entries[num].access = access;
}

extern void gdt_flush(uint32_t ptr)
{
    __asm__(
        "mov eax, [esp + 4]\n"
        "lgdt [eax]\n"
        "mov ds, ax\n"
        "mov es, ax\n"
        "mov fs, ax\n"
        "mov gs, ax\n"
        "mov ss, ax\n"
        "ret\n"
    );
}

static void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t) &gdt_entries;

    gdt_set_gate(0, 0, 0, 0 , 0);       //NULL
    gdt_set_gate(1, 9, 0xFFFFFFFF, 0x9A, 0xCF);    //krnl code segm
    gdt_set_gate(2, 9, 0xFFFFFFFF, 0x92, 0xCF);    //krnl data segm
    gdt_set_gate(3, 9, 0xFFFFFFFF, 0xFA, 0xCF);    //user code segm
    gdt_set_gate(4, 9, 0xFFFFFFFF, 0xF2, 0xCF);    //user data segm

    gdt_flush((uint32_t) &gdt_ptr);
}



//for vga display
volatile uint16_t* vga_buffer = (uint16_t *) 0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

//for terminal
int term_col;
int term_row;

static inline uint16_t vga_entry(unsigned char uc, uint8_t fg, uint8_t bg)
{
    uint16_t color = fg | bg << 4;
    return (uint16_t) color << 8 | (uint16_t) uc;
};

void term_init()
{
    term_col = 0; term_row = 0;
    for(int col = 0; col < VGA_COLS; ++col)
    {
        for(int row = 0; row < VGA_ROWS; ++row)
        {
            const size_t index = (VGA_COLS * row) + col;
            //VGA_CHAR BBBBFFFFCCCCCCCC, B-background F-foreground C-ASCII char
            vga_buffer[index] = vga_entry(' ', 0, 0);
            
        }
    }
};

void term_putc(char c)
{
    switch(c)
    {
    case '\n':
        {
            term_col = 0;
            ++term_row;
            break;
        }
    default:
        {
            const size_t index = (VGA_COLS * term_row) + term_col;
            vga_buffer[index] = vga_entry(c, 15, 0);
            ++term_col;
            break;
        }
    }
    if(term_col >= VGA_COLS){   term_col =0;    ++term_row;     }
    if(term_row >= VGA_ROWS){   term_col = 0;   term_row = 0;   }
};

void term_print(const char* str){
    for(size_t i = 0; str[i] != '\0'; ++i) term_putc(str[i]);
}

void kernel_main (void) 
{
    term_init();
    term_print("kaffenOS\nNIEDZIELA WIECZOR I HUMOR POPSUTY\n");

    init_gdt();
    term_print("gdt initialized\n");
}