#include "stdint.h"
#include <stddef.h>
#include <stdbool.h>
#include "../helpers.h"
#include "../drivers/io/vgatxt.h"
#include "../drivers/io/keyboard.h"

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
        asm volatile("int $0x08");
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

char line[128];
size_t line_len = 0;

void builtin_terminal(void)
{
    vgatxt_writestring(" > ");
    while(1)
    {
        char *key = keyboard_getkey();

        if(strcmp(key, "NUL") == 0)
        {
            continue;
        }

        if(strcmp(key, "BS") == 0)
        {
            if (line_len > 0)
            {
                line_len--;
                line[line_len] = '\0';
                vgatxt_backspace();
            }
            continue;
        }

        if (strcmp(key, "\n") == 0)
        {
            vgatxt_putchar('\n');
            line[line_len] = '\0';
            execute_builtins_from_cmd(line);
            line_len = 0;
            vgatxt_writestring(" > ");
            continue;
        }

        line[line_len++] = key[0];
        vgatxt_putchar(key[0]);
    }
}
