
#include "page_frame_allocator.h"
#include <stdint.h>
#include <string.h>

#define PAGESIZE 0x1000
#define HIGHER_HALF_BASE 0xC0000000
#define KERNEL_LOAD_ADRESS 0x100000

__attribute__((aligned(0x8)))  static uint32_t memory_bitmap[(unsigned long)(-1) / PAGESIZE / sizeof(uint32_t)];

// this is index into memory bitmap to
// start searching for empty frame
uint32_t search_bottom;

// this is index into memory bitmap to
// stop searching for empty frame
uint32_t search_top;

extern unsigned long _kernel_start;
extern unsigned long _text_section_start;
extern unsigned long _rodata_section_start;
extern unsigned long _data_section_start;
extern unsigned long _bss_section_start;
extern unsigned long _kernel_end;

/*
* Marks continous page blocks as reserved or available
* start: physical adress of first page(must be aligned to PAGESIZE)
* value: 1 if available, 0 if reserved
* len: number of pages to set 
*/
static void set_page_frames(unsigned long start, int value, size_t len)
{
    unsigned long _start_bit = start / PAGESIZE;

    // if we are setting lots of bits
    // 
    if(len >= 128) {

        unsigned long _misaligment = (-_start_bit) % 32;
        len -= _misaligment;


        while(_misaligment--) {
            if(value)
                memory_bitmap[_start_bit / 32] |= (uint32_t)1 << (_start_bit % 32);
            else
                memory_bitmap[_start_bit / 32] &= ~((uint32_t)1 << (_start_bit % 32));

            _start_bit++;
        }

        uint32_t opsized_val = 0;
        if(value)
            opsized_val = 0xFFFFFFFF;

        while(len >= 32) {
            memory_bitmap[_start_bit / 32] = opsized_val;
            _start_bit += 32;
            len -= 32;
        }
    }


    // handle the remaining bits
    while(len--) {
        if(value)
            memory_bitmap[_start_bit / 32] |= (uint32_t)1 << (_start_bit % 32);
        else
            memory_bitmap[_start_bit / 32] &= ~((uint32_t)1 << (_start_bit % 32));

        _start_bit++;
    }
}

void init_page_frame_allocator(multiboot_memory_map_t *mmm, size_t num_entries)
{
    char buffer[64];
    size_t i;
    /*
    * Step 1: Mark all memory reserved
    * Step 2: Mark available memory
    * Step 3: Find out where kernel data is, and mark
    * those pages used. 
    * 
    * If there is less physical memory then addressable memory
    * they will be marked as used in the first step so we will
    * never touch them.
    */

    // step 1: Mark all used
    memset(&memory_bitmap, 0x0, sizeof(memory_bitmap));


    // step 2: mark available memory regions
    for(i = 0; i < num_entries;i++)
    {
        if(mmm[i].type == MULTIBOOT_MEMORY_AVAILABLE)
        {
           
            unsigned long start = mmm[i].addr_low;
            unsigned long len   = mmm[i].len_low;
            size_t misalignment = (-start) % PAGESIZE;
            start += misalignment;
            len -= misalignment;

            set_page_frames(start, 1, len / PAGESIZE);
            search_top = ((start + len) >> 17) + 1;
        }
    }

    
    unsigned long kernel_start = HIGHER_HALF_BASE + KERNEL_LOAD_ADRESS;
    unsigned long kernel_end = (unsigned long)&_kernel_end;

    kernel_end += (-kernel_end) % PAGESIZE;
    unsigned long kernel_len = kernel_end - kernel_start;


    set_page_frames(KERNEL_LOAD_ADRESS, 0, kernel_len / PAGESIZE);
    search_bottom = (0x100000 + kernel_len) >> 17;

}

void *alloc_page_frame()
{
    uint32_t i;
    for(i = search_bottom; i < search_top; i++)
    {
        if(memory_bitmap[i])
        {
            search_bottom = i;
            uint32_t freebit = __builtin_ffs(memory_bitmap[i]) - 1;
            memory_bitmap[i] &= ~(1 << freebit);
            return ((i * 32) + freebit) * 4096; 
        }
    }

    // couldnt find free page
    return 0;
}

void free_page_frame(void *p)
{
    unsigned long i = (unsigned long)p >> 12;
    unsigned long bit = i&31;
    i /= 32;
    if(i < search_bottom)
        search_bottom = i;

    memory_bitmap[i] |= 1 << bit;
}