#ifndef VGATXT_H
#define VGATXT_H


#include "../../builtins/stdint.h"
#include <stddef.h>
#include <stdbool.h>
#include "../../helpers.h"

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

#define vgatxt_WIDTH 80
#define vgatxt_HEIGHT 25
#define vgatxt_MEMORY 0xB8000

extern size_t vgatxt_row;
extern size_t vgatxt_column;
extern uint8_t vgatxt_color;
extern uint16_t* vgatxt_buffer;

void vgatxt_initialize(void);
void vgatxt_setcolor(uint8_t color);
void vgatxt_putentryat(char c, uint8_t color, size_t x, size_t y);
void vgatxt_putchar(char c);
void vgatxt_write(const char* data, size_t size);
void vgatxt_writestring(const char* data);
void vgatxt_writeint(int value, char* result, int base);
void vgatxt_write_u64(uint64_t value, int base, bool add_prefix, bool uppercase);
void vgatxt_writehex(uint8_t value);
void vgatxt_writehex_u16(uint16_t value);
void vgatxt_safe_backspace(size_t start_row, size_t start_col);
void disable_cursor(void);
void vgatxt_set_color(enum vgatxt_color fg, enum vgatxt_color bg);
void vgatxt_set_entry(size_t row, size_t col);



#endif
