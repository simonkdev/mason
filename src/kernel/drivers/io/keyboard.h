#include "../../helpers.h"
#include "vga.h"
#include "io.h"


void send_signal_to_ps2(uint8_t signal);
uint8_t read_signal_from_ps2();
int get_current_code_set();
// uint8_t convert_code_set_hex_to_int(uint8_t reply);
