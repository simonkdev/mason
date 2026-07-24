#include "pic.h"
#include "../drivers/io/io.h"
#include "../builtins/stdint.h"


void init_pic()
{
    uint8_t master_mask = inb(0x21);
    uint8_t slave_mask = inb(0xA1);

    outb(0x20, 0x11);       // initialize into reconfiguration (ICW1)
    outb(0xA0, 0x11);       // same for slave

    io_wait();

    outb(0x21, 0x20);       // "your first IRQ gets vector 0x20" -> counts up by itself (IRQ0 -> 0x20 ---> IRQ7 -> IRQ27)   (ICW2)
    outb(0xA1, 0x28);       // same for slave, except this starts where the master ends

    io_wait();

    outb(0x21, 0x04);       // "slave is on your IRQ2"    (ICW3)
    outb(0xA1, 0x02);       // "you are on masters IRQ2"

    io_wait();

    outb(0x21, 0x01);       // "operate in 8086 mode" (ICW4)
    outb(0xA1, 0x01);

    io_wait();

    outb(0x21, master_mask);
    outb(0xA1, slave_mask);
}

bool enable_irq (uint8_t irq)
{
    if (irq <= 7)
    {
        uint8_t mask = inb(0x21);
        mask &= ~(1 << irq);
        outb(0x21, mask);
        return 0;
    } else if (irq <= 15 )
    {
        uint8_t mask = inb(0xA1);
        mask &= ~(1 << (irq - 8));
        outb(0xA1, mask);
        return 0;
    } else
        return 1;
}

bool disable_irq (uint8_t irq)
{
    if (irq <= 7)
    {
        uint8_t mask = inb(0x21);
        mask |= (1 << irq);
        outb(0x21, mask);
        return 0;
    } else if (irq <= 15 )
    {
        uint8_t mask = inb(0xA1);
        mask |= (1 << (irq - 8));
        outb(0xA1, mask);
        return 0;
    } else
        return 1;
}

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);
}
