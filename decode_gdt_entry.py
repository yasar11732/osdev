a = [0xFF,0xFF,0x00,0x00,0x00,0x93, 0xCF, 0x00]

def parse_gdt(gdt):
    limit = a[0]
    limit |= a[1] << 8
    limit |= (a[5] & 31) << 16
    print(limit)

parse_gdt(a)