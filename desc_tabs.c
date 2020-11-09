#include "desc_tabs.h"
#include <stdint.h>
#define B_MASK 0xFFFF
#define S_MASK 0xFF
#define LIMIT 0xFFFFFFFF

extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr; 

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

uint8_t idt_set_flags(uint8_t present, uint8_t privilage_lvl)
{
    uint8_t ret = 14;       //bits between 4-1 are always 0b01110, 0xE, 0d14
    ret |= privilage_lvl << 5;
    ret |= (~~(present) << 7);    
    return ret;
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags /* | 0x60 */;
} 

void init_descriptor_tables()
{
    init_gdt();
}

