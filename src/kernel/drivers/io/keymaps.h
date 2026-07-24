#include "../../builtins/stdint.h"
#include <stddef.h>

const char* get_key_val_from_code(uint8_t key_code, int modifier_flags[]);
uint8_t get_key_code_ascii_pressed(uint8_t key_code, int modifier_flags[]);
