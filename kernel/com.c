
#include "com.h"
#include "i386/io.h"
#include <stdio.h>

int is_transmit_empty(int port)
{
    return inb(COM_LINE_STAT(port)) & 0x20;
}

void serial_putchar(int port, char c)
{
    while(is_transmit_empty(port) == 0) ;
    outb(port, c);
}

void serial_puts(int port, char *s)
{
    while(*s)
        serial_putchar(port, *s++);
    serial_putchar(port, '\n');
}

int init_serial(int port, int div)
{
    printf("init serial called with port %d, divisor %d\n", port, div);
    // disable interrupts
    outb(COM_INTERRUPT_ENBL(port), 0x0);

    //enable dlab
    outb(COM_LINE_CTRL(port), 0x80);
    
    // set divisor
    outb(COM_DIV_LO(port), div & 0xFF);
    outb(COM_DIV_HI(port), div >> 8);

    // set line properties
    outb(COM_LINE_CTRL(port), COM_DATA_BITS(8) | COM_PARITY_NONE | COM_STOP_BIT_1);
    
    // set FIFO
    outb(COM_FIFO_CTRL(port), COM_FIFO_ENABLE | COM_CLR_RECIEVER | COM_CRL_TRANSMITTER | COM_FIFO_TRIGGER_14);

    // go into loopback mode
    outb(COM_MODEM_CTRL(port), COM_ENABLE_DTR | COM_ENABLE_RTS | COM_ENABLE_OUT2);
    outb(COM_MODEM_CTRL(port), COM_ENABLE_RTS | COM_ENABLE_OUT1 | COM_ENABLE_OUT2 | COM_ENABLE_LOOPBACK);

    outb(port, 0xCA);
    if(inb(port) != 0xCA) return -1;

    outb(port, 0xFE);
    if(inb(port) != 0xFE) return -1;

    outb(port, 0xBA);
    if(inb(port) != 0xBA) return -1;

    outb(port, 0xBE);
    if(inb(port) != 0xBE) return -1;

    // it works, go into regular mode
    outb(COM_MODEM_CTRL(port), COM_ENABLE_DTR | COM_ENABLE_RTS | COM_ENABLE_OUT1 | COM_ENABLE_OUT2);
    return 0;
}