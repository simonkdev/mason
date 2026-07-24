#include "builtins/stdint.h"
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
    builtin_terminal();

    while(true) {
        //char* uinput = (char*)tty_input_interrupts();
        //uint8_t builtins_response = execute_builtins_from_cmd(uinput);
    }
}
