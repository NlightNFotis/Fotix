/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * descriptor_tables.h -- Defines structs and functions that
 * are going to be used to implement the GDT.
 */

#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

#include "common.h"

/* This structure contains the value of one GDT entry. */
struct gdt_entry_struct
{
    u16int limit_low;       /* The lower 16 bits of the limit */
    u16int base_low;        /* The lower 16 bits of the base */
    u8int base_middle;      /* The next 8 bits of the base */
    u8int access;           /* Access flags, determine what ring this segment can be used in */
    u8int granularity;
    u8int base_high;        /* The last 8 bits of the base */
} __attribute__((packed)); /* Used to tell GCC not to change the alignment in the structure */

/* To tell the processor where to find our GDT, we have to
 * give it the address of this structure.
 *
 * The base is the address of the first entry in our GDT,
 * the limit is the size of the table - 1.
 */
struct gdt_ptr_struct
{
    u16int limit;   /* The upper 16 bits of all selector limits. */
    u32int base;    /* The address of the first gdt_entry_t struct */
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;
typedef struct gdt_ptr_struct gdt_ptr_t;

/* Initialisation function is publicly accessible. */
void init_descriptor_tables ();
