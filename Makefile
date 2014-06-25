# Makefile for fotix
# Copyright (c) 2013 Fotis Koutoulakis

SOURCES=sys/boot.o sys/main.o sys/common.o sys/monitor.o sys/descriptor_tables.o \
	sys/isr.o sys/interrupt.o sys/dt_asm.o sys/timer.o sys/kheap.o sys/paging.o \
	sys/ordered_array.o sys/fs.o sys/initrd.o sys/task.o sys/process.o sys/syscall.o \
	sys/keyboard.o

RAMDISK_INIT=utils/mkinit.c
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32 -I./
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf
CC=i586-elf-gcc

all: $(SOURCES) link

clean:
	rm sys/*.o kernel
	rm -rf temp/
	rm rd_init
	rm initrd.img

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
