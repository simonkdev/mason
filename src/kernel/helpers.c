#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


char *strslice(char *dest, const char *src, int start, int stop) 
{
    size_t src_len = 0;
    while (src[src_len] != '\0') src_len++;

    size_t actual_start = (start < 0) ? (src_len + start) : (size_t)start;
    size_t actual_stop = (stop < 0) ? (src_len + stop) : (size_t)stop;

    if (actual_start > src_len) actual_start = src_len;
    if (actual_stop > src_len) actual_stop = src_len;
    if (actual_start > actual_stop) actual_start = actual_stop;

    size_t i;
    for (i = 0; actual_start + i < actual_stop; i++) {
        dest[i] = src[actual_start + i];
    }
    dest[i] = '\0';
    return dest;
}

char* convert_to_base(int value, char* result, int base) 
{
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

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void strcpy(char dest[], char src[]) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; // Null-terminate the destination string
}

void do_nothing()
{

}

int strcmp(const char* s1, const char* s2) 
{
    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}