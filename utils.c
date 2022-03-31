
#include <stdint.h>
#include <stdio.h>

struct GDT {
    uint32_t base;
    uint32_t limit;
    uint32_t access_byte;
    uint32_t flags;
};

void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0,%1" : : "a"(val), "Nd"(port));
}

static inline struct GDT make_descriptor(uint32_t base, uint32_t limit, uint32_t access_byte, uint32_t flags) {
    struct GDT g;
    g.base = base;
    g.limit = limit;
    g.access_byte = access_byte;
    g.flags = flags;
    return g;
}

void print_encoded_gdt(struct GDT source) {
    uint8_t target[8];

    if(source.limit > 0xFFFFF) {
        printf("GDT cannot encode limits larger than 0xFFFFF\r\n");
        return;
    }

    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;
 
    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
 
    // Encode the access byte
    target[5] = source.access_byte;
 
    // Encode the flags
    target[6] |= (source.flags << 4);

    uint32_t *target_as_long = (uint32_t *)target;

    printf("%8X %8X\n", 
        target_as_long[0],
        target_as_long[1]);
}

int main(int argc, char *argv[]) {
    struct GDT null_seg = make_descriptor(0,0,0,0);
    struct GDT code_seg = make_descriptor(0x0, 0xFFFFF, 0x9A, 0xC);
    struct GDT data_seg = make_descriptor(0x0, 0xFFFFF, 0x92, 0xC);

    printf("Null Segment: ");
    print_encoded_gdt(null_seg);

    printf("Code Segment: ");
    print_encoded_gdt(code_seg);

    printf("Data Segment: ");
    print_encoded_gdt(data_seg);


}