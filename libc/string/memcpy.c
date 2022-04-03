
#include <string.h>
#include <stdio.h>

/*
* Implement memcpy as if it were memmove so we don't implement it twice
*/

#define ALINGMASK (sizeof(int)-1)

static inline void copy_bytes_forward(void *dest, const void *src, size_t n)
{
    char *_dest = dest;
    const char *_src = src;
    while(n) {
        *_dest++ = *_src++;
        n--;
    }
}

static inline void copy_bytes_backwards(void *dest, const void *src, size_t n)
{

    char *_dest = dest;
    const char *_src = src;
    while(n) {

        *_dest-- = *_src--;
        n--;
    }
}

// n is number of ints to copy
static inline void copy_ints_forward(void *dest, const void *src, size_t n)
{

    int *_dest = dest;
    const int *_src = src;
    while(n) {
        *_dest++ = *_src++;
        n--;
    }
}

static inline void copy_ints_backwards(void *dest, const void *src, size_t n)
{

    int *_dest = dest;
    const int *_src = src;

    while(n) {
        *_dest-- = *_src--;
        n--;
    }
}

static void *memcpy_backwards(void *dest, const void *src, size_t n)
{
    // point to end of arrays
    unsigned long destp = (unsigned long)dest + n - 1;
    unsigned long srcp = (unsigned long)src + n - 1;
    
    if(n > sizeof(int)*4)
    {
        size_t len = destp & ALINGMASK;
        copy_bytes_backwards((void*)destp, (void*)srcp, len);
        destp -= len;
        srcp  -= len;
        n -= len;

        len = n / sizeof(int);
        copy_ints_backwards((void*)destp, (void*)srcp, len);
        destp -= len * sizeof(int);
        srcp -= len * sizeof(int);
        n -= len * sizeof(int);
    }

    copy_bytes_backwards((void*)destp, (void*)srcp, n);

    return dest;
}

static void *memcpy_forwards(void *dest, const void *src, size_t n)
{


    unsigned long destp = (unsigned long)dest;
    unsigned long srcp = (unsigned long)src;

    if(n > sizeof(int)*4) { // does it worth the trouble of aligned copy?
        
        // align dest pointer
        size_t len = (-destp) & ALINGMASK;
        copy_bytes_forward((void*)destp, (void*)srcp, len);

        destp += len;
        srcp += len;
        n -= len;

        // make aligned copy
        len = n / sizeof(int);
        copy_ints_forward((void*)destp, (void*)srcp, len);

        destp += len * sizeof(int);
        srcp += len * sizeof(int);
        n -= len * sizeof(int);
    }

    // copy remaining bytes;
    copy_bytes_forward((void*)destp, (void*)srcp, n);

    return dest;
}

void *memcpy(void *dest, const void *src, size_t n)
{

    if(dest == src)
        return dest;

    if(src > dest)
        return memcpy_forwards(dest, src, n);
    else
        return memcpy_backwards(dest, src, n);
}

void *memmove(void *dest, const void *src, size_t n)
{
    return memcpy(dest, src, n);
}