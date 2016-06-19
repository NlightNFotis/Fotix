# The GDT and the IDT

The GDT and the IDT are descriptor tables. They are arrays of flags and bit values that describe the 
operation of either the segmentation system (in the case of the GDT) or the interrupt vector table
(in the case of the IDT).

## The Global Descriptor Table

The x86 architecture has two methods of memory protection and of providing virtual memory:
  
  * segmentation, and
  * paging.

With *segmentation*, every memory access is evaluated with respect to a segment. That is, the memory
address is added to the segment's base address, and checked against the segment's length. You can think
of a segment as a window into the address space - The process doesn't know it's a window, all it sees is
a linear address space starting at zero and going up to the segment length.

With *paging*, the address space is split into (usually 4KB, but this can change) blocks, called pages.
Each page can be mapped into physical memory, onto what is called a "frame". It can also be unmapped. This
way, you can create virtual memory spaces.

Both of these methods have their advantages, but paging is much better. Segmentation is, although still usable,
fast becoming obsolete as a method of memory protection and of virtual memory. In fact, the x86-64 architecture
requires a flat memory model (one segment with a base of `0` and a limit of `0xFFFFFFFF`) for some of its 
instructions to operate properly.

Segmentation is, however, totally built-in the x86 architecture. It's impossible to get around it. So, nowdays,
we use it to setup out Global Descriptor Table - a list of segment descriptors - in order to try and setup a 
flat memory model. The segment's window should start at `0x00000000` and extend to `0xFFFFFFFF` (the end of memory).
However, there's one thing that segmentation can do that paging can't, and that's setting the *ring level*.

### x86 Protection rings

A ring is a privilege level - `ring zero (0)` is the most privileged, and `ring three (3)` is the least. Processes
executing in `ring zero` are said to be running in `kernel-mode`, and they can use instruction like `cli` 
(clear interrupts) or `hlt` (halt the processor) that processes running in other, least privileged rings can't.

Most of the time, rings 1 and 2 are not used, but since they can access more instructions than ring 3, they may be
used for drivers, vm hypervisors, etc. Some microkernel architectures also use these for running servers.

A segment descriptor carries inside it a number representing the ring level it applies to. To change ring levels,
among other things, you need segments that represent both ring 0 and ring 3.

In the x86, we have 6 segmentation registers. Each holds an offset in the GDT. They are `cs` (code segment), 
`ds` (data segment), `es` (extra segment), `fs`, `gs`, and `ss` (stack segment). The code segment **must** refer
to a descriptor which is set as a code segment. There is a flag for this is in the access byte. The rest should all
reference a descriptor which is set as the data segment.

## The Interrupt Descriptor Table

There are times when we want to stop the processor from doing what it does right now, and make it do something else.
This is useful for example, when we want a timer to signal us (in order to run the scheduler) or when a driver responds
to an event (for example, when the keyboard driver runs). To do that, we need to set up routines to respond to `interrupts`
An interrupt is like a POSIX signal - but in the hardware level. It signals the processor that an event happened, and 
also contains some information regarding the nature of the event. 

The processor can register interrupt handlers that deal with the interrupt, respond to an interrupt with halting current
context execution and running the interrupt handler, and then resume with the previous context execution after the 
interrupt handler has done its due. Interrupts can be fired in the hardware via IRQs (interrupt requests) or in software
via the x86 `int` instruction.

The Interrupt Descriptor Table is an array that contains references to the handlers for each interrupt. There are 256 
possible interrupt codes, so we need to specify 256 interrupt handlers. If an interrupt occurs and there is no entry for
it, not even a `NULL` entry, the CPU will panic and reset.

### Faults, traps and exceptions

The processor will sometimes need to signal our kernel about an event, such as a page fault, or a divide-by-zero fault.
To do that, it uses the first 32 interrupts. The special, CPU-dedicated interrupts are shown below:

 * `0` - Divide by zero exception
 * `1` - Debug exception (Used by debuggers in breakpoints, etc)
 * `2` - Non maskable interrupt (occurs for RAM errors and unrecoverable hardware problems)
 * `3` - Breakpoint exception
 * `4` - INT detected overflow
 * `5` - Out of bounds exception
 * `6` - Invalid opcode exception
 * `7` - No coprocessor exception
 * `8` - Double fault (*pushes an error code*)
 * `9` - Coprocessor segment overrun
 * `10` - Bad TSS (Task State Segment - *pushes an error code*)
 * `11` - Segment not present (*pushes an error code*)
 * `12` - Stack fault (*pushes an error code*)
 * `13` - General protection fault (*pushes an error code*)
 * `14` - Page fault (*pushes an error code*)
 * `15` - Unknown interrupt exception
 * `16` - Coprocessor fault
 * `17` - Alignment check exception
 * `18` - Machine check exception
 * `19` -- `31` - Reserved