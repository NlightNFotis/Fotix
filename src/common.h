/* This file is a part of fotix.
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * common.h -- Defines typedefs and some global functions.
 */

#ifndef COMMON_H
#define COMMON_H

/* Some nice typedefs, to standardise sizes across platforms.
 * These typedefs are written for 32bit x86. */
typedef unsigned int    u32int;
typedef          int    s32int;
typedef unsigned short  u16int;
typedef          short  s16int;
typedef unsigned char   u8int;
typedef          char   s8int;

void    outb (u16int port, u8int value);
u8int   inb (u16int port);
u16int  inw (u16int port);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))
#define ERROR(msg) error(msg)

extern void panic (const char *message, const char *file, u32int line);
extern void panic_assert(const char *file, u32int line, const char *desc);
void error(const char *message);

#endif
