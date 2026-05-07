#include "../../helpers.h"
#include "vga.h"
#include "io.h"
#include <stdbool.h>
#include "keymaps.h"
#include <stddef.h>

#define SIGNALS_TO_TERMINAL 0

int modifier_flags[3] = {0, 0, 0}; //index 0 is shift, index 1 is shift lock, index 2 is alt gr

void send_signal_to_ps2(uint8_t signal) 
{
    outb(0x60, signal);
    if (SIGNALS_TO_TERMINAL == 1) {
        vga_writestring("Sent: 0x");
        vga_writehex(signal);
        vga_writestring("\n");
    }
}

uint8_t read_signal_from_ps2() 
{
    uint8_t received = inb(0x60);
    if (SIGNALS_TO_TERMINAL == 1) {
        vga_writestring("Received: 0x");
        vga_writehex(received);
        vga_writestring("\n");
    }
    return received;

}

int get_current_code_set()
{
    send_signal_to_ps2(0xF0); // Request current code set

    uint8_t ack = read_signal_from_ps2();
    if (ack != 0xFA) {
        vga_writestring("Keyboard did not acknowledge code set request.\n");
        return 0xFF; // Indicate failure
    }

    uint8_t reply = read_signal_from_ps2();
    while (reply == 0xFA) { // Handle potential ACKs before the actual code set response
        vga_writestring("Received ACK, waiting for code set response...\n");
        reply = read_signal_from_ps2();
    }
    switch (reply) {
        case 0x43:
            vga_writestring("Keyboard code set: 1\n");
            return 0x43; // Code Set 1
        case 0x41:
            vga_writestring("Keyboard code set: 2\n");
            return 0x41; // Code Set 2
        case 0x3F:
            vga_writestring("Keyboard code set: 3\n");
            return 0x3F; // Code Set 3
        case 0xFA:
            vga_writestring("PS/2 NACK (not acknowledged).\n");
            return 0xFA; // Return the error code
        default:
            vga_writestring("Unknown keyboard code set response: 0x");
            uint8_t reply_hex;
            vga_writeint(reply, (char*)&reply_hex, 16);
            return 0xFF; // Indicate failure
    }

    vga_writestring("Failed to determine keyboard code set.\n");
    return 0xFF; // Indicate failure
}
// uint8_t convert_code_set_hex_to_int(uint8_t reply)
// {
// switch (reply) {
//     case 0x43:
//         return 1; // Code Set 1
//     case 0x41:
//         return 2; // Code Set 2
//     case 0x3F:
//         return 3; // Code Set 3
//     default:
//         return 8; // Unknown code set
// };
// }

int send_echo()
{
    send_signal_to_ps2(0xEE); // Send ECHO command
    uint8_t response = read_signal_from_ps2();
    if (response == 0xEE) {
        vga_writestring("ECHO successful: Keyboard responded correctly.\n");
        return 1; // Success
    } else {
        vga_writestring("ECHO failed: Unexpected response from keyboard.\n");
        return 0; // Failure
    }
}

int initialize_keyboard() {
    // Reset keyboard
    send_signal_to_ps2(0xFF);
    uint8_t ack = read_signal_from_ps2();
    if (ack != 0xFA) {
        vga_writestring("Keyboard did not acknowledge reset.\n");
        return 0;
    }

    // Wait for self-test result
    uint8_t self_test_result = read_signal_from_ps2();
    if (self_test_result == 0xFC) {
        vga_writestring("Keyboard self-test failed.\n");
        return 0;
    } else if (self_test_result == 0xFD) {
        vga_writestring("Keyboard self-test failed \n");
        return 0;
    }
    else if (self_test_result != 0xAA) {
        vga_writestring("Unexpected self-test result.\n");
        return 0;
    }

    // Enable scanning
    send_signal_to_ps2(0xF4);
    ack = read_signal_from_ps2();
    if (ack != 0xFA) {
        vga_writestring("Keyboard did not acknowledge enable scanning.\n");
        return 0;
    }

    vga_writestring("Keyboard initialized successfully.\n");
    return 1;
}

void update_modifier_flags(code)
{
    switch (code) 
    {
        case 0x2A: // Shift press
            modifier_flags[0] = 1;
            break;
        case 0xAA: // Shift release
            modifier_flags[0] = 0;
            break;
        case 0x3A: // Shift lock press
            if (modifier_flags[1] == 1)
            {
                modifier_flags[1] = 0;
                // vga_writestring("Set shift lock pos to 0");
                break;
            } else if (modifier_flags[1] == 0)
            {
                modifier_flags[1] = 1;
                // vga_writestring("Set shift lock pos to 1");
                break;
            }
            break;
        case 0x38:
            modifier_flags[2] = 1;
            break;
        case 0xB8:
            modifier_flags[2] = 0;
            break;
        default:
            do_nothing();
            break;
    }
}

char* main_loop(void) {
    uint8_t code = read_signal_from_ps2();
    uint8_t last_key_code = code;

    vga_writestring("\n");
    vga_writestring("> ");
    size_t inital_row = vga_row;

    size_t inital_column = vga_column;
    static char input_buffer[128];
    uint64_t index = 0;
    while(true) {

        if (vga_row < inital_row) 
        {
            continue;
        }
        
        code = read_signal_from_ps2();

        //vga_writehex(code);

        
        if (code == last_key_code) {
            continue; // Skip processing if the same key code is received again
        } else if(code == 0xFA) {
            continue; // Ignore ACKs in the main loop
        }
        
        update_modifier_flags(code);

        
        
        char* ascii = get_key_val_from_code(code, modifier_flags);

        if (strcmp(ascii, "NUL") == 0)
        {
            do_nothing();
        } else if (strcmp(ascii, "BS") == 0)
        {
            if (index > 0) {
                index--;
            }
            vga_safe_backspace(inital_row, inital_column);
        } else if (strcmp(ascii, "\n") == 0)
        {
            input_buffer[index] = '\0';
            return input_buffer; 
        }
        else
        {
            input_buffer[index] = ascii[0];
            index++;
            vga_writestring(ascii);
        }

        last_key_code = code;
    }
}
