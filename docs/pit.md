# The PIT

The **programmable interval timer** (or PIT) is a chip connected to IRQ0. It can interrupt
the CPU at a user-defined rate (between 18.2Hz and 1.1931 MHz). The PIT is the primary method
for implementing a system clock and the only method available for implementing multitasking.

The PIT has an interval clock which oscillates at approximately 1.1931MHz. This clock signal
is fed through a frequency divider, to modulate the final output frequency. It has 3 channels,
each with its own frequency divider:

  * Channel 0 is the most useful. It's output is connected to IRQ0.
  * Channel 1 is very un-useful and on modern hardware is no longer implemented. It used to
    control refresh rates for DRAM.
  * Channel 2 controls the PC speaker.
  
Channel 0 is the only one of use to us at the moment.

Ok, so we want to set the PIT up so it interrupts us at regular intervals, at frequency f.
I generally set f to be about 100Hz (once every 10 milliseconds), but feel free to set it
to whatever you like. To do this, we send the PIT a 'divisor'. This is the number that it
should divide its input frequency by. It's dead easy to work out:

`divisor = 1193180 Hz / frequency (in Hz)`

Also worthy of not is that the PIT has 4 registers in I/O space - `0x40-0x42` are the data
ports for channels 0-2 respectively, and `0x43` is the command port.
