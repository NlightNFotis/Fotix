# Makefile for fotix
# Copyright (c) 2013 Fotis Koutoulakis

CC=i586-elf-gcc

SOURCES=src/boot.o src/main.o
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm src/*.o src/kernel

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
