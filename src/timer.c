/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * timer.c -- Initialises the PIT, and handles clock updates.
 */

#include "timer.h"
#include "isr.h"
#include "monitor.h"

u32int tick = 0;

static void
timer_callback (registers_t reg)
{
    tick++;
    monitor_write ("Tick: ");
    monitor_write_dec (tick);
    monitor_write("\n");
}

void
init_timer (u32int frequency)
{
    monitor_write ("Initializing timer.");

    /* Firstly register our timer callback. */
    register_interrupt_handler (IRQ0, &timer_callback);

    /* The value we send to the PIT is the vale to divide it's input clock
     * (1193180 HZ) by, to get our required frequency. Import to note is
     * that the divisor must be small enough to fit into 16bits. */
    u32int divisor = 1193180 / frequency;

    /* Send the command byte */
    outb (0x43, 0x36);

    /* Divisor has to be set byte-wise, so split here into upper/lower bytes */
    u8int lower = (u8int) (divisor & 0xFF);
    u8int upper = (u8int) ((divisor >> 8) & 0xFF);

    /* Send the frequency divisor */
    outb (0x40, lower);
    outb (0x40, upper);
}
