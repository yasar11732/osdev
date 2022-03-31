#ifndef _COM_H
#define _COM_H

#define COM1 0x3f8

#define COM_DIV_LO(C) (C)
#define COM_DIV_HI(C) (C+1)

#define COM_INTERRUPT_ENBL(C) (C+1)
#define COM_INTR_IDEN(C) (C+2)
#define COM_FIFO_CTRL(C) (C+2)
#define COM_LINE_CTRL(C) (C+3)
#define COM_MODEM_CTRL(C) (C+4)
#define COM_LINE_STAT(C) (C+5)
#define COM_MODEM_STAT(C) (C+6)
#define COM_STRATCH(C) (C+7)

#define COM_DATA_BITS(n) (n-5)

#define COM_STOP_BIT_1 0
#define COM_STOP_BIT_2 (1 << 2)

#define COM_PARITY_NONE 0
#define COM_PARITY_ODD (1 << 3)
#define COM_PARITY_EVEN (3 << 3)
#define COM_PARITY_MARK (5 << 3)
#define COM_PARITY_SPACE (7 << 3)

#define COM_FIFO_ENABLE 1
#define COM_CLR_RECIEVER 2
#define COM_CRL_TRANSMITTER 4
#define COM_DMA_MODE 8

#define COM_FIFO_TRIGGER_1 0
#define COM_FIFO_TRIGGER_4 (1 << 6)
#define COM_FIFO_TRIGGER_8 (1 << 7)
#define COM_FIFO_TRIGGER_14 (COM_FIFO_TRIGGER_4 | COM_FIFO_TRIGGER_8)

#define COM_ENABLE_DTR (1 << 0)
#define COM_ENABLE_RTS (1 << 1)
#define COM_ENABLE_OUT1 (1 << 2)
#define COM_ENABLE_OUT2 (1 << 3)
#define COM_ENABLE_LOOPBACK (1 << 4)

int init_serial(int port, int div);
void serial_puts(int port, char *s);
void serial_putchar(int port, char c);

#endif