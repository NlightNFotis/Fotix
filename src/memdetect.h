/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 */

#include "common.h"

typedef struct SMAP_entry 
{
    u32int BaseL;   /* Base address QWORD */
    u32int BaseH;
    u32int LengthL; /* Length QWORD */
    u32int LengthH;
    u16int Type;    /* Entry Type */
    u16int ACPI;    /* Extended */
} __attribute__((packed)) SMAP_entry_t;

int __attribute__((noinline)) __attribute__((regparm(3))) detectMemory(SMAP_entry_t *buffer, int maxentries);
