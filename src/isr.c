/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * isr.c -- High level interrupt service routines and
 * interrupt request handlers.
 */

#include "common.h"
#include "isr.h"
#include "monitor.h"

/* This get's called from our ASM interrupt handler stub. */
void 
isr_handler (registers_t regs)
{
    monitor_write ("received interrupt: ");
    monitor_write_dec (regs.int_no);
    monitor_put ('\n');
}
