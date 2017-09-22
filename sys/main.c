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
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"
#include "keyboard.h"

extern u32int placement_address;
u32int initial_esp;
u8int  failure;

/*
 * The kernel's entry point.
 *
 * The C entry point of the kernel; called by the assemly loader.
 *
 * The kernel boot process does the following:
 *  - Initialise descriptor tables (GDT, IDT)
 *  - Initialise the PIT
 *  - Initialise Paging
 *  - Initialise the tasking system (scheduler)
 *  [Further booting]
 */
u32int
kernel_start (struct multiboot *mboot_ptr, /* Initial multiboot information, passed by the bootloader */
              u32int initial_stack) /* The initial stack pointer */
{
    initial_esp = initial_stack;
    
    monitor_clear ();

    monitor_write ("Fotix v0.1\n");
    monitor_write ("Copyright (c) 2016 Fotis Koutoulakis\n");
    monitor_write ("http://www.fotiskoutoulakis.com\n");
    monitor_put ('\n');

    /* Initialise all the ISRs and segmentation */
    failure = init_descriptor_tables ();
    if (!failure)
      {
        monitor_write_padded ("Initializing descriptor tables", 1);
      }
    else
      {
        monitor_write_padded ("Initializing descriptor tables", 0);
      }

    /* Initialise the PIT to 100Hz */
    asm volatile ("sti");
    failure = init_timer (50);
    if (!failure)
      {
        monitor_write_padded ("Initializing timer", 1);
      }
    else
      {
        monitor_write_padded ("Initializing timer", 0);
      }

    /* Find the location of our initial ramdisk. */
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int *) mboot_ptr->mods_addr);
    u32int initrd_end      = *(u32int *) (mboot_ptr->mods_addr + 4);
    monitor_write_padded ("Looking for initial ramdisk", 1);

    /* Don't trample our module with placement accesses, please! */
    placement_address = initrd_end;

    /*  Initialise paging. */
    failure = initialise_paging ();
    if (!failure)
      {
        monitor_write_padded ("Initializing paging", 1); 
      }
    else
      {
        monitor_write_padded ("Initializing paging", 0);
      }

    /* Initialise multitasking */
    failure = initialise_tasking ();
    if (!failure)
      {
        monitor_write_padded ("Initializing scheduling", 1);
      }
    else
      {
        monitor_write_padded ("Initializing scheduling", 0);
      }

    /* Initialise the initial ramdisk, and set it as the filesystem root. */
    fs_root = initialise_initrd (initrd_location);
    if (!fs_root)
      {
        failure = 1;
      }
    else
      {
        failure = 0;
      }

    if (!failure)
      {
        monitor_write_padded ("Initializing ramdisk", 1);
      }
    else
      {
        monitor_write_padded ("Initializing ramdisk", 0);
      }

    failure = initialise_syscalls();
    if (!failure)
      {
        monitor_write_padded ("Initializing kernel system call interface", 1);
      }
    else
      {
        monitor_write_padded ("Initializing kernel system call interface", 0);
      }

    init_keyboard_driver ();

    switch_to_user_mode();

    syscall_monitor_write("Hello, user world!\n");

    for (;;)
       {
           char c = keyboard_getchar ();
           if (c)
               monitor_put (c);
       }

    return 0xdeadbeef;
}
