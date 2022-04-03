#ifndef _DYNAMIC_ARRAY
#define _DYNAMIC_ARRAY
#include <stdint.h>

typedef struct _dArray d_array_t;

// create a new dynamic array
d_array_t *d_array_create(uint32_t elemSize, uint32_t initCap);

// reset internal item count to 0, without
// changing capacity
void d_array_reset(d_array_t *arr);

// decrease internal item count by 1
void d_array_pop(d_array_t *arr);

// get a pointer to item at index (zero-based)
// this resizes array if necessary, so returned
// pointer is alway writable
void *d_array_get_pointer(d_array_t *arr, uint32_t index);

// returns pointer to next free
// item, it is always a valid pointer
// also increases internal item count
void *d_array_get_first_free(d_array_t *arr);

// Use these two function to iterate over
// items
uint32_t d_array_get_item_count(d_array_t *arr);
void *d_array_get_first_item(d_array_t *arr);

#endif