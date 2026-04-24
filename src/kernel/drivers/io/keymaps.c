#include "keymaps.h"
#include "../../helpers.h"

uint8_t key_code_pressed[6][17] = {
    {0x01, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x57, 0x58, 0x0, 0x0, 0x0, 0x0},
    {0x29, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0, 0x0, 0x0},
    {0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x0, 0x0, 0x0},
    {0x3A, 0x1e, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x1C, 0x0, 0x0, 0x0},
    {0x2A, 0x56, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x0, 0x0, 0x0, 0x0},
    {0x1D, 0xE05B, 0x38, 0x39, 0xE038, 0xE05C, 0xE01D, 0xE048, 0xE049, 0xE04B, 0xE051, 0xE050, 0xE04D, 0x0, 0x0, 0x0}
};

char key_vals[6][17] = {
    {'esc', 'f1', 'f2', 'f3', 'f4', 'f5', 'f6', 'f7', 'f8', 'f9', 'f10', 'f11', 'f12', 'null', 'null', 'null', 'null'},
    {'^', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'ß', '´', '\b', 'null', 'null', 'null'},
    {'  ', 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', 'ü', '+', '\n', 'null', 'null', 'null'},
    {'caps', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'ö', 'ä', '#', '\n', 'null', 'null', 'null'},
    {'lshift', '<', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 'rshift', 'null', 'null', 'null', 'null'},
    {'ctrl', 'mod', 'lalt', ' ', 'altgr', 'prt', 'strg', 'pgup', 'up', 'pgdown', 'left', 'down', 'right', 0x0, 0x0, 0x0, 0x0}
};

uint8_t get_key_code_ascii_pressed_index(uint8_t key_code)
{
    for (int i = 0; i < sizeof(key_code_pressed); i++)
    {
        for (int j = 0; j < sizeof(key_code_pressed[i]); j++)
        {
            if (key_code_pressed[i][j] == key_code)
            {
                return i, j; // Return the index of the ASCII character
            }
        }
    }
    return 0, 0; // Return 0,0 if the key code is not found
}

char get_key_val_from_code(uint8_t key_code)
{
    int index_r, index_c = get_key_code_ascii_pressed_index(key_code);
    if (index_r == 0 && index_c == 0) {
        return "null"; // Return "null" if the key code is not found
    }
    char ret; //[5]; 
    ret = key_vals[index_r][index_c];//strcpy(ret, key_vals[index_r][index_c]);
    return ret;
}