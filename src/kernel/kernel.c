#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "helpers.h"
#include "drivers/io/vgatxt.h"
#include "drivers/io/keyboard.h"
#include "builtins/builtins.h"

void kernel_main(void)
{
    vgatxt_writestring("GDT initialized\n");
    vgatxt_writestring("Hello, kernel World!\n");
    vgatxt_writestring("In a galaxy far, far away...\n");

    send_echo();

    initialize_keyboard();

    while(true) {
        char* uinput = (char*)tty_input();
        uint8_t builtins_response = execute_builtins_from_cmd(uinput);

        // vgatxt_writestring("\n");
        // vgatxt_writestring("You entered: ");
        // vgatxt_writestring(uinput);
    }
}
