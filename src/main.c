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

	monitor_write ("Successfuly initialised paging.\n");

    u32int a = kmalloc (8);
    u32int b = kmalloc (8);
    u32int c = kmalloc (8);

    /* Print a hello world like string to the screen. */
    monitor_write ("Hello, from Fotix.\n");
    monitor_write ("Copyright (c) 2013 Fotis Koutoulakis\n");
    monitor_write ("http://www.fotiskoutoulakis.com\n");

    monitor_write ("a: ");
    monitor_write_hex (a);
    monitor_write (", b: ");
    monitor_write_hex (b);
    monitor_write ("\nc: ");
    monitor_write_hex (c);

    kfree (c);
    kfree (b);
    u32int d = kmalloc (12);
    monitor_write (", d: ");
    monitor_write_hex (d);

    return 0;
}
