#ifndef _PIC_H
#define _PIC_H

#define MASTER_PIC_COMMAND 0x0020
#define MASTER_PIC_DATA    0x0021
#define SLAVE_PIC_COMMAND  0x00A0
#define SLAVE_PIC_DATA 0x00A1

#define PIC_EOI 0x20

#define ICW1_ICW4       0x01    // ICW4 needed
#define ICW1_SINGLE     0x02  // Single Mode
#define ICW1_INTERVAL4  0x04
#define ICW1_LEVEL      0x08
#define ICW1_INIT       0x10

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM 0x10

#define PIC_EOI 0x20

void PIC_remap(int master_offset, int slave_offset);
void PIC_sendEOI(int irq);

#endif