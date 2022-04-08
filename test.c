#include <stdint.h>
#include <stdio.h>

static inline uint8_t hash_adr(void *p) {
    uint32_t x = (uint32_t)p;

    uint8_t hash = x & 0xF;
    while(x) {
        x >>= 4;
        hash ^= x & 0xF;
    }
    return hash;
}

int main(int arc, char *argv[])
{
    char *p = 0xC010B000;

    while(p++ < 0xC010B030)
        printf("%X -> %X\n", p, hash_adr(p));
}