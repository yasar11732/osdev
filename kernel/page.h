#ifndef _PAGE_H
#define _PAGE_H

#include <stdint.h>

#define PAGE_PRESENT (1 << 0)
#define PAGE_WRITE (1 << 1)
#define PAGE_USERSPACE (1 << 2)
#define PAGE_WRITE_THROUGH (1 << 3)
#define PAGE_DISABLE_CACHE (1 << 4)

#define PAGE_DIRECTORY ((uint32_t *)0xFFFFF000)
#define PAGE_TABLES ((uint32_t *)0xFFC00000) 

void map_page(void *physical, void *virtual, int flags);

#endif