
HOST=i686-elf
HOSTARCH=i386
SYSROOT=/mnt/c/Users/yasar/osdev/sysroot

CROSS_COMPILER_BIN_DIR=/home/yasar/opt/cross/bin

INCLUDEDIR=/usr/include
BOOTDIR=/boot

AR=$(CROSS_COMPILER_BIN_DIR)/$(HOST)-ar
AS=$(CROSS_COMPILER_BIN_DIR)/$(HOST)-as
CC=$(CROSS_COMPILER_BIN_DIR)/$(HOST)-gcc

include $(HOSTARCH)/make.config

CC:=$(CC) --sysroot=$(SYSROOT) -isystem=$(INCLUDEDIR)

CFLAGS=-g -O2 -ffreestanding -Wall $(KERNEL_ARCH_CFLAGS)
CPPFLAGS=-D__is_kernel -Iinclude $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS=$(KERNEL_ARCH_LDFLAGS)
LIBS=-nostdlib -lk -lgcc $(KERNEL_ARCH_LIBS)

KERNEL_OBJS= \
$(KERNEL_ARCH_OBJS) \
kernel.o \
com.o \
PIC.o \
page_frame_allocator.o \
heap.o \
page.o \

OBJS= $(KERNEL_OBJS)
LINK_LIST=$(LDFLAGS) $(KERNEL_OBJS) $(LIBS)

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .s

all: kernel.bin

kernel.bin: $(OBJS) $(HOSTARCH)/linker.ld
	$(CC) -T $(HOSTARCH)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $@

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f kernel.bin
	rm -f $(OBJS)
	rm -f $(OBJS:.o=.d)

install: install-headers install-kernel

install-headers:
	mkdir -p $(SYSROOT)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(SYSROOT)$(INCLUDEDIR)/.

install-kernel:
	mkdir -p $(SYSROOT)$(BOOTDIR)
	cp kernel.bin $(SYSROOT)$(BOOTDIR)

-include $(OBJS:.o=.d)