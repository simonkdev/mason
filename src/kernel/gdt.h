#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr;

void gdt_init(void);

static void gdt_load(gdtr* gdtr_ptr);

#endif