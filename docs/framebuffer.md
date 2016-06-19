# The framebuffer

The kernel gets booted by GRUB in text mode. That means that a framebuffer has been made 
available to it that controls a screen of characters (NOT pixels) which is 80 characters
wide by 25 characters high. More control for the screen is being given in VESA mode.

The area of memory known as the framebuffer is accessible just like normal RAM, at address
`0xB8000`. This isn't actually part of the RAM of your machine however, rather, it's part
of the VGA's dedicated video memory that has been memory mapped via hardware into the kernel's
linear address space.

The framebuffer itself is just an array of 16-bit words, each 16-bit value representing one 
character. The offset from the start of the framebuffer of the word that specifies a character
at position x, y is given from the following equation:

```
(y * 80 + x) * 2
```

It's important to note that the `* 2` is there because each character is 2 bytes (16 bits) long.
If you're indexing an array of 16-bit values, the index should be found by `y * 80 + x`.

### Word format

In ASCII (unicode is not supported in text mode), 8 bits are used to represent a character. That
gives us 8 more bits which are unused. The VGA uses these to designate foreground and background
colours (they are 4 bits each). This allows us to display a 16 possible colours:

 * black
 * blue
 * green
 * cyan
 * red
 * magenta
 * brown
 * light grey
 * dark grey
 * light blue
 * light green
 * light cyan
 * light red
 * light magenta
 * light brown
 * white

## Controller ports

The VGA controller also supports some ports on the main I/O bus, which can be used to send it specific
instructions. For example, it has a control register at `0x3D4` and a data register at `0x3D5`, which are
being used to update the cursor position.