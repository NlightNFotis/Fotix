#!/bin/bash
# Script to update the kernel image and run fotix
# Copyright (C) 2013 Fotis Koutoulakis

# Updating the image stored in the floppy disk drive
# dd if=kernel of=/dev/fd0

sudo losetup /dev/loop0 floppy.img
sudo mount /dev/loop0 /mnt
sudo cp kernel /mnt/kernel
sudo umount /dev/loop0
sudo losetup /dev/loop0

# Initialize QEMU with our image
qemu -fda /dev/loop0 --boot a -no-fd-bootchk &
