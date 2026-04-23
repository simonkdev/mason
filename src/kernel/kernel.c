#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "module-headers/helpers.h"
#include "module-headers/vga.h"

void kernel_main(void)
{
    vga_writestring("GDT initialized\n");
    vga_writestring("Hello, kernel World!\n");
    vga_writestring("In a galaxy far, far away...\n");

    uint64_t dummy = 123; 
    vga_write_u64(dummy, 10, false, false);
}
