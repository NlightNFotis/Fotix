/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * common.c - Defines some globally used
 * functions.
 */

#include "include/common.h"

/* This family of functions (outb, inb, inw etc) have to do 
 * with low level port input and output. The out* functions
 * do port output, in* do port input, b-suffix denotes
 * byte width, and w-suffix denotes word width.
 */

/* Write a byte out to the specified port. */
void 
outb (u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int 
inb (u16int port)
{
    u8int ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int 
inw (u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

/* Copy len bytes from src to dest. */
void 
memcpy (u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *) src;
    u8int *dp = (u8int *) dest;
    for ( ; len != 0; len--) *dp++ = *sp++;
}

/* Write len copies of val into dest. */
void 
memset (u8int *dest, u8int val, u32int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

/* Compare two strings. Should return -1 if 
   str1 < str2, 0 if they are equal or 1 otherwise. */
int 
strcmp (char *str1, char *str2)
{
      int i      = 0,
          failed = 0;

      while(str1[i] != '\0' && str2[i] != '\0')
        {
          if(str1[i] != str2[i])
            {
              failed = 1;
              break;
            }
          i++;
        }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

/* Copy the NULL-terminated string src into dest, and
   return dest. */
char *
strcpy (char *dest, const char *src)
{
    do
        {
            *dest++ = *src++;
        }
    while (*src != 0);
}

/* Concatenate the NULL-terminated string src onto
   the end of dest, and return dest. */
char *
strcat (char *dest, const char *src)
{
    while (*dest != 0)
        {
            *dest = *dest++;
        }

    do
        {
            *dest++ = *src++;
        }
    while (*src != 0);
    
    return dest;
}

/* Return the length of a string */
int 
strlen (char *src)
{
    int i = 0;
    while (*src++)
        i++;
    return i;
}

/* XXX: I don't like the two below. Maybe move them to a different
 * file and create a panic specific mechanism that reacts better, and
 * has the ability to be given custom messages to show to the screen
 * (too much code duplication below).
 */

/* We encountered a situation where we have to result to a kernel panic. */
extern void 
panic (const char *message, const char *file, u32int line)
{
    asm volatile ("cli"); /* Disable interrupts. */

    monitor_write ("PANIC(");
    monitor_write (message);
    monitor_write (") at ");
    monitor_write (file);
    monitor_write (":");
    monitor_write_dec (line);
    monitor_write ("\n");

    /* Halting the computer as a reaction to the kernel panic,
     * seems to be a good idea for the time being. At least
     * it's better than putting the computer in an infinite loop.
     */
    monitor_write ("Computer halting now!\n");
    asm volatile ("hlt");
}

/* An assertion failed so we have to result to a kernel panic. */
extern void 
panic_assert (const char *file, u32int line, const char *desc)
{
    asm volatile("cli"); /* Disable interrupts. */

    monitor_write ("ASSERTION-FAILED(");
    monitor_write (desc);
    monitor_write (") at ");
    monitor_write (file);
    monitor_write (":");
    monitor_write_dec (line);
    monitor_write ("\n");

    /* Again, halting the computer seems like a good solution for now */
    monitor_write ("Computer halting now!\n");
    asm volatile ("hlt");
}

/* TODO: maybe define an importance level below, and
 * log the level of fault each time? For instance
 * [!!] ERROR: foo bared
 * NOTE: foo bared
 * [!!] FATAL: foo bared
 * etc.
 */
void
error (const char *message)
{
    monitor_write ("[!!] ERROR: ");
    monitor_write (message);
}
