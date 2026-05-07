#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "helpers.h"
#include "drivers/io/vga.h"
#include "drivers/io/keyboard.h"
#include "builtins/builtins.h"

void kernel_main(void)
{
    vga_writestring("GDT initialized\n");
    vga_writestring("Hello, kernel World!\n");
    vga_writestring("In a galaxy far, far away...\n");

    send_echo();

    initialize_keyboard();

    while(true) {
        char* uinput = (char*)main_loop();
        uint8_t builtins_response = execute_builtins_from_cmd(uinput);
        if (builtins_response != 0) {
            vga_writestring("\n No command found. \n");
            continue; // Builtin command executed, skip normal processing
        }

        // vga_writestring("\n");
        // vga_writestring("You entered: ");
        // vga_writestring(uinput);
    }
}
