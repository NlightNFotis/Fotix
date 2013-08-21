;
; boot.s -- the kernel bootstrapper.
; This file is part of fotix
; Copyright (c) 2013 Fotis Koutoulakis
;

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002  ; Multiboot Magic value
; NOTE to self: I shouldn't use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]                   ; All instructions should be 32 bit

[GLOBAL mboot]              ; Make 'mboot' accessible from C.
[EXTERN code]               ; Start of the '.text' section.
[EXTERN bss]                ; Start of the .bss section
[EXTERN end]                ; End of the last loadable section

mboot:
    dd MBOOT_HEADER_MAGIC   ; GRUB will search for this value on each
                            ; 4 byte boundary in your kernel file
    dd MBOOT_HEADER_FLAGS   ; How GRUB should load your file / settings
    dd MBOOT_CHECKSUM       ; To ensure that the above values are correct

    dd mboot                ; Location of this descriptor
    dd code                 ; Start of kernel '.text' (code) section
    dd bss                  ; End of kernel '.data' section
    dd end                  ; End of kernel
    dd start                ; Kernel entry point (Initial EIP).

[GLOBAL start]              ; Kernel entry point.
[EXTERN kernel_start]       ; This is the entry point of our C Code

start:
    ; Load multiboot information:
    push esp
    push ebx

    ; Execute the kernel:
    cli                     ; Disable all interrupts.
    call kernel_start       ; call our main() function.
    
    ; Halt after we are done with it:
    hlt
