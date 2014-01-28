/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * memdetect.c - Routines to ask the BIOS for the amount of memory
 * on the computer atm.
 */

#include "common.h"
#include "memdetect.h"

/* just for a test */
asm (".code16gcc\n");

/* XXX: SHIT! This won't work 'cause I am working on protected (32bit mode)
 * I either have to get back to real mode (16bit) momentarily to make it work, or, look into
 * virtual 8086 mode. SHIT!
 */

/* load memory map to buffer - note: regparm(3) avoids stack issues with gcc in realmode */
int __attribute__((noinline)) __attribute__((regparm(3)))
detectMemory (SMAP_entry_t *buffer, int maxentries)
{
    u32int contID  = 0,
           entries = 0,
           signature,
           bytes;
    
    do
      {
        asm __volatile__ ("int $0x15"
            : "=a"(signature), "=c"(bytes), "=b"(contID)
            : "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(buffer));
        if (signature != 0x534D4150)
            return -1; /* erroneous state */
        if (bytes > 20 && (buffer->ACPI & 0x0001) == 0)
          {
            /* ignore this entry */
          }
        else
          {
            buffer++;
            entries++;
          }
      }
    while (contID != 0 && entries < maxentries);
    return entries;
}
