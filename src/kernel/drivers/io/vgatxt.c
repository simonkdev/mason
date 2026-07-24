#include "../../builtins/stdint.h"
#include <stddef.h>
#include <stdbool.h>
#include "../../helpers.h"
#include "keyboard.h"
#include "io.h"

enum vgatxt_color {
    vgatxt_COLOR_BLACK = 0,
    vgatxt_COLOR_BLUE = 1,
    vgatxt_COLOR_GREEN = 2,
    vgatxt_COLOR_CYAN = 3,
    vgatxt_COLOR_RED = 4,
    vgatxt_COLOR_MAGENTA = 5,
    vgatxt_COLOR_BROWN = 6,
    vgatxt_COLOR_LIGHT_GREY = 7,
    vgatxt_COLOR_DARK_GREY = 8,
    vgatxt_COLOR_LIGHT_BLUE = 9,
    vgatxt_COLOR_LIGHT_GREEN = 10,
    vgatxt_COLOR_LIGHT_CYAN = 11,
    vgatxt_COLOR_LIGHT_RED = 12,
    vgatxt_COLOR_LIGHT_MAGENTA = 13,
    vgatxt_COLOR_LIGHT_BROWN = 14,
    vgatxt_COLOR_WHITE = 15
};


void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void set_cursor_pos(int x, int y)
{
	uint16_t pos = y * vgatxt_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

static inline uint8_t vgatxt_entry_color(enum vgatxt_color fg, enum vgatxt_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vgatxt_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

#define vgatxt_WIDTH 80
#define vgatxt_HEIGHT 25
#define vgatxt_MEMORY 0xB8000

size_t vgatxt_row;
size_t vgatxt_column;
uint8_t vgatxt_color;
uint16_t* vgatxt_buffer = (uint16_t*) vgatxt_MEMORY;

void vgatxt_initialize(void)
{
    vgatxt_row = 0;
    vgatxt_column = 0;
    vgatxt_color = vgatxt_entry_color(vgatxt_COLOR_MAGENTA, vgatxt_COLOR_BLACK);

    for (size_t y = 0; y<vgatxt_HEIGHT; y++) {
        for (size_t x = 0; x<vgatxt_WIDTH; x++) {
            const size_t index = y * vgatxt_WIDTH + x;
            vgatxt_buffer[index] = vgatxt_entry(' ', vgatxt_color);
        }
    }
}

void vgatxt_clear()
{
    for (size_t y = 0; y<vgatxt_HEIGHT; y++) {
        for (size_t x = 0; x<vgatxt_WIDTH; x++) {
            const size_t index = y * vgatxt_WIDTH + x;
            vgatxt_buffer[index] = vgatxt_entry(' ', vgatxt_color);
        }
    }
    vgatxt_column = 0;
    vgatxt_row = 0;
}

void vgatxt_setcolor(uint8_t color)
{
    vgatxt_color = color;
}

void vgatxt_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * vgatxt_WIDTH + x;
    vgatxt_buffer[index] = vgatxt_entry(c, color);
}

void vgatxt_putchar(char c)
{
    if (c == '\n') {
        vgatxt_column = 0;
        if (++vgatxt_row == vgatxt_HEIGHT)
            // vgatxt_initialize(); // Clear screen if we reach the bottom
            vgatxt_scroll_oneline();
    } else {
        vgatxt_putentryat(c, vgatxt_color, vgatxt_column, vgatxt_row);
        if (++vgatxt_column == vgatxt_WIDTH) {
            vgatxt_column = 0;
            if (++vgatxt_row == vgatxt_HEIGHT)
                vgatxt_row = 0;
        }
    }
}

void vgatxt_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        vgatxt_putchar(data[i]);
}

void vgatxt_writestring(const char* data)
{
    vgatxt_write(data, strlen(data));
    set_cursor_pos(vgatxt_column, vgatxt_row);
}


void vgatxt_write_u64(uint64_t value, int base, bool add_prefix, bool uppercase)
{
    static const char digits_low[]  = "0123456789abcdef";
    static const char digits_high[] = "0123456789ABCDEF";
    const char* digits = uppercase ? digits_high : digits_low;

    if (base != 2 && base != 10 && base != 16) {
        return;
    }

    char buf[80];
    int pos = 0;
    if (value == 0) {
        if (add_prefix && base == 16) {
            buf[pos++] = '0'; buf[pos++] = uppercase ? 'X' : 'x';
        } else if (add_prefix && base == 2) {
            buf[pos++] = '0'; buf[pos++] = 'b';
        }
        buf[pos++] = '0';
        buf[pos] = '\0';
        vgatxt_writestring(buf);
        vgatxt_writestring("\n");
        return;
    }

    char rev[80];
    int ri = 0;
    while (value != 0) {
        unsigned rem = (unsigned)(value % (uint64_t)base);
        rev[ri++] = digits[rem];
        value /= (uint64_t)base;
    }

    if (add_prefix) {
        if (base == 16) {
            buf[pos++] = '0'; buf[pos++] = uppercase ? 'X' : 'x';
        } else if (base == 2) {
            buf[pos++] = '0'; buf[pos++] = 'b';
        }
    }

    for (int i = ri - 1; i >= 0; --i)
        buf[pos++] = rev[i];

    buf[pos] = '\0';
    vgatxt_writestring(buf);
    vgatxt_writestring("\n");
}

void vgatxt_writeint(int value, char* result, int base)
{
    convert_to_base(value, result, base);
    vgatxt_writestring(result);
    vgatxt_writestring("\n");
}

void put_debug_msg()
{
    vgatxt_writestring("Debug: This is a debug message.\n");
}

void vgatxt_writehex(uint8_t value) {
    char hex_digits[] = "0123456789abcdef";
    char buffer[3]; // 2 hex digits + null terminator
    buffer[0] = hex_digits[(value >> 4) & 0x0F]; // High nibble
    buffer[1] = hex_digits[value & 0x0F];        // Low nibble
    buffer[2] = '\0';
    vgatxt_writestring(buffer);
}

void vgatxt_writehex_u16(uint16_t value) {
    vgatxt_writehex((uint8_t)(value >> 8));  // High byte
    vgatxt_writehex((uint8_t)(value & 0xFF)); // Low byte
}

void vgatxt_writehex_u32(uint32_t value) {
    vgatxt_writehex_u16((uint16_t)(value >> 16)); // High word
    vgatxt_writehex_u16((uint16_t)(value & 0xFFFF)); // Low word
}

void vgatxt_backspace()
{
    vgatxt_putentryat(' ', vgatxt_color, vgatxt_column - 1, vgatxt_row);
    vgatxt_column -= 1;
    set_cursor_pos(vgatxt_column, vgatxt_row);
}

void vgatxt_safe_backspace(size_t start_row, size_t start_col)
{
    if (vgatxt_row < start_row) {
        set_cursor_pos(vgatxt_column, vgatxt_row);
        return;
    }
    if (vgatxt_row == start_row && vgatxt_column <= start_col) {
        set_cursor_pos(vgatxt_column, vgatxt_row);
        return;
    }

    if (vgatxt_column > 0) {
        vgatxt_column -= 1;
        vgatxt_putentryat(' ', vgatxt_color, vgatxt_column, vgatxt_row);
        set_cursor_pos(vgatxt_column, vgatxt_row);
        return;
    }

    if (vgatxt_row == 0) {
        set_cursor_pos(vgatxt_column, vgatxt_row);
        return;
    }

    size_t new_row = vgatxt_row - 1;
    if (new_row < start_row) {
        set_cursor_pos(vgatxt_column, vgatxt_row);
        return;
    }

    vgatxt_row = new_row;
    vgatxt_column = vgatxt_WIDTH - 1;
    vgatxt_putentryat(' ', vgatxt_color, vgatxt_column, vgatxt_row);
    set_cursor_pos(vgatxt_column, vgatxt_row);
}

char* vgatxt_uinput()
{
    size_t inital_column = vgatxt_column;
    char input_buffer[128];
    uint64_t index = 0;
    uint8_t last_key_code;
    while (true) {
        if (vgatxt_column < inital_column)
        {
            break;
        }
        uint8_t code = read_signal_from_ps2();

        if (code == last_key_code) {
            continue; // Skip processing if the same key code is received again
        }

        if(code == 0xFA) {
            continue; // Ignore ACKs in the main loop
        }

        update_modifier_flags(code);


        char* ascii = get_key_val_from_code(code, modifier_flags);
        if (strcmp(ascii, "\n") == 0) {
            input_buffer[index] = '\0'; // Null-terminate the string
            break;
        } else if (strcmp(ascii, "BS") == 0) {
            if (index > 0) {
                index--;
                vgatxt_backspace();
            }
        } else if (strcmp(ascii, "NUL") != 0) {
            input_buffer[index] = "a";//ascii[0]; // Assuming ascii is a single character string
            index++;
            vgatxt_putchar(ascii);
        }
        last_key_code = code;
    }
    return input_buffer; // Note: This returns a pointer to a local variable, which is unsafe. Consider using a static buffer or dynamic allocation.
}

char* vgatxt_tty()
{
    vgatxt_writestring("Welcome to the vgatxt Terminal!\n");
    vgatxt_writestring("Type something and press Enter:\n");
    char* user_input = vgatxt_uinput();
    vgatxt_writestring("You entered: ");
    vgatxt_writestring(user_input);
    vgatxt_writestring("\n");
    return user_input;
}

void vgatxt_scroll_oneline()
{
    for (size_t y = 1; y < vgatxt_HEIGHT; y++) {
        for (size_t x = 0; x < vgatxt_WIDTH; x++) {
            const size_t from_index = y * vgatxt_WIDTH + x;
            const size_t to_index = (y - 1) * vgatxt_WIDTH + x;
            vgatxt_buffer[to_index] = vgatxt_buffer[from_index];
        }
    }
    // Clear the last line
    for (size_t x = 0; x < vgatxt_WIDTH; x++) {
        const size_t index = (vgatxt_HEIGHT - 1) * vgatxt_WIDTH + x;
        vgatxt_buffer[index] = vgatxt_entry(' ', vgatxt_color);
    }
    if (vgatxt_row > 0) {
        vgatxt_row--;
    }
    set_cursor_pos(vgatxt_column, vgatxt_row);
}
