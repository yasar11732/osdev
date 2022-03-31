
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h> // memmove
#include <kernel/tty.h>

#include "vga.h"


size_t row = 0;
size_t col = 0;
uint8_t vga_color = VGA_COLOR(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);;
static vga_entry *const buffer = (vga_entry *)0xC00B8000;


void terminal_initialize() {
    int i;

    vga_entry e = {' ', vga_color};

    for(i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        buffer[i] = e;
    }

    row = 0;
    col = 0;
}

void terminal_putchar(uint8_t c) {


    if(c == '\n') {
        col = 0;
        row++;
    } else {
        buffer[row * VGA_WIDTH + col] = (vga_entry){c, vga_color};
        col++;
    }

            
    if(col == VGA_WIDTH) {
        col = 0;
        row++;
    }

    if(row == VGA_HEIGHT)
    {
        /*
        * VGA memory consists of 80x25 16 bit numbers 2 bytes
        * so, visible memory consists of = 80x25x2=4000 bytes
        * first line consists of 80x2 = 160 bytes
        * remaining lines consists of 3840 bytes
        */
        memmove(buffer, &buffer[80], (VGA_HEIGHT-1)*VGA_WIDTH*sizeof(vga_entry));

        
        int i;
        uint32_t x = (uint32_t)vga_color << 24 | (uint32_t)' ' << 16 | (uint32_t)vga_color << 8 | ' ';
        uint32_t *long_buffer = (uint32_t *)((unsigned long)&buffer[0] + (VGA_HEIGHT-1) * VGA_WIDTH * sizeof(vga_entry));

        for(i = 0; i < VGA_WIDTH/2; i++) {
            long_buffer[i] = x;
        }

        row--;
    }
}

void terminal_writestring(char *s)
{
    while(*s)
        terminal_putchar(*s++);
}

