# Makefile for fotix
# Copyright (c) 2013 Fotis Koutoulakis

SOURCES=src/boot.o src/main.o src/common.o src/monitor.o src/descriptor_tables.o \
	src/isr.o src/interrupt.o src/dt_asm.o src/timer.o src/kheap.o src/paging.o \
	src/ordered_array.o src/fs.o src/initrd.o src/task.o src/process.o src/syscall.o \
	src/keyboard.o src/memdetect.o

RAMDISK_INIT=utils/mkinit.c
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf
CC=i586-elf-gcc

all: $(SOURCES) link

clean:
	rm -rf temp/
	rm rd_init
	rm initrd.img
	rm src/*.o kernel

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<

ramdisk:
	gcc $(RAMDISK_INIT) -o rd_init
	mkdir temp
	touch temp/test.txt
	touch temp/test1.txt
	touch temp/test2.txt
	./rd_init temp/test.txt temp/test1.txt temp/test2.txt
