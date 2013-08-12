/* The main entry point for the fotix kernel.
 * Copyright (c) 2013 Fotis Koutoulakis.
 *
 * main.c -- Defines the C-code kernel entry point,
 * and also calls initialization routines.
 */

#include "common.h"
#include "monitor.h"

int
main (struct multiboot *mboot_ptr)
{
    /* Clear the screen from previous artifacts. */
    monitor_clear ();

    /* Print a hello world like string to the screen. */
    monitor_write ("Hello, from Fotix.\n");

    return 0xDEADBABA;
}
