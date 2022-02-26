
#ifndef _BOOT_H
#define _BOOT_H

// writes byte, short or int to a output port
void outb(unsigned int port, unsigned int  data);
void outh(unsigned int  port, unsigned int  data);
void out(unsigned int  port, unsigned int  data);

unsigned int inb(unsigned int  port);
unsigned int inh(unsigned int  port);
unsigned int in(unsigned int port);
#endif