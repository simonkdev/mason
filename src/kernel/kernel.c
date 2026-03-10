#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "gdt.h"


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

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*) VGA_MEMORY;

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);

    for (size_t y = 0; y<VGA_HEIGHT; y++) {
        for (size_t x = 0; x<VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0;
        }
    }
}

void terminal_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}


void terminal_write_u64(uint64_t value, int base, bool add_prefix, bool uppercase)
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
        terminal_writestring(buf);
        terminal_writestring("\n");
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
    terminal_writestring(buf);
    terminal_writestring("\n");
}

char* itoa(int value, char* result, int base) {
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
  
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void terminal_writeint(int value, char* result, int base)
{
    itoa(value, result, base);
    terminal_writestring(result);
    terminal_writestring("\n");
}

void terminal_debugmsg_as()
{
    terminal_writestring("Debug: This is a debug message.\n");
}


// void kernel_gdt(void)
// {
//     terminal_initialize();
//     terminal_writestring("Hello, kernel World!\n");
//     terminal_writestring("In a galaxy far, far away...\n");

//     gdt_init();

//     kernel_main();
// }

void kernel_main(void)
{
    terminal_initialize();
    terminal_writestring("Hello, kernel World!\n");
    terminal_writestring("In a galaxy far, far away...\n");

    terminal_writestring("GDT initialized\n");

    uint64_t dummy = 123; 
    terminal_write_u64(dummy, 10, false, false);
}
