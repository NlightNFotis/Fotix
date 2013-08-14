/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * paging.c -- Defines functions to be used to help 
 * with virtual memory management.
 */

#include "paging.h"
#include "kheap.h"
#include "common.h"

/* The kernel's page directory */
page_directory_t *kernel_directory = 0;

/* The current page directory */
page_directory_t *current_directory = 0;

/* A bitset of frames - used or free. */
u32int *frames;
u32int nframes;

/* Defined in kheap.c */
extern u32int placement_address;

/* Macros used in the bitset algorithms. */
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

/* Static function to set a bit in the frames bitset */
static void 
set_frame (u32int frame_addr)
{
    u32int frame  = frame_addr / 0x1000;
    u32int index  = INDEX_FROM_BIT(frame); 
    u32int offset = OFFSET_FROM_BIT(frame);
    
    frames[index] |= (0x1 << offset);
}

/* Static function to clear a bit in the frames bitset */
static void
clear_frame (u32int frame_addr)
{
    u32int frame  = frame_addr / 0x1000;
    u32int index  = INDEX_FROM_BIT(frame);
    u32int offset = OFFSET_FROM_BIT(frame);

    frames[index] &= ~(0x1 << offset);
}

/* Static function to test if a bit is set */
static u32int
test_frame (u32int frame_addr)
{
    u32int frame  = frame_addr / 0x1000;
    u32int index  = INDEX_FROM_BIT(frame);
    u32int offset = OFFSET_FROM_BIT(frame);
    
    return (frames[index] & (0x1 << offset));
}

/* Static function to find the first free frame */
static u32int
first_frame ()
{
    u32int first_counter, second_counter;

    for (first_counter = 0; first_counter < INDEX_FROM_BIT(nframes); first_counter++)
        {
            /* Nothing free; exit early */
            if (frames[first_counter] != 0xFFFFFFFF)
                {
                    for (second_counter = 0; second_counter < 32; second_counter++)
                        {
                            u32int toTest = 0x1 << second_counter;
                            if ( !(frames[first_counter] & toTest) )
                                {
                                    return first_counter * 4 * 8 + second_counter;
                                }
                        }
                }
        }
}

/* Function to allocate a frame. */
void 
alloc_frame (page_t *page, int is_kernel, int is_writable)
{
    if (page->frame != 0)
        {
            return; /* frame was already allocated, return straight away. */
        }
    else
        {
            u32int index = first_frame (); /* index is now the index of the first free frame */
            if (index == (u32int) - 1)
                {
                    /* PANIC is just a macro that prints a message to the screen,
                     * and then hits an infinite loop */
                    PANIC("NO FREE FRAMES!");
                }

            set_frame (index * 0x1000); /* this frame is now ours! */
            page->present = 1; /* mark it as present */
            page->rw      = (is_writable) ? 1 : 0; 
            page->user    = (is_kernel) ? 0 : 1;
            page->frame   = index;
        }
}

/* Function to dealocate a frame */
void
free_frame (page_t *page)
{
    u32int frame;
    if (!(frame = page->frame))
        {
            return; /* The given page didn't actually have an allocated frame */
        }
    else
        {
            clear_frame (frame); /* Frame is now free again */
            page->frame = 0x0; /* Page now doesn't have a frame */
        }
}

/* Code for paging initialisation */
void 
initialise_paging ()
{
    /* The size of physical memory. For the moment we assume it is 16MB big */
    u32int mem_end_page = 0x1000000;

    nframes = mem_end_page / 0x1000;
    frames = (u32int *) kmalloc (INDEX_FROM_BIT(nframes));
    memset (frames, 0, INDEX_FROM_BIT(nframes));

    /* Let's make a page directory */
    kernel_directory = (page_directory_t *) kmalloc_a (sizeof (page_directory_t));
    current_directory = kernel_directory;

    /* We need to identity map (phys addr = virt addr) from
     * 0x0 to the end of used memory, so we can access this
     * transparently, as if paging wasn't enabled.
     * NOTE that we use a while loop here deliberately.
     * inside the loop body we actually change placement_address
     * by calling kmalloc(). A while loop causes this to be computed on
     * the fly rather than once at the start.
     */
    int counter = 0;
    while (counter < placement_address)
        {
            /* kernel code is readable but not writeable from userspace */
            alloc_frame (get_page (counter, 1, kernel_directory), 0, 0);
            counter += 0x1000;
        }

    /* Before we enable paging, we must register our page fault handler */
    register_interrupt_handler (14, page_fault);

    /* Now, enable paging! */
    switch_page_directory (kernel_directory);
}

void
switch_page_directory (page_directory_t *dir)
{
    current_directory = dir;
    asm volatile ("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    u32int cr0;
    asm volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; /* enable paging */
    asm volatile ("mov %0, %%cr0":: "r"(cr0));
}

page_t *
get_page (u32int address, int make, page_directory_t *dir)
{
    /* Turn the address into an index. */
    address /= 0x1000;

    /* Find the page table containing this address */
    u32int table_idx = address / 1024;
    if (dir->tables[table_idx]) /* If this table is already assigned */
        {
            return &dir->tables[table_idx]->pages[address % 1024];
        }
    else if (make)
        {
            u32int temp;
            dir->tables[table_idx] = (page_table_t *) kmalloc_ap (sizeof (page_table_t), &temp);
            dir->tablesPhysical[table_idx] = temp | 0x7; /* PRESENT, RW, US */
            return &dir->tables[table_idx]->pages[address % 1024];
        }
    else
        {
            return 0;
        }
}

/* The page fault handler */
void
page_fault (registers_t regs)
{
    /* A page fault has occured.
     * The page faulting address is stored in the CR2 register.
     */
    u32int faulting_address;
    asm volatile ("mov %%cr2, %0" : "=r" (faulting_address));

    /* The error code gives us details of what happened. */
    int present  = !(regs.err_code & 0x1);   /* Page not present */
    int rw       = regs.err_code & 0x2;     /* Write operation? */
    int us       = regs.err_code & 0x4;     /* Processor was in user-mode? */
    int reserved = regs.err_code & 0x8;     /* Overwritten CPU-reserved bits of page entry? */
    int id       = regs.err_code & 0x10;    /* Caused by an instruction fetch? */

    /* Output an error message. */
    monitor_write ("Page fault! ( ");
    if (present)  monitor_write ("present ");
    if (rw)       monitor_write ("read-only ");
    if (us)       monitor_write ("user-mode ");
    if (reserved) monitor_write ("reserved ");
    monitor_write (") at 0x");
    monitor_write_hex (faulting_address);
    monitor_write ("\n");
    PANIC("Page fault");
}
