
#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdint.h>

typedef struct terminal terminal;

void terminal_initialize();
void terminal_putchar(uint8_t c);
void terminal_writestring(char *s);

#endif