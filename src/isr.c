/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * isr.c -- High level interrupt service routines and
 * interrupt request handlers.
 */

#include "common.h"
#include "isr.h"
#include "monitor.h"

isr_t interrupt_handlers[256];

void 
register_interrupt_handler (u8int num, isr_t handler)
{
    interrupt_handlers[num] = handler;
}

/* This get's called from our ASM interrupt handler stub. */
void 
isr_handler (registers_t regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler (regs);
    }
    else
    {
        monitor_write ("unhandled interrupt: ");
        monitor_write_dec (regs.int_no);
        monitor_put ('\n');
    }
}

/* This get's called from our ASM interrupt handler stub. */
void
irq_handler (registers_t regs)
{
    /* Send an EOI (end of interrupt) signal to the PICS.
       if the IRQ was > 7 (interrupt number > 40), send
       a signal to the slave.

       In either way send one to the master too. */
    if (regs.int_no >= 40)
        {
            /* Send reset signal to slave */
            outb (0xA0, 0x20);
        }

    /* Send reset signal to master */
    outb (0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
        {
            isr_t handler = interrupt_handlers[regs.int_no];
            handler (regs);
        }
}
