/* This file is a part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * monitor.c -- Defines functions that are going to be 
 * used to control the screen.
 */

#include "include/monitor.h"
#include "include/common.h"

/* The VGA framebuffer starts at 0xB8000 */
u16int *video_memory = (u16int *) 0xB8000;

/* Store the cursor position */
u8int cursor_x = 0;
u8int cursor_y = 0;


/* Update the hardware cursor. */
static void
move_cursor ()
{
    /* The screen is 80 characters wide... */
    u16int cursorLocation = cursor_y * 80 + cursor_x;
    outb (0x3D4, 14);                   /* Tell the VGA we are setting the high cursor byte. */
    outb (0x3D5, cursorLocation >> 8);  /* Send the high cursor byte. */
    outb (0x3D4, 15);                   /* Tell the VGA board we are setting the low cursor byte */
    outb (0x3D5, cursorLocation);       /* Send the low cursor byte. */
}

/* Scroll the text on the screen up by one line */
static void
scroll ()
{
    /* Get a space character with the default colour attributes. */
    u8int  attributeByte = (0 /*black*/ << 4) | (15 /*white*/ &0x0F);
    u16int blank         = 0x20 /*space*/ | (attributeByte << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if (cursor_y >= 25)
        {
            /* Move the current text chunk that makes up the screen
             * back in the buffer by a line */
            int counter;
            for (counter = 0; counter < 24 * 80; counter++)
              {
                video_memory[counter] = video_memory[counter + 80];
              }

            /* The last line should now be a blank. Do this by writing
             * 80 spaces to it. */
            for (counter = 24 * 80; counter < 25 * 80; counter++)
              {
                video_memory[counter] = blank;
              }

            /* The cursor should now be on the last line. */
            cursor_y = 24;
        }
}

/* Writes a single character to the screen */
void
monitor_put (char c)
{
    u8int backColour = 0;   /* Background colour is black (0) */
    u8int foreColour = 15;  /* Foreground colour is white (15) */

    /* The attribute byte is made up of two nibbles - the lower being the
     * foreground colour, and the upper the background colour. */
    u8int attributeByte = (backColour << 4) | (foreColour &0x0F);

    /* The attribute byte is the top 8 bits of the word we have to send to 
     * the VGA board. */
    u16int attribute = attributeByte << 8;
    u16int *location;

    /* Handle a backspace, by moving the cursor back one space */
    if (c == 0x08 /*backspace*/ && cursor_x)
      {
        cursor_x--;
      }

    /* Handle a tab, by increasing the cursor's X, but only to a point
     * where it is divisible by 8 */
    else if (c == 0x09 /*tab*/)
      {
        cursor_x = (cursor_x + 8) & ~(8 - 1);
      }

    /* Handle carriage return */
    else if (c == '\r')
      {
        cursor_x = 0;
      }

    /* Handle a newline by moving cursor back to left and increasing row */
    else if (c == '\n')
      {
        cursor_x = 0;
        cursor_y++;
      }

    /* Handle any other printable character. */
    else if (c >= ' ')
      {
        location  = video_memory + (cursor_y * 80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
      }

    /* Check if we need to insert a new line because we have reached the end of 
     * the screen. */
    if (cursor_x >= 80)
      {
        cursor_x = 0;
        cursor_y++;
      }

    /* Scroll the screen if needed. */
    scroll ();

    /* Move the hardware cursor */
    move_cursor ();
}

/* Clear the screen, by copying lots of spaces to the framebuffer. */
void
monitor_clear ()
{
    /* Make an attribute byte for the default colours */
    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ &0x0F);
    u16int blank        = 0x20 /*space*/ | (attributeByte << 8);

    int counter;
    for (counter = 0; counter < 80 * 25; counter++)
      {
        video_memory[counter] = blank;
      }

    /* Move the hardware cursor back to the start. */
    cursor_x = 0;
    cursor_y = 0;
    move_cursor ();
}

/* Output a null-terminated ASCII string to the screen. */
void
monitor_write (char *c)
{
    int counter = 0;
    while (c[counter])
      {
        monitor_put (c[counter++]);
      }
}

void 
monitor_write_dec (u32int number)
{

    if (number == 0)
      {
        monitor_put ('0');
        return;
      }

    s32int acc = number;
    char c[32];
    int i = 0;
    while (acc > 0)
      {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
      }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
      {
        c2[i--] = c[j++];
      }
    monitor_write (c2);
}

void
monitor_write_hex (u32int number)
{
    s32int temp;

    monitor_write ("0x");

    u8int noZeroes = 1;

    int counter;
    for (counter = 28; counter > 0; counter -= 4)
      {
        temp = (number >> counter) & 0xF;
        if (temp == 0 && noZeroes != 0)
          {
            continue;
          }

        if (temp >= 0xA)
          {
            noZeroes = 0;
            monitor_put (temp - 0xA + 'a');
          }
        else
          {
            noZeroes = 0;
            monitor_put (temp + '0');
          }
      }

    temp = number & 0xF;
    if (temp >= 0xA)
      {
        monitor_put (temp - 0xA + 'a');
      }
    else
      {
        monitor_put (temp + '0');
      }
}

void
monitor_write_padded(char *message, u8int success)
{
    char buff[80];
    u16int index;
    // Get the length of the message
    u16int strln = 0;
    while (*message)
      {
        *(buff+strln) = *(message++);
        strln++;
      }

    for (index = strln; index < 75; index++)
      {
        *(buff + index) = '.';
      }

    if (success)  // if the previous operation was successfull
      {
        buff[75] = '[';
        buff[76] = 'O';
        buff[77] = 'K';
        buff[78] = ']';
        buff[79] = '\n';
      }
    else
      {
        buff[75] = '[';
        buff[76] = 'F';
        buff[77] = 'L';
        buff[78] = ']';
        buff[79] = '\n';
      }

    monitor_write (buff);
}
