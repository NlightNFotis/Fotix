/* The main entry point for the fotix kernel.
 * Copyright (c) 2013 Fotis Koutoulakis.
 *
 * main.c -- Defines the C-code kernel entry point,
 * and also calls initialization routines.
 */

int
main (struct multiboot *mboot_ptr)
{
    // All our initialization code goes here :)
    return 0xDEADBABA;
}
