#include <string.h>

// naive implementation
int memcmp ( const void * ptr1, const void * ptr2, size_t num )
{
    size_t i;
    const unsigned char *p1 = ptr1;
    const unsigned char *p2 = ptr2;

    for(i = 0; i < num; i++)
    {
        unsigned char c1 = p1[i];
        unsigned char c2 = p2[i];
        int d = c1-c2;
        if(d != 0)
            return d;
    }

    return 0;
}