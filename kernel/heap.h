#ifndef _HEAP_H
#define _HEAP_H

#include <stdint.h>

void init_heap();
void kfree(void *p);
void *kmalloc(uint32_t size);
void *krealloc(void *p, uint32_t size);

#endif