/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * descriptor_tables.h -- Defines structs and functions that
 * are going to be used to implement the GDT.
 */

#include "common.h"

/* 
 * -------------------------------
 *  GLOBAL DESCRIPTOR TABLE
 * ------------------------------
 */

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

/*
 * ---------------------------------
 *  INTERRUPT DESCRIPTOR TABLE
 * ---------------------------------
 */

/* A struct defining an interrupt gate. */
struct idt_entry_struct
{
    u16int base_lo;     /* The lower 16bits of the address to jump to when this interrupt fires */
    u16int sel;         /* The kernel segment selector. */
    u8int always_zero;      /* This must always be zero. */
    u8int flags;        
    u16int base_hi;     /* The upper 16 bits of the address to jump to. */
} __attribute__((packed));

/* A struct describing a pointer to an array of interrupt handlers.
 * This is a format suitable for giving to 'lidt'.
 */
struct idt_ptr_struct
{
    u16int limit;
    u32int base;        /* The address of the first element in our idt_entry_t array */
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;
typedef struct idt_ptr_struct   idt_ptr_t;

/* These extern directives let us access the addresses of our ASM ISR handlers*/
extern void isr0 ();  /* Division by zero exception */
extern void isr1 ();  /* Debug exception */
extern void isr2 ();  /* Non maskable interrupt */
extern void isr3 ();  /* Breakpoint exception */
extern void isr4 ();  /* Into detected overflow */
extern void isr5 ();  /* Out of bounds exception */
extern void isr6 ();  /* Invalid opcode exception */
extern void isr7 ();  /* No coprocessor exception */
extern void isr8 ();  /* Double fault (pushes an error code) */
extern void isr9 ();  /* Coprocessor segment overrun */
extern void isr10 (); /* Bad TSS (pushes an error code) */
extern void isr11 (); /* Segment not present (pushes an error code) */
extern void isr12 (); /* Stack fault (pushes an error code) */
extern void isr13 (); /* General protection fault (pushes an error code) */
extern void isr14 (); /* Page fault (pushes an error code) */
extern void isr15 (); /* Unknown interrupt exception */
extern void isr16 (); /* Coprocessor fault */
extern void isr17 (); /* Alignment check exception */
extern void isr18 (); /* Machine check exception */
extern void isr19 (); /* RESERVED */
extern void isr20 (); /* ... */
extern void isr21 (); /* ... */
extern void isr22 (); /* ... */
extern void isr23 (); /* ... */
extern void isr24 (); /* ... */
extern void isr25 (); /* ... */
extern void isr26 (); /* ... */
extern void isr27 (); /* ... */
extern void isr28 (); /* ... */
extern void isr29 (); /* ... */
extern void isr30 (); /* ... */
extern void isr31 (); /* ... */

extern void irq0  ();
extern void irq1  ();
extern void irq2  ();
extern void irq3  ();
extern void irq4  ();
extern void irq5  ();
extern void irq6  ();
extern void irq7  ();
extern void irq8  ();
extern void irq9  ();
extern void irq10 ();
extern void irq11 ();
extern void irq12 ();
extern void irq13 ();
extern void irq14 ();
extern void irq15 ();
