#ifndef _DESC_TABS_H_
#define _DESC_TABS_H_
#include <stdint.h>
//GDT structs
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
//segment lengtd                            3 - 0

struct gdt_ptr_struct
{
   uint16_t limit;               // The upper 16 bits of all selector limits.
   uint32_t base;                // The address of the first gdt_entry_t struct.
}
 __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t; 

//IDT structs

struct idt_entry_struct
{
   uint16_t base_lo;             // The lower 16 bits of the address to jump to 
   uint16_t sel;                 // Kernel segment selector.
   uint8_t  always0;             // This must always be zero.
   uint8_t  flags;               // More flags.
   uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
   uint16_t limit;
   uint32_t base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

//funcs

void init_descriptor_tables();

//access to adresses of isr
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

#endif