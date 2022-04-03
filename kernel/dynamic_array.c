
#include "dynamic_array.h"
#include "heap.h"

#include "com.h"
#include <stdio.h>

typedef struct _dArray {
    uint32_t elemSize;
    uint32_t cap;
    uint32_t i;
    void *data;
};

d_array_t *d_array_create(uint32_t elemSize, uint32_t initCap)
{
    d_array_t *a = kmalloc(sizeof(d_array_t));
    a->elemSize = elemSize;
    a->i = 0;

    if(initCap) {
        a->cap = initCap;
        a->data = kmalloc(initCap * elemSize);
    } else {
        a->cap = 0;
        a->data = 0;
    }

    return a;
}

void d_array_reset(d_array_t *arr)
{
    arr->i = 0;
}

void d_array_pop(d_array_t *arr)
{
    arr->i--;
}

static inline void growArray(d_array_t *arr, uint32_t index)
{
    uint32_t newCap = arr->cap;
    while(index >= newCap) {
        newCap <<= 1;
    }

    if(newCap > arr->cap) {
        arr->cap = newCap;
        arr->data = krealloc(arr->data, newCap * arr->elemSize);
    }
}

static inline void *get_index_pointer(d_array_t *arr, uint32_t index)
{
    return (void *)((uint32_t)arr->data + (index * arr->elemSize));
}

// this resizes array if necessary, so returned
// pointer is alway writable
void *d_array_get_pointer(d_array_t *arr, uint32_t index){
    growArray(arr, index);
    return get_index_pointer(arr, index);
}

// returns pointer to next free
// item, it is always a valid pointer
// also increases internal item count
void *d_array_get_first_free(d_array_t *arr){
    uint32_t i = arr->i++;
    growArray(arr,i);
    void *result = get_index_pointer(arr, i);
    return result;
}

// Use these two function to iterate over
// items
uint32_t d_array_get_item_count(d_array_t *arr)
{
    return arr->i;
}

void *d_array_get_first_item(d_array_t *arr)
{
    return arr->data;
}