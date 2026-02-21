#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "gdt.h"
#include "kernel.h"

#define SEG_G_FLAG(x) ((x) << 11) // Granularity flag (bit 11)
#define SEG_DB_FLAG(x) ((x) << 10) // Size flag (bit 10)
#define SEG_L_FLAG(x) ((x) << 9) // Long mode flag (bit 9)

#define SEG_P_FLAG(x) ((x) << 7) // Present flag (bit 7)
#define SEG_DPL_FLAG(x) ((x) << 5) // Descriptor Privilege Level (bits 5-6)
#define SEG_S_FLAG(x) ((x) << 4) // Descriptor type (bit 4)
#define SEG_E_FLAG(x) ((x) << 3) // Executable flag (bit 3)
#define SEG_DC_FLAG(x) ((x) << 2) // Direction/Conforming flag (bit 2)
#define SEG_RW_FLAG(x) ((x) << 1) // Readable/Writable flag
#define SEG_A_FLAG(x) ((x)) // Accessed flag (bit 0)

#define GDT_ENTRY_NULL 0
#define GDT_ENTRY_KCODE_FLAGS (SEG_G_FLAG(1) | SEG_DB_FLAG(1) | SEG_L_FLAG(0) | SEG_P_FLAG(1) | SEG_DPL_FLAG(0) | SEG_S_FLAG(1) | SEG_E_FLAG(1) | SEG_DC_FLAG(0) | SEG_RW_FLAG(1) | SEG_A_FLAG(1))  
#define GDT_ENTRY_KDATA_FLAGS (SEG_G_FLAG(1) | SEG_DB_FLAG(1) | SEG_L_FLAG(0) | SEG_P_FLAG(1) | SEG_DPL_FLAG(0) | SEG_S_FLAG(1) | SEG_E_FLAG(0) | SEG_DC_FLAG(0) | SEG_RW_FLAG(1) | SEG_A_FLAG(1))
#define GDT_ENTRY_COUNT 3

typedef struct {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) gdtr;

uint64_t assemble_gdt_entry(uint32_t base, uint32_t limit, uint16_t flags)
{
    uint64_t descriptor = 0;

    descriptor |= (base & 0xFF000000); // set base bits 24-31
    descriptor |= (flags & 0b111000000000); //set flag (flags 8-11)                 //set granularity (flags bit 11)
    
    descriptor |= (limit & 0x000F0000); // set limit bits 16-19

    descriptor |= (flags & 0b000011111111); // set access flags (bits 0-7)

    descriptor |= (base & 0xFF0000); // set base bits 16-23
    descriptor = descriptor << 32; // shift the base and flags to the upper 32 bits

    descriptor |= (base & 0x0000FFFF); // set base bits 0-15 and shift them to bits 16-31
    descriptor |= (limit & 0x0000FFFF); // set limit bits 0-15

    return descriptor;
}

extern void setGdt(uint16_t limit, uint32_t base);
extern void reloadSegments(void);

void gdt_init() {

    uint64_t gdt[3]; // GDT with 3 entries: null, kernel code, kernel data
    gdt[0] = GDT_ENTRY_NULL; // Null descriptor
    gdt[1] = assemble_gdt_entry(0x0000000, 0xFFFFF, GDT_ENTRY_KCODE_FLAGS); 
    gdt[2] = assemble_gdt_entry(0x0000000, 0xFFFFF, GDT_ENTRY_KDATA_FLAGS);

    uint16_t gdtr_limit = sizeof(gdt) - 1;
    uint32_t gdtr_base = (uint32_t)&gdt;

    terminal_writestring("GDT entries set up\n");

    setGdt(gdtr_limit, gdtr_base);
    reloadSegments();
    
    terminal_writestring("GDT set\n");
}


