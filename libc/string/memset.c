#include <string.h>

void * memset ( void * ptr, int value, size_t num )
{

    unsigned long destp = (unsigned long)ptr;

    // try aligned 32 bit if it is worth the trouble
    if(num > 8*sizeof(int)) {
        size_t len = (-destp) & (sizeof(int)-1);
        switch(len){
            case 3:
                ((unsigned char *)destp)[2] = value;
                // fall through
            case 2:
                ((unsigned char *)destp)[1] = value;
                // fall through
            case 1:
                ((unsigned char *)destp)[0] = value;
        };

        destp += len;
        num -= len;

        int _val = value << 24 | value << 16 | value << 8 | value;
        while(num >= sizeof(int))
        {
            ((uint32_t *)destp)[0] = _val;
            destp+=sizeof(int);
            num -= sizeof(int);
        }
        
    }

    // set remaining bytes one at a time
    while(num) {
        *((char *)destp) = value;
        destp++;
        num--;
    }

    return ptr;
}