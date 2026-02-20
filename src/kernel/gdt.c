#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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
#define GDT_ENTRY_KDATA (SEG_G_FLAG(1) | SEG_DB_FLAG(1) | SEG_L_FLAG(0) | SEG_P_FLAG(1) | SEG_DPL_FLAG(0) | SEG_S_FLAG(1) | SEG_E_FLAG(0) | SEG_DC_FLAG(0) | SEG_RW_FLAG(1) | SEG_A_FLAG(1))

uint64_t assemble_gdt_entry(uint32_t base, uint32_t limit, uint16_t flags)
{
    uint64_t descriptor = 0;

    descriptor |= (base & 0xFF000000); // set base bits 24-31
    descriptor |= (flags & 0b10000000000); //set granularity (flags bit 10)
    descriptor |= (flags & 0b01000000000); //set size (flags bit 9)
    descriptor |= (flags & 0b00100000000); //set long mode (flags bit 8)
    descriptor |= (limit & 0x000F0000); // set limit bits 16-19
    descriptor |= (flags & 0b00001111111); // set access flags (bits 0-7)
    descriptor |= (base & 00000000011111111000000000000000); // set base bits 16-23
    descriptor = descriptor << 32; // shift the base and flags to the upper 32 bits
    descriptor |= (base & 0x0000FFFF) << 16; // set base bits 0-15 and shift them to bits 16-31
    descriptor |= (limit & 0x0000FFFF); // set limit bits 0-15

    return descriptor;
}



