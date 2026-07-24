
#include "idt.h"
#include "../drivers/io/vgatxt.h"
#include "../builtins/stdint.h"
#include "pic.h"
#include "../drivers/io/keyboard.h"

_Static_assert(sizeof(struct idt_entry) == 8, "IDT entry wrong size");
_Static_assert(sizeof(struct idt_pointer) == 6, "IDT entry wrong size");

struct idt_entry idt[256];

struct idt_entry generate_idt_entry(uint32_t handler_address, unsigned int dpl, unsigned int gate_type)
{
    struct idt_entry entry;
    uint8_t masked_dpl = dpl & 0x3; //lowest two bits
    uint8_t masked_gate_type = gate_type & 0xF; //lowest four bits
    uint8_t attr = 0x00;    // two hex digits -> one byte -> eight bits as above
    attr |= 0x1 << 7; // present bit
    attr |= masked_dpl << 5;
    attr |= masked_gate_type;
    entry.attributes = attr;
    entry.offset_low = handler_address & 0xFFFF;
    entry.offset_high = (handler_address >> 16) & 0xFFFF;
    entry.reserved = 0;
    entry.segment_selector = 0x08;
    return entry;
}

void testHandler(uint32_t vector)
{
    vgatxt_write_u64(vector, 16, true, false);
}

void interrupt_dispatcher(uint32_t vector, struct interrupt_frame *frame)
{
    switch(vector)
    {
        case 0x30:
            testHandler(vector);
            break;
        case 0x21:
            keyboard_interrupt_handler();
            pic_send_eoi(1);
            break;
        case 0x20:
            pic_send_eoi(0);
            break;
    }
}

void init_idt()
{
    init_pic();
    enable_irq(1);
    enable_irq(0);
    for (int i = 0; i < 256; i++)
    {
        idt[i].offset_low = 0;
        idt[i].segment_selector = 0;
        idt[i].reserved = 0;
        idt[i].attributes = 0;
        idt[i].offset_high = 0;
    }
    struct idt_entry entry = generate_idt_entry((uint32_t)&interrupt_stub_30, 0x0, 0xE);
    idt[0x30] = entry;
    struct idt_entry timer = generate_idt_entry((uint32_t)&interrupt_stub_20, 0x0, 0xE);
    idt[0x20] = timer;
    struct idt_entry keyboard = generate_idt_entry((uint32_t)&interrupt_stub_21, 0x0, 0xE);
    idt[0x21] = keyboard;
    struct idt_pointer ptr;
    ptr.limit = sizeof(idt) - 1;
    ptr.base = (uint32_t)&idt[0];
    vgatxt_writestring("loading IDT\n");
    load_idt(&ptr);
    vgatxt_writestring("IDT loaded! \n");
}
