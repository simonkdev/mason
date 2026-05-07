#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../../helpers.h"


#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

extern size_t vga_row;
extern size_t vga_column;
extern uint8_t vga_color;
extern uint16_t* vga_buffer;

void vga_initialize(void);
void vga_setcolor(uint8_t color);
void vga_putentryat(char c, uint8_t color, size_t x, size_t y);
void vga_putchar(char c);
void vga_write(const char* data, size_t size);
void vga_writestring(const char* data);
void vga_writeint(int value, char* result, int base);
void vga_write_u64(uint64_t value, int base, bool add_prefix, bool uppercase);
void vga_writehex(uint8_t value);
void vga_writehex_u16(uint16_t value);
void vga_safe_backspace(size_t start_row, size_t start_col);
