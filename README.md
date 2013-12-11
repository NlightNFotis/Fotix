fotix is a unix like clone.  
Copyright (c) 2013 Fotis Koutoulakis

Introduction
============

fotix is a unix like kernel that I made, mostly for self-education / self-amusement.

Features:

- Interrupt handling.
- Multitasking capable
- Some (primal) memory management
- A (primal) scheduler

Instructions
============

## Prerequisites

To build fotix, you should have a cross-compiler. I personally used GCC-4.9 for
my development purposes, targeted at i586. It goes without saying that you 
should also have binutils. You should also have nasm. 

To build the iso, you should also have installed `grub-mkrescue` and `xorriso`.
To run the iso, you can use any virtual machine hypervisor, although I personally
use QEMU, and the one-step-build script is automated to use QEMU.

## Building instructions

If you do have all that, just run `make` at the top of the project's root directory
and then run the `update_and_run.sh` script (as root). This will create an ISO,
and run it with QEMU.

If you don't want to use QEMU, simply modify `update_and_run.sh` to not delete the
iso image, so that you can use it with another hypervisor, or just replace QEMU
with your favourite hypervisor.

I should have a cross compiler built (along with binutils)
 - There is one at earth. It is under $HOME/bin/cross/
 - To use it for the session I have to do `export PATH="$HOME/bin/cross/bin:$PATH"`

## Known bugs

 - When a fork()-ed process attemtps to run, we get a page fault, 
   which results on a kernel panic.

## Notes

```
02:50:34 nlightnfotis@earth ~/osdev/fotix $ cloc .
      37 text files.
      37 unique files.
     353 files ignored.

http://cloc.sourceforge.net v 1.56  T=1.0 s (33.0 files/s, 3337.0 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C                               14            325            428           1488
C/C++ Header                    13            130            190            459
Assembly                         4             39             48            190
make                             1              6              2             14
Bourne Shell                     1              3              4             11
-------------------------------------------------------------------------------
SUM:                            33            503            672           2162
-------------------------------------------------------------------------------
```

TODO
======

 - Implement IPC
 - Implement threading
 - Improve memory management
 - Implement a console
 - Improve the scheduler


LICENSE
========

fotix is distributed under the BSD 3-Clause license. See file LICENSE for more info.
