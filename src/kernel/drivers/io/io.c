#include "../module-headers/io.h"

void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t intb(uint16_t port)
{
    uint8_t ret;
    __asm__ volative("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}