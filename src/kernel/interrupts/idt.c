
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
        case 0x00:
            kernel_panic(vector);
            break;
        case 0x01:
            kernel_panic(vector);
            break;
        case 0x02:
            kernel_panic(vector);
            break;
        case 0x04:
            kernel_panic(vector);
            break;
        case 0x05:
            kernel_panic(vector);
            break;
        case 0x06:
            kernel_panic(vector);
            break;
        case 0x07:
            kernel_panic(vector);
            break;
        case 0x08:
            kernel_panic(vector);
            break;
    }
}

void kernel_panic(uint32_t vector)
{
    vgatxt_set_color(vgatxt_COLOR_WHITE, vgatxt_COLOR_BLUE);
    vgatxt_initialize();
    vgatxt_set_entry(10, 29);
    disable_cursor();
    vgatxt_writestring("KERNEL PANIC OCCURED\n");
    vgatxt_set_entry(12, 29);
    switch(vector)
    {
        case 0x00:
            vgatxt_writestring("DIVIDE BY ZERO\n");
            break;
        case 0x01:
            vgatxt_writestring("DEBUG INTERRUPT\n");
            break;
        case 0x02:
            vgatxt_writestring("NONMASKABLE\n");
            break;
        case 0x04:
            vgatxt_writestring("OVERFLOW\n");
            break;
        case 0x05:
            vgatxt_writestring("BOUND RANGE EXCEEDED\n");
            break;
        case 0x06:
            vgatxt_writestring("INVALID OPCODE\n");
            break;
        case 0x07:
            vgatxt_writestring("DEVICE UNAVAILABLE\n");
            break;
        case 0x08:
            vgatxt_writestring("DOUBLE FAULT\n");
            break;
    }
    while (1)
    {
        asm volatile("hlt");
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
    struct idt_entry isr00 = generate_idt_entry((uint32_t)&interrupt_stub_00, 0x0, 0xE);
    idt[0x00] = isr00;
    struct idt_entry isr01 = generate_idt_entry((uint32_t)&interrupt_stub_01, 0x0, 0xE);
    idt[0x01] = isr01;
    struct idt_entry isr02 = generate_idt_entry((uint32_t)&interrupt_stub_02, 0x0, 0xE);
    idt[0x02] = isr02;
    struct idt_entry isr04 = generate_idt_entry((uint32_t)&interrupt_stub_04, 0x0, 0xE);
    idt[0x04] = isr04;
    struct idt_entry isr05 = generate_idt_entry((uint32_t)&interrupt_stub_05, 0x0, 0xE);
    idt[0x05] = isr05;
    struct idt_entry isr06 = generate_idt_entry((uint32_t)&interrupt_stub_06, 0x0, 0xE);
    idt[0x06] = isr06;
    struct idt_entry isr07 = generate_idt_entry((uint32_t)&interrupt_stub_07, 0x0, 0xE);
    idt[0x07] = isr07;
    struct idt_entry isr08 = generate_idt_entry((uint32_t)&interrupt_stub_08, 0x0, 0xE);
    idt[0x08] = isr08;
    struct idt_pointer ptr;
    ptr.limit = sizeof(idt) - 1;
    ptr.base = (uint32_t)&idt[0];
    vgatxt_writestring("loading IDT\n");
    load_idt(&ptr);
    vgatxt_writestring("IDT loaded! \n");
}
