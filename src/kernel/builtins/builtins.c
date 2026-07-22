#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../helpers.h"
#include "../drivers/io/vgatxt.h"

uint8_t execute_builtins_from_cmd(char* cmd)
{
    char *buffer = "";
    if (strcmp(cmd, "clear") == 0)
    {
        vgatxt_clear();
        return 0; // Indicate that a builtin command was executed
    }
    else if (strcmp(cmd, "debug") == 0)
    {
        put_debug_msg();
        return 0; // Indicate that a builtin command was executed
    }
    else if (strcmp(cmd, "hello there") == 0)
    {
        vgatxt_writestring("General Kenobi! \n");
        return 0; // Indicate that a builtin command was executed
    }
    else if (strcmp(cmd, "loop") == 0)
    {
        while(true)
        {
            vgatxt_writestring(" [ AS ] ");
        }
        return 0; // Indicate that a builtin command was executed
    }
    else if (strcmp(cmd, "interrupt") == 0)
    {
        asm volatile("int $0x30");
        return 0; // Indicate that a builtin command was executed
    }
    else
    {
        vgatxt_writestring("Error: Command ");
        vgatxt_writestring(cmd);
        vgatxt_writestring(" not found. \n"); // Builtin command executed, skip normal processing
        return 1; // Indicate that no builtin command was executed
    }
}
