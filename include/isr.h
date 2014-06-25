/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * isr.h -- Interface and structures for high level interrupt service routines.
 */

#ifndef ISR_H
#define ISR_H

#include "common.h"

/* IRQ definitions (to make life easier) */
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

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

/* Enables registration of callbacks for interrupts or IRQs.
 * For IRQ, to ease confusion, use the #defines above as the first parameter */
typedef void (*isr_t)(registers_t *);
void register_interrupt_handler (u8int number, isr_t handler);

#endif
