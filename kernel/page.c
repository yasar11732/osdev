
#include "page.h"
#include "page_frame_allocator.h"

static inline void invlpg(void *add)
{
    __asm__ __volatile__ ("invlpg (%0)"
    : /* no output */
    : "r"(add) /* put address into a register */
    : "memory" /* memory is clobbered */
    );
}

void map_page(void *physical, void *virtual, int flags)
{    
    unsigned long pdindex = (uint32_t)virtual >> 22;
    unsigned long ptindex = (uint32_t)virtual >> 12 & 0b1111111111;

    // check if page table is allocated
    if(! (PAGE_DIRECTORY[pdindex] & PAGE_PRESENT) )
    {
        void *p = alloc_page_frame();
        p = (uint32_t)p | PAGE_PRESENT | PAGE_WRITE;
        PAGE_DIRECTORY[pdindex] = p;
    }

    uint32_t current_map = PAGE_TABLES[1024 * pdindex + ptindex];
    if(current_map & PAGE_PRESENT)
    {
        free_page_frame(current_map & 0xFFFF000);
    }

    PAGE_TABLES[1024 * pdindex + ptindex] = (uint32_t)physical | flags | PAGE_PRESENT;
    invlpg(virtual);
}