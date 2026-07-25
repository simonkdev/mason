#include "../builtins/stdint.h"

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

struct interrupt_frame {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
}__attribute__((packed));

extern void load_idt(struct idt_pointer*);
extern void interrupt_stub_30(void);
extern void interrupt_stub_21(void);
extern void interrupt_stub_20(void);
extern void interrupt_stub_00(void);
extern void interrupt_stub_01(void);
extern void interrupt_stub_02(void);
extern void interrupt_stub_04(void);
extern void interrupt_stub_05(void);
extern void interrupt_stub_06(void);
extern void interrupt_stub_07(void);
extern void interrupt_stub_08(void);


void interrupt_dispatcher(uint32_t vector, struct interrupt_frame *frame);
void testHandler(uint32_t vector);
void init_idt(void);
struct idt_entry generate_idt_entry(uint32_t handler_address, unsigned int dpl, unsigned int gate_type);
