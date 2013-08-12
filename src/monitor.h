/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * monitor.h -- Define the interface for monitor.h
 */

#ifndef MONITOR_H
#define MONITOR_H

/* Write a single character to the screen. */
void monitor_put (char c);

/* Clear the screen to all black. */
void monitor_clear ();

/* Output a null-terminated ASCII string to the monitor. */
void monitor_write (char *c);

#endif
