/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * kheap.c -- This file includes functions useful
 * to allocate memory in the kernel.
 */

#include "kheap.h"

/* end is defined in the linker script */
extern u32int end;
u32int placement_address = (u32int) &end;

u32int
kmalloc_int (u32int size, int align, u32int *phys)
{
    /* If the address is not already page-aligned, align it */
    if (align == 1 && (placement_address & 0xFFFFF000))
        {
            placement_address &= 0xFFFFF000;
            placement_address += 0x1000;
        }

    if (phys)
        {
            *phys = placement_address;
        }

    u32int temp = placement_address;
    placement_address += size;
    return temp;
}

/* WRAPPER FUNCTIONS FOR kmalloc_int */

/* Page aligned */
u32int
kmalloc_a (u32int size)
{
    return kmalloc_int (size, 1, 0);
}

/* Returns a physical address */
u32int 
kmalloc_p (u32int size, u32int *phys)
{
    return kmalloc_int (size, 0, phys);
}

/* Page aligned *AND* returns a physical address */
u32int
kmalloc_ap (u32int size, u32int *phys)
{
    return kmalloc_int (size, 1, phys);
}

/* vanilla */
u32int
kmalloc (u32int size)
{
    return kmalloc_int (size, 0, 0);
}
