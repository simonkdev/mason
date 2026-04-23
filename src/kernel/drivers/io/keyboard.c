#include "../module-headers/helpers.h"
#include "../module-headers/vga.h"
#include "../module-headers/io.h"

void send_signal_to_ps2(uint8_t signal) {
    outb(0x60, signal);
}

uint8_t read_signal_from_ps2() {
    return intb(0x60);
}

