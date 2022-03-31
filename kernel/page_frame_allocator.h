
#ifndef _PAGE_FRAME_ALLOCATOR_H
#define _PAGE_FRAME_ALLOCATOR_H

#include "multiboot.h"
#include <stddef.h>

void init_page_frame_allocator(multiboot_memory_map_t *mmm, size_t num_entries);
void *alloc_page_frame();
void free_page_frame(void *p);

#endif