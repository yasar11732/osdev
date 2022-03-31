
#include "PIC.h"
#include "i386/io.h"

void PIC_sendEOI(int irq)
{
	if(irq >= 8)
		outb(SLAVE_PIC_COMMAND,PIC_EOI);
 
	outb(MASTER_PIC_COMMAND,PIC_EOI);
}

void PIC_remap(int master_offset, int slave_offset)
{
    unsigned char a1, a2;

    a1 = inb(MASTER_PIC_DATA);
    a2 = inb(SLAVE_PIC_DATA);

    outb(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4); // init the pic, use 4 commands
    outb(SLAVE_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);

    outb(MASTER_PIC_DATA, master_offset);
    outb(SLAVE_PIC_DATA, slave_offset);

    outb(MASTER_PIC_DATA, 0x4);
    outb(SLAVE_PIC_DATA, 0x2);

    outb(MASTER_PIC_DATA, ICW4_8086);
    outb(SLAVE_PIC_DATA, ICW4_8086);

    outb(MASTER_PIC_DATA, a1);
    outb(SLAVE_PIC_DATA, a2);
}