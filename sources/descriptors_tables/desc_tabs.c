#include "desc_tabs.h"
#include <stdint.h>
#include "asm_c_funcs.h"

//================= DEFINE SECTION ===================

//gdt
#define LIMIT 0xFFFFFFFF//4 byte mask
#define B_MASK 0xFFFF   //2 byte mask
#define S_MASK 0xFF     //1 byte mask

//irq
#define PIC1 0x20               //pic master adress
#define PIC2 0xA0               //pic slave adress
#define PIC1_DATA (PIC1 + 1)    //pic master data reg port
#define PIC2_DATA (PIC2 + 1)    //pic slave data reg port

#define ICW1_INIT 0x10          //initialization command
#define ICW1_ICW4 0x01          //set IC4 flag in init control word

extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr; 

// =========================== GDT ======================

static void gdt_set_gate(   uint32_t num,   uint32_t base,
                            uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low       = (base & B_MASK);
    gdt_entries[num].base_mid       = (base >> 16) & S_MASK;
    gdt_entries[num].base_high      = (base >> 24) & S_MASK;

    gdt_entries[num].limit_low      = (limit & B_MASK);
    gdt_entries[num].granularity    = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity    |= gran & 0xF0;
    gdt_entries[num].access         = access;

}

static void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t) &gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, LIMIT, 0x9A, 0xCF);
    gdt_set_gate(2, 0, LIMIT, 0x92, 0xFC);
    gdt_set_gate(3, 0, LIMIT, 0xFA, 0xCF);
    gdt_set_gate(4, 0, LIMIT, 0xF2, 0xCF);
    
    gdt_flush((uint32_t) &gdt_ptr);
}

//======================= IDT ======================================

uint8_t idt_set_flags(uint8_t present, uint8_t privilage_lvl)
{
    uint8_t ret = 14;               //bits between 4-1 are always 0b01110, 0xE, 0d14
    ret |= privilage_lvl << 5;      //bits 5-6 are pivilage level 
    ret |= (~~(present) << 7);      //last one signifies the entry is present
    return ret;
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
   idt_entries[num].base_lo = base & B_MASK;
   idt_entries[num].base_hi = (base >> 16) & B_MASK;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;

   idt_entries[num].flags   = flags;
}

void pic_remap(void)
{
    //ICW - first init word
    outb(PIC1, ICW1_INIT | ICW1_ICW4);
    outb(PIC1, ICW1_INIT | ICW1_ICW4);

    //ICW2 - vector offsets
    outb(PIC1_DATA, PIC1);                  
    outb(PIC2_DATA, (PIC1 + 0x08));         // 0x08 here is offset between pic and pic2

    //ICW3 - cascade PICs
    outb(PIC1_DATA, 4);     //we set up bit to show pic master which 
                            //irq line correspond to pic slave( irq2)
    outb(PIC2_DATA, 2);     //tell pic slave its cascade identity

    //ICW4 - modes/buffers/etc
    outb(PIC1_DATA, 1);     //setting first bit only to get 8086 mode
    outb(PIC2_DATA, 1);

    //clearing masks
    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 0);
}

static void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base = (uint32_t) &idt_entries;

    uint8_t *ptr = (uint8_t*) &idt_entries;
    uint16_t idt_size = sizeof(idt_entry_t) * 256;
    for(int i = 0; i < idt_size; ++i) ptr[i] = 0;

    pic_remap();

    idt_set_gate( 0, (uint32_t)isr0,  0x08, idt_set_flags(1, 0));
    idt_set_gate( 1, (uint32_t)isr1,  0x08, idt_set_flags(1, 0));
    idt_set_gate( 2, (uint32_t)isr2,  0x08, idt_set_flags(1, 0));
    idt_set_gate( 3, (uint32_t)isr3,  0x08, idt_set_flags(1, 0));
    idt_set_gate( 4, (uint32_t)isr4,  0x08, idt_set_flags(1, 0));
    idt_set_gate( 5, (uint32_t)isr5,  0x08, idt_set_flags(1, 0));
    idt_set_gate( 6, (uint32_t)isr6,  0x08, idt_set_flags(1, 0));
    idt_set_gate( 7, (uint32_t)isr7,  0x08, idt_set_flags(1, 0));
    idt_set_gate( 8, (uint32_t)isr8,  0x08, idt_set_flags(1, 0));
    idt_set_gate( 9, (uint32_t)isr9,  0x08, idt_set_flags(1, 0));
    idt_set_gate(10, (uint32_t)isr10, 0x08, idt_set_flags(1, 0));
    idt_set_gate(11, (uint32_t)isr11, 0x08, idt_set_flags(1, 0));
    idt_set_gate(12, (uint32_t)isr12, 0x08, idt_set_flags(1, 0));
    idt_set_gate(13, (uint32_t)isr13, 0x08, idt_set_flags(1, 0));
    idt_set_gate(14, (uint32_t)isr14, 0x08, idt_set_flags(1, 0));
    idt_set_gate(15, (uint32_t)isr15, 0x08, idt_set_flags(1, 0));
    idt_set_gate(16, (uint32_t)isr16, 0x08, idt_set_flags(1, 0));
    idt_set_gate(17, (uint32_t)isr17, 0x08, idt_set_flags(1, 0));
    idt_set_gate(18, (uint32_t)isr18, 0x08, idt_set_flags(1, 0));
    idt_set_gate(19, (uint32_t)isr19, 0x08, idt_set_flags(1, 0));
    idt_set_gate(20, (uint32_t)isr20, 0x08, idt_set_flags(1, 0));
    idt_set_gate(21, (uint32_t)isr21, 0x08, idt_set_flags(1, 0));
    idt_set_gate(22, (uint32_t)isr22, 0x08, idt_set_flags(1, 0));
    idt_set_gate(23, (uint32_t)isr23, 0x08, idt_set_flags(1, 0));
    idt_set_gate(24, (uint32_t)isr24, 0x08, idt_set_flags(1, 0));
    idt_set_gate(25, (uint32_t)isr25, 0x08, idt_set_flags(1, 0));
    idt_set_gate(26, (uint32_t)isr26, 0x08, idt_set_flags(1, 0));
    idt_set_gate(27, (uint32_t)isr27, 0x08, idt_set_flags(1, 0));
    idt_set_gate(28, (uint32_t)isr28, 0x08, idt_set_flags(1, 0));
    idt_set_gate(29, (uint32_t)isr29, 0x08, idt_set_flags(1, 0));
    idt_set_gate(30, (uint32_t)isr30, 0x08, idt_set_flags(1, 0));
    idt_set_gate(31, (uint32_t)isr31, 0x08, idt_set_flags(1, 0));

    idt_set_gate(32, (uint32_t)irq0,  0x08, idt_set_flags(1, 0));
    idt_set_gate(33, (uint32_t)irq1,  0x08, idt_set_flags(1, 0));
    idt_set_gate(34, (uint32_t)irq2,  0x08, idt_set_flags(1, 0));
    idt_set_gate(35, (uint32_t)irq3,  0x08, idt_set_flags(1, 0));
    idt_set_gate(36, (uint32_t)irq4,  0x08, idt_set_flags(1, 0));
    idt_set_gate(37, (uint32_t)irq5,  0x08, idt_set_flags(1, 0));
    idt_set_gate(38, (uint32_t)irq6,  0x08, idt_set_flags(1, 0));
    idt_set_gate(39, (uint32_t)irq7,  0x08, idt_set_flags(1, 0));
    idt_set_gate(40, (uint32_t)irq8,  0x08, idt_set_flags(1, 0));
    idt_set_gate(41, (uint32_t)irq9,  0x08, idt_set_flags(1, 0));
    idt_set_gate(42, (uint32_t)irq10, 0x08, idt_set_flags(1, 0));
    idt_set_gate(43, (uint32_t)irq11, 0x08, idt_set_flags(1, 0));
    idt_set_gate(44, (uint32_t)irq12, 0x08, idt_set_flags(1, 0));
    idt_set_gate(45, (uint32_t)irq13, 0x08, idt_set_flags(1, 0));
    idt_set_gate(46, (uint32_t)irq14, 0x08, idt_set_flags(1, 0));
    idt_set_gate(47, (uint32_t)irq15, 0x08, idt_set_flags(1, 0));

    idt_flush((uint32_t)&idt_ptr);
}


void init_descriptor_tables()
{
    init_gdt();
    init_idt();
}

