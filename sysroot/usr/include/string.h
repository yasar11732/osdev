
#ifndef _STRING_H
#define _STRING_H

#include <stdint.h>
#include <stddef.h>

size_t strlen(const char *str);

void *memset ( void * ptr, int value, size_t num );
void *memcpy(void *dest_str, const void *src_str, size_t number);
void *memmove(void *dest_str, const void *src_str, size_t number);
int memcmp ( const void * ptr1, const void * ptr2, size_t num );

char* strcpy(char* destination, const char* source);

#endif