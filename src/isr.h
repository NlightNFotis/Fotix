/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * isr.h -- Interface and structures for high level interrupt service routines.
 */

#include "common.h"

typedef struct registers
{
    u32int ds,          /* Data segment selector */

           /* Pushed by pusha */
           edi,
           esi,
           ebp,
           esp,
           ebx,
           edx,
           ecx,
           eax,

           int_no,       /* Interrupt number */
           err_code,     /* Error code (if applicable) */
           
           /* Pushed by the processor automatically */
           eip,
           cs,
           eflags,
           useresp,
           ss;
} registers_t;
