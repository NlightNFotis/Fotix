/* The main entry point for the fotix kernel.
 * Copyright (c) 2013 Fotis Koutoulakis.
 *
 * main.c -- Defines the C-code kernel entry point,
 * and also calls initialization routines.
 */

#include "common.h"
#include "monitor.h"
#include "timer.h"

int
main (struct multiboot *mboot_ptr)
{
    /* Initialize the descriptors tables, to be able
     * to better utilise memory and user interrupts. */
    init_descriptor_tables ();

    /* Clear the screen from previous artifacts. */
    monitor_clear ();

    /* Print a hello world like string to the screen. */
    monitor_write ("Hello, from Fotix.\n");
    monitor_write ("Copyright (c) 2013 Fotis Koutoulakis\n");
    monitor_write ("http://www.fotiskoutoulakis.com\n");
    
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    asm volatile ("sti");
    
    /* Initialise timer at 50 Hz */
    init_timer (50);

    return 0;
}
