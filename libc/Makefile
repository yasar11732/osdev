
HOST=i686-elf
HOSTARCH=i386
SYSROOT=/mnt/c/Users/yasar/osdev/sysroot

CROSS_COMPILER_BIN_DIR=/home/yasar/opt/cross/bin

INCLUDEDIR=/usr/include
BOOTDIR=/boot
LIBDIR=/usr/lib

AR=$(CROSS_COMPILER_BIN_DIR)/$(HOST)-ar
AS=$(CROSS_COMPILER_BIN_DIR)/$(HOST)-as
CC=$(CROSS_COMPILER_BIN_DIR)/$(HOST)-gcc

include $(HOSTARCH)/make.config

CC:=$(CC) --sysroot=$(SYSROOT) -isystem=$(INCLUDEDIR)

CFLAGS=-g -O2 -ffreestanding -Wall -Wextra
CPPFLAGS=-D__is_libc -Iinclude
LIBKCPPFLAGS=-D__is_libk -Iinclude

OBJS=string/strlen.o stdio/putchar.o stdio/printf.o
LIBK_OBJS=$(OBJS:.o=.libk.o)

BINARIES=libk.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(CFLAGS) $(LIBKCPPFLAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(LIBKCPPFLAGS)

clean:
	rm -f $(BINARIES)
	rm -f $(LIBK_OBJS)
	rm -f $(LIBK_OBJS:.o=.d)

install: install-headers install-libs

install-headers:
	mkdir -p $(SYSROOT)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(SYSROOT)$(INCLUDEDIR)/.

install-libs:
	mkdir -p $(SYSROOT)$(LIBDIR)
	cp $(BINARIES) $(SYSROOT)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS=.o=d)