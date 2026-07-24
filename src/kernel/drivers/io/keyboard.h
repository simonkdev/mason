#include "../../helpers.h"
#include "vgatxt.h"
#include "io.h"

extern int modifier_flags[3];
#define KEYBOARD_BUFFER_SIZE 128

void send_signal_to_ps2(uint8_t signal);
uint8_t read_signal_from_ps2();
int get_current_code_set();
int send_echo(void);
int initialize_keyboard(void);
void keyboard_interrupt_handler(void);
char keyboard_getchar(void);
