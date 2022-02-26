
.PHONY: all libc kernel clean

all: myos.iso

myos.iso: kernel
	grub-mkrescue -o $@ sysroot

kernel: libc
	cd kernel && make && make install

libc:
	cd libc && make && make install

clean:
	rm -f myos.iso
	cd libc && make clean
	cd kernel && make clean