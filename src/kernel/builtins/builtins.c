#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../helpers.h"
#include "../drivers/io/vga.h"

uint8_t execute_builtins_from_cmd(char* cmd)
{
    if (strcmp(cmd, "clear") == 0)
    {
        vga_clear();
        return 0; // Indicate that a builtin command was executed
    }
    else 
    {
        return 1; // Indicate that no builtin command was executed
    }
}