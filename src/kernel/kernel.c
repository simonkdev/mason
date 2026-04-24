#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "helpers.h"
#include "drivers/io/vga.h"
#include "drivers/io/keyboard.h"

void kernel_main(void)
{
    vga_writestring("GDT initialized\n");
    vga_writestring("Hello, kernel World!\n");
    vga_writestring("In a galaxy far, far away...\n");

    send_echo();

    initialize_keyboard();
    main_loop();
}
