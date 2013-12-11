#!/bin/bash
# 1 step build script
# Copyright (C) 2013 Fotis Koutoulakis

# Begin building fotix
make all
make ramdisk

# Start creating an iso
mkdir -p isodir/
mkdir -p isodir/boot
cp kernel isodir/boot/fotix
mkdir -p isodir/boot/grub
cp grub.cfg isodir/boot/grub/grub.cfg
cp initrd.img isodir/boot/initrd.img
grub2-mkrescue -o fotix.iso isodir

# Initialize QEMU with our image
qemu-system-i386 -cdrom fotix.iso

# Cleaning up after qemu has run
rm -rf isodir/
rm fotix.iso
