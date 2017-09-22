/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * monitor.h -- Define the interface for monitor.h
 */

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

/* Write a single character to the screen. */
void monitor_put(char c);

/* Clear the screen to all black. */
void monitor_clear();

/* Output a null-terminated ASCII string to the monitor. */
void monitor_write(char *c);

/* Output a decimal number on the monitor */
void monitor_write_dec(u32int number);

/* Output a hexadecimal number on the monitor */
void monitor_write_hex(u32int hex_number);

/* Output a padded string (usefull in kernel logging) to the monitor */
void monitor_write_padded(char *message, u8int success);

#endif
