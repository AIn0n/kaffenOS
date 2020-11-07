#ifndef _DESC_TABS_H_
#define _DESC_TABS_H_
#include <stdint.h>
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
//segment lengtd                            3 - 0

struct gdt_ptr_struct
{
   uint16_t limit;               // The upper 16 bits of all selector limits.
   uint32_t base;                // The address of the first gdt_entry_t struct.
}
 __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t; 

void init_descriptor_tables();

#endif