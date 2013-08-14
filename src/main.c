/* The main entry point for the fotix kernel.
 * Copyright (c) 2013 Fotis Koutoulakis.
 *
 * main.c -- Defines the C-code kernel entry point,
 * and also calls initialisation routines.
 */

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"

int
main (struct multiboot *mboot_ptr)
{
    /* Initialise all the ISRs and segmentation */
    init_descriptor_tables ();
    /* Initialise the screen (by clearing it) */
    monitor_clear ();

    /* Initialise paging */
    initialise_paging ();

    /* Print a hello world like string to the screen. */
    monitor_write ("Hello, from Fotix.\n");
    monitor_write ("Copyright (c) 2013 Fotis Koutoulakis\n");
    monitor_write ("http://www.fotiskoutoulakis.com\n");

    u32int *ptr = (u32int *) 0xA0000000;
    u32int do_page_fault = *ptr;

    return 0;
}
