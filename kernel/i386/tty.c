
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>

#include "vga.h"


size_t row = 0;
size_t col = 0;
uint8_t color = VGA_COLOR(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);;
vga_entry *const buffer = (vga_entry *)0xB8000;


void terminal_initialize() {
    int i;

    vga_entry e = {' ', color};

    for(i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        buffer[i] = e;
    }
}

void terminal_putchar(uint8_t c) {
    int _row,_col;

    if(c == '\n') {
        col = 0;
        row++;
    } else {
        buffer[row * VGA_WIDTH + col] = (vga_entry){c, color};
        col++;
    }

            
    if(col == VGA_WIDTH) {
        col = 0;
        row++;
    }

    if(row == VGA_HEIGHT)
    {
        for(_row=1; _row < VGA_HEIGHT;_row++) {
            for(_col = 0; _col < VGA_WIDTH; _col++) {
                buffer[(_row-1) * VGA_WIDTH + _col] = buffer[_row * VGA_WIDTH + _col];
            }
        }

        for(_col = 0; _col < VGA_WIDTH; _col++) {
            buffer[(_row-1) * VGA_WIDTH + _col] = (vga_entry){' ', color};
        }

        row--;
    }
}

void terminal_writestring(char *s)
{
    while(*s)
        terminal_putchar(*s++);
}

