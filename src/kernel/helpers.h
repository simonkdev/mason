#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

char* convert_to_base(int value, char* result, int base);
size_t strlen(const char* str);
void strcpy(char *dest, const char *src);
char *strslice(char *dest, const char *src, int start, int stop);