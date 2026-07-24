#include "../../helpers.h"
#include "vgatxt.h"
#include "io.h"

extern int modifier_flags[3];

void send_signal_to_ps2(uint8_t signal);
uint8_t read_signal_from_ps2();
int get_current_code_set();
int send_echo(void);
int initialize_keyboard(void);
char* tty_input(void);
// uint8_t convert_code_set_hex_to_int(uint8_t reply);
void keyboard_interrupt_handler(void);
char* tty_input_interrupts(void);
