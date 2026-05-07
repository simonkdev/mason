#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../helpers.h"
#include "../drivers/io/vga.h"

uint8_t execute_builtins_from_cmd(char* cmd)
{
    char *buffer = "";
    if (strcmp(cmd, "clear") == 0)
    {
        vga_clear();
        return 0; // Indicate that a builtin command was executed
    }
    else if (strcmp(cmd, "debug") == 0)
    {
        put_debug_msg();
        return 0; // Indicate that a builtin command was executed
    }
    else if (strcmp(cmd, "hello there") == 0)
    {
        vga_writestring("General Kenobi! \n");
        return 0; // Indicate that a builtin command was executed
    }
    else if (strcmp(cmd, "loop") == 0)
    {
        while(true) 
        {
            vga_writestring(" [ AS ] ");
        }
        return 0; // Indicate that a builtin command was executed
    }
    else 
    {
        vga_writestring("Error: Command ");
        vga_writestring(cmd);
        vga_writestring(" not found. \n"); // Builtin command executed, skip normal processing
        return 1; // Indicate that no builtin command was executed
    }
}