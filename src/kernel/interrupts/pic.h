#include "../drivers/io/io.h"
#include "../builtins/stdint.h"


void init_pic(void);
bool enable_irq (uint8_t irq);
bool disable_irq (uint8_t irq);
void pic_send_eoi(uint8_t irq);
