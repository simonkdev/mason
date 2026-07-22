#include <stdint.h>

struct idt_entry {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t offset_high;
}__attribute__((packed));

struct idt_pointer {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));


extern void load_idt(struct idt_pointer*);
extern void interrupt_stub_30(void);


void testHandler(void);
void init_idt(void);
struct idt_entry generate_idt_entry(uint32_t handler_address, unsigned int dpl, unsigned int gate_type);
