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

extern u32int placement_address;
u32int initial_esp;

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
int
main (struct multiboot *mboot_ptr, /* Initial multiboot information, passed by the bootloader */
        u32int initial_stack) /* The initial stack pointer */
{
    initial_esp = initial_stack;

    /* Initialise all the ISRs and segmentation */
    init_descriptor_tables ();
    /* Initialise the screen (by clearing it) */
    monitor_clear ();

    /* Initialise the PIT to 100Hz */
    asm volatile ("sti");
    init_timer (50);

    /* Print a hello world like string to the screen. */
    monitor_write ("Hello, from Fotix.\n");
    monitor_write ("Copyright (c) 2013 Fotis Koutoulakis\n");
    monitor_write ("http://www.fotiskoutoulakis.com\n");

    /* Find the location of our initial ramdisk. */
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int *) mboot_ptr->mods_addr);
    u32int initrd_end      = *(u32int *) (mboot_ptr->mods_addr + 4);

    /* Don't trample our module with placement accesses, please! */
    placement_address = initrd_end;

    /* Initialise paging. */
    initialise_paging ();

    /* Initialise multitasking */
    initialise_tasking ();

    /* Initialise the initial ramdisk, and set it as the filesystem root. */
    fs_root = initialise_initrd (initrd_location);
    
    /* Create a new process in a new address space which is a clone of this */
    int ret = fork ();

    monitor_write ("fork() returned ");
    monitor_write_hex (ret);
    monitor_write (", and getpid() returned ");
    monitor_write_hex (getpid ());
    monitor_write("\n=====================================================\n");

    /* The next section of code is not reentrant, so make sure we are not interrupted
     * during it. */
    asm volatile ("cli");

    /* Test the ramdisk */
    int counter;
    struct dirent *node = 0;
    while ((node = readdir_fs (fs_root, counter)) != 0)
      {
        monitor_write ("Found file ");
        monitor_write (node->name);
        fs_node_t *fsnode = finddir_fs (fs_root, node->name);

        if ((fsnode->flags & 0x7) == FS_DIRECTORY)
            monitor_write ("\n\t(directory)\n");
        else
          {
             monitor_write ("\n\t contents: \"");
             char buf[256];
             u32int sz = read_fs (fsnode, 0, 256, buf);
             int second_counter;
             for (second_counter = 0; second_counter < sz; second_counter++)
                monitor_put (buf[second_counter]);

             monitor_write("\"\n");
           }

        counter++;
      }

    monitor_write ("\n");

    asm volatile ("sti");

    return 0;
}
