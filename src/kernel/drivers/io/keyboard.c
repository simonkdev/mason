#include "../../helpers.h"
#include "vgatxt.h"
#include "io.h"
#include <stdbool.h>
#include "keymaps.h"
#include <stddef.h>
#include "keyboard.h"

#define SIGNALS_TO_TERMINAL 0

const char *keyboard_buffer[KEYBOARD_BUFFER_SIZE];

size_t head = 0;
size_t tail = 0;

int modifier_flags[3] = {0, 0, 0}; //index 0 is shift, index 1 is shift lock, index 2 is alt gr

void send_signal_to_ps2(uint8_t signal)
{
    outb(0x60, signal);
    if (SIGNALS_TO_TERMINAL == 1) {
        vgatxt_writestring("Sent: 0x");
        vgatxt_writehex(signal);
        vgatxt_writestring("\n");
    }
}

uint8_t read_signal_from_ps2()
{
    uint8_t received = inb(0x60);
    if (SIGNALS_TO_TERMINAL == 1) {
        vgatxt_writestring("Received: 0x");
        vgatxt_writehex(received);
        vgatxt_writestring("\n");
    }
    return received;
}

int get_current_code_set()
{
    send_signal_to_ps2(0xF0); // Request current code set

    uint8_t ack = read_signal_from_ps2();
    if (ack != 0xFA) {
        vgatxt_writestring("Keyboard did not acknowledge code set request.\n");
        return 0xFF; // Indicate failure
    }

    uint8_t reply = read_signal_from_ps2();
    while (reply == 0xFA) { // Handle potential ACKs before the actual code set response
        vgatxt_writestring("Received ACK, waiting for code set response...\n");
        reply = read_signal_from_ps2();
    }
    switch (reply) {
        case 0x43:
            vgatxt_writestring("Keyboard code set: 1\n");
            return 0x43; // Code Set 1
        case 0x41:
            vgatxt_writestring("Keyboard code set: 2\n");
            return 0x41; // Code Set 2
        case 0x3F:
            vgatxt_writestring("Keyboard code set: 3\n");
            return 0x3F; // Code Set 3
        case 0xFA:
            vgatxt_writestring("PS/2 NACK (not acknowledged).\n");
            return 0xFA; // Return the error code
        default:
            vgatxt_writestring("Unknown keyboard code set response: 0x");
            uint8_t reply_hex;
            vgatxt_writeint(reply, (char*)&reply_hex, 16);
            return 0xFF; // Indicate failure
    }

    vgatxt_writestring("Failed to determine keyboard code set.\n");
    return 0xFF; // Indicate failure
}

int send_echo()
{
    send_signal_to_ps2(0xEE); // Send ECHO command
    uint8_t response = read_signal_from_ps2();
    if (response == 0xEE) {
        vgatxt_writestring("ECHO successful: Keyboard responded correctly.\n");
        return 1; // Success
    } else {
        vgatxt_writestring("ECHO failed: Unexpected response from keyboard.\n");
        return 0; // Failure
    }
}

int initialize_keyboard() {
    // Reset keyboard
    send_signal_to_ps2(0xFF);
    uint8_t ack = read_signal_from_ps2();
    if (ack != 0xFA) {
        vgatxt_writestring("Keyboard did not acknowledge reset.\n");
        return 0;
    }

    // Wait for self-test result
    uint8_t self_test_result = read_signal_from_ps2();
    if (self_test_result == 0xFC) {
        vgatxt_writestring("Keyboard self-test failed.\n");
        return 0;
    } else if (self_test_result == 0xFD) {
        vgatxt_writestring("Keyboard self-test failed \n");
        return 0;
    }
    else if (self_test_result != 0xAA) {
        vgatxt_writestring("Unexpected self-test result.\n");
        return 0;
    }

    // Enable scanning
    send_signal_to_ps2(0xF4);
    ack = read_signal_from_ps2();
    if (ack != 0xFA) {
        vgatxt_writestring("Keyboard did not acknowledge enable scanning.\n");
        return 0;
    }

    vgatxt_writestring("Keyboard initialized successfully.\n");
    return 1;
}

void update_modifier_flags(code)
{
}

void keyboard_interrupt_handler(void)
{
    uint8_t code = read_signal_from_ps2();

    if (code == 0xFA)
        return;

    update_modifier_flags(code);

    char* ascii = get_key_val_from_code(code, modifier_flags);

    keyboard_buffer[head] = ascii;
    head = (head + 1) % KEYBOARD_BUFFER_SIZE;
}

bool keyboard_available(void)
{
    return head != tail;
}

char *keyboard_getkey(void)
{
    if (head == tail)
        return "NUL";

    char *ret = (char *)keyboard_buffer[tail];
    tail = (tail + 1) % KEYBOARD_BUFFER_SIZE;
    return ret;
}
