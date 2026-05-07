#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../../helpers.h"
#include "keyboard.h"
#include "io.h"

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15
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
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t vga_row;
size_t vga_column;
uint8_t vga_color;
uint16_t* vga_buffer = (uint16_t*) VGA_MEMORY;

void vga_initialize(void)
{
    vga_row = 0;
    vga_column = 0;
    vga_color = vga_entry_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK);

    for (size_t y = 0; y<VGA_HEIGHT; y++) {
        for (size_t x = 0; x<VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', vga_color);
        }
    }
}

void vga_clear()
{
    for (size_t y = 0; y<VGA_HEIGHT; y++) {
        for (size_t x = 0; x<VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', vga_color);
        }
    }
    vga_column = 0;
    vga_row = 0;
}

void vga_setcolor(uint8_t color)
{
    vga_color = color;
}

void vga_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    vga_buffer[index] = vga_entry(c, color);
}

void vga_putchar(char c)
{
    if (c == '\n') {
        vga_column = 0;
        if (++vga_row == VGA_HEIGHT)
            // vga_initialize(); // Clear screen if we reach the bottom
            vga_scroll_oneline();
    } else {
        vga_putentryat(c, vga_color, vga_column, vga_row);
        if (++vga_column == VGA_WIDTH) {
            vga_column = 0;
            if (++vga_row == VGA_HEIGHT)
                vga_row = 0;
        }
    }
}

void vga_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        vga_putchar(data[i]);
}

void vga_writestring(const char* data)
{
    vga_write(data, strlen(data));
    set_cursor_pos(vga_column, vga_row);
}


void vga_write_u64(uint64_t value, int base, bool add_prefix, bool uppercase)
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
        vga_writestring(buf);
        vga_writestring("\n");
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
    vga_writestring(buf);
    vga_writestring("\n");
}

void vga_writeint(int value, char* result, int base)
{
    convert_to_base(value, result, base);
    vga_writestring(result);
    vga_writestring("\n");
}

void put_debug_msg()
{
    vga_writestring("Debug: This is a debug message.\n");
}

void vga_writehex(uint8_t value) {
    char hex_digits[] = "0123456789abcdef";
    char buffer[3]; // 2 hex digits + null terminator
    buffer[0] = hex_digits[(value >> 4) & 0x0F]; // High nibble
    buffer[1] = hex_digits[value & 0x0F];        // Low nibble
    buffer[2] = '\0';
    vga_writestring(buffer);
}

void vga_writehex_u16(uint16_t value) {
    vga_writehex((uint8_t)(value >> 8));  // High byte
    vga_writehex((uint8_t)(value & 0xFF)); // Low byte
}

void vga_writehex_u32(uint32_t value) {
    vga_writehex_u16((uint16_t)(value >> 16)); // High word
    vga_writehex_u16((uint16_t)(value & 0xFFFF)); // Low word
}

void vga_backspace()
{
    vga_putentryat(' ', vga_color, vga_column - 1, vga_row);
    vga_column -= 1;
    set_cursor_pos(vga_column, vga_row);
}

void vga_safe_backspace(size_t start_row, size_t start_col)
{
    if (vga_row < start_row) {
        set_cursor_pos(vga_column, vga_row);
        return;
    }
    if (vga_row == start_row && vga_column <= start_col) {
        set_cursor_pos(vga_column, vga_row);
        return;
    }

    if (vga_column > 0) {
        vga_column -= 1;
        vga_putentryat(' ', vga_color, vga_column, vga_row);
        set_cursor_pos(vga_column, vga_row);
        return;
    }

    if (vga_row == 0) {
        set_cursor_pos(vga_column, vga_row);
        return;
    }

    size_t new_row = vga_row - 1;
    if (new_row < start_row) {
        set_cursor_pos(vga_column, vga_row);
        return;
    }

    vga_row = new_row;
    vga_column = VGA_WIDTH - 1;
    vga_putentryat(' ', vga_color, vga_column, vga_row);
    set_cursor_pos(vga_column, vga_row);
}

char* vga_uinput()
{
    size_t inital_column = vga_column;
    char input_buffer[128];
    uint64_t index = 0;
    uint8_t last_key_code; 
    while (true) {
        if (vga_column < inital_column) 
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
                vga_backspace();
            }
        } else if (strcmp(ascii, "NUL") != 0) {
            input_buffer[index] = "a";//ascii[0]; // Assuming ascii is a single character string
            index++;
            vga_putchar(ascii);
        }
        last_key_code = code;
    }
    return input_buffer; // Note: This returns a pointer to a local variable, which is unsafe. Consider using a static buffer or dynamic allocation.
}

char* vga_tty()
{
    vga_writestring("Welcome to the VGA Terminal!\n");
    vga_writestring("Type something and press Enter:\n");
    char* user_input = vga_uinput();
    vga_writestring("You entered: ");
    vga_writestring(user_input);
    vga_writestring("\n");
    return user_input;
}

void vga_scroll_oneline()
{
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t from_index = y * VGA_WIDTH + x;
            const size_t to_index = (y - 1) * VGA_WIDTH + x;
            vga_buffer[to_index] = vga_buffer[from_index];
        }
    }
    // Clear the last line
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        vga_buffer[index] = vga_entry(' ', vga_color);
    }
    if (vga_row > 0) {
        vga_row--;
    }
    set_cursor_pos(vga_column, vga_row);
}
