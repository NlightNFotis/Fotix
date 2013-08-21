/* This file is part of fotix.
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * keyboard.h -- Defines the interface for the 
 * keyboard driver.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"
#include "descriptor_tables.h"
#include "isr.h"

#define CONTROL    0x1
#define ALT	    0x2
#define ALTGR      0x4
#define LSHIFT     0x8
#define RSHIFT     0x10
#define CAPSLOCK   0x20
#define SCROLLLOCK 0x40
#define NUMLOCK	0x80

#define RELEASED_MASK 0x80

/*
 * 8 Bits for control key statuses (1 = set, 0 = not set)
 * 
 * 0: control
 * 1: alt
 * 2: alt-gr
 * 3: left shift
 * 4: right shift
 * 5: caps_lock
 * 6: scroll_lock
 * 7: num_lock
 */
typedef u8int key_status_t;

typedef struct keymap
{
	/* All the chars mapped to their respective scancodes */
	u8int scancodes[128];
	u8int shift_scancodes[128];

	/* The function keys mapped to the bit position in the key status map. */
	u8int control_map[8];

	/* The statuses of the control keys, initialised to 0 */
	key_status_t controls;
} keymap_t;

void init_keyboard_driver ();
void switch_layout (keymap_t *layout);
void keyboard_handler (registers_t *regs);

/* Returns a character from the keyboard; does not block
 * Returns '\0' if no character is available
 */
char keyboard_getchar ();

#endif
