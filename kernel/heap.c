#include "heap.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "page_frame_allocator.h"
#include "page.h"

#define CHUNK_USED (1 << 0)
#define PREV_USED (1 << 1)

#define MIN_CHUNK 8

// when we allocate a chunk we
// use one 32 bit int at the header
// and another at the footer
#define OVERHEAD (2 * sizeof(uint32_t))
#define FLAGS_MASK 7
#define SIZEMASK ~FLAGS_MASK

typedef struct _chunk {
    // SIZE is alinged to 8 byte boundary
    // last 3 bits used for flags
    uint32_t size;
    // when chunk is free, we put next
    // chunk adress in free list here,
    // when chunk is allocated, this
    // is invalid and should not be
    // written to or read from.
    struct _chunk *next;
} chunk_t;

static chunk_t *freelist;

unsigned long heapend;
unsigned long heapstart;




static inline void WRITE_FOOTER(chunk_t *p)
{

    uint32_t size = p->size & SIZEMASK;
    uint32_t *footer = (uint32_t*)(&p->next);
    footer += (size / sizeof(uint32_t));
    *footer = size;
}

static inline chunk_t *FIND_ADJ_NEXT(chunk_t *p)
{
    uint32_t next = (uint32_t)p;
    uint32_t size = p->size & SIZEMASK;
    size += OVERHEAD;
    next += size;
    return (chunk_t *)next;
}

static inline chunk_t *FIND_ADJ_PREV(chunk_t *p)
{
    uint32_t prev = (uint32_t)p;
    prev -= sizeof(uint32_t);
    prev -= *(uint32_t*)prev;
    prev -= sizeof(uint32_t);
    return (chunk_t *)prev;
}

static inline chunk_t *merge_back(chunk_t *c)
{
    while(c && ! (c->size & PREV_USED))
    {
        chunk_t *prev = FIND_ADJ_PREV(c);
        prev->size += (c->size & SIZEMASK) + OVERHEAD;
        WRITE_FOOTER(prev);
        c = prev;
    }
    c->size |= PREV_USED;
    return c;
}

static inline chunk_t *merge_forward(chunk_t *c)
{
    chunk_t *next = FIND_ADJ_NEXT(c);
    
    while(next < heapend && ! (next->size & CHUNK_USED))
    {
        c->size += (next->size & SIZEMASK) + OVERHEAD;
        WRITE_FOOTER(c);
        next = FIND_ADJ_NEXT(c);
    }

    if(next < heapend)
        next->size &= ~PREV_USED;
        
    return c;
}

void init_heap()
{
    // we will find first unmapped
    // page above 0xc0000000 and start
    // the heap there

    uint32_t i = 0xC0000;

    while(PAGE_TABLES[i] & PAGE_PRESENT)
        i++;

    i <<= 12;
    heapstart = i;
    freelist = (chunk_t *)i;
    map_page(alloc_page_frame(), freelist, PAGE_WRITE);
    heapend = heapstart + 4096;
    // set prev used flags to prevent
    // coalescing go behind heap start
    freelist->size = (4096 - OVERHEAD) | PREV_USED;
    freelist->next = 0;

    WRITE_FOOTER(freelist);
}

static inline void listChunks()
{
    char *chunk_types[] = {
        "CHUNK FREE, PREV FREE",
        "CHUNK USED, PREV FREE",
        "CHUNK FREE, PREV USED",
        "CHUNK USED, PREV USED"
    };

    chunk_t *c = heapstart;
    printf("Listing Chunks\n");
    while(c < heapend) {
        chunk_t *next = FIND_ADJ_NEXT(c);

        printf("%X-%X (%d bytes, %s)\n", c, next, c->size & SIZEMASK, chunk_types[(c->size) & 7]);

        c = next;
    }
}

static chunk_t *grow_heap(uint32_t size)
{

    uint32_t i;



    size += OVERHEAD;
    size += (-size) % 4096;
    chunk_t *c = heapend;

    for(i = 0; i < size / 4096; i++)
    {
        map_page(alloc_page_frame(), heapend, PAGE_WRITE);
        heapend += 4096;
    }

    /*
    * Caution: If we merge back successfully,
    * we must not add this chunk to free list again
    * it would cause wrong behaviour
    */

    chunk_t *prev = FIND_ADJ_PREV(c);
    if(prev->size & CHUNK_USED) {
        c->size = size - OVERHEAD;
        c->next = 0;
        WRITE_FOOTER(c);
        c->size |= PREV_USED;
        c->next = freelist;
        freelist = c;
        return c;
    } else {
        prev->size += size;
        WRITE_FOOTER(prev);
        return prev;
    }
}

void *kmalloc(uint32_t size)
{
    printf("Kmalloc called with %d\n", size);

    if(!size)
        return 0;

    size += (-size) % 8;

    chunk_t *search;
    chunk_t **p;

    for(search = freelist;
        search && search->size < size;
        search = search->next)
        ; /* first chunk that is big enough */
    
    if(search == 0) {
        // there is no usable chunk that is large enough
        search = grow_heap(size);
    }

    if(size < search->size - OVERHEAD - MIN_CHUNK)
    {
        // remaining space is large enough to split
        chunk_t *next = (chunk_t *)((unsigned long)search + size + OVERHEAD);
        next->size = (search->size - size - OVERHEAD) & SIZEMASK;
        next->next = search->next;
        search->next = next;

        // set new size but keep flags
        search->size = size | search->size & FLAGS_MASK;
        WRITE_FOOTER(search);
        WRITE_FOOTER(next);
    }

    search->size |= CHUNK_USED;
    chunk_t *adj = FIND_ADJ_NEXT(search);
    if(adj < heapend) {
        adj->size |= PREV_USED;
    }

    // p should point to pointer that points to search
    p = &freelist;
    while(*p != search)
        p = &((*p)->next);
    
    *p = (*p)->next;

    listChunks();
    return &search->next;
}

// This is a stupid placeholder
// realloc function, normally
// we should grow or shrink chunks
// if possible without moving data
void *krealloc(void *p, uint32_t size)
{
    if(!p)
        return;

    chunk_t *c = (chunk_t *)((uint32_t)p - sizeof(uint32_t));
    
    uint32_t oldsize = c->size & SIZEMASK;

    void *p2 = kmalloc(size);
    memcpy(p2, p, oldsize >= size ? size : oldsize);
    kfree(p);
    return p2;
}

void kfree(void *p)
{


    if(!p)
        return;

    chunk_t *c = (chunk_t *)((uint32_t)p - sizeof(uint32_t));
    c->size &= ~CHUNK_USED;
    chunk_t *next = FIND_ADJ_NEXT(c);
    if(next < heapend)
        next->size &= ~PREV_USED;

    if(! (c->size & PREV_USED)) {
        // if prev chunk is free
        // it is already in free list
        // so we append to it
        chunk_t *prev = FIND_ADJ_PREV(c);
        prev->size += (c->size & SIZEMASK) + OVERHEAD;
        WRITE_FOOTER(prev);
    } else {
        // put this chunk in free list
        c->next = freelist;
        freelist = c;
    }

    listChunks();
}

