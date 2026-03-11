#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "module-headers/helpers.h"
#include "module-headers/terminal.h"

void kernel_main(void)
{
    terminal_writestring("GDT initialized\n");
    terminal_writestring("Hello, kernel World!\n");
    terminal_writestring("In a galaxy far, far away...\n");

    uint64_t dummy = 123; 
    terminal_write_u64(dummy, 10, false, false);
}
