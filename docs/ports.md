# I/O Ports

When we are talking about an **I/O Port**, we usually refer to a specific address on the 
x86's I/O bus. This bus provides communication with devices in a fixed order and size, and was
used as an alternative to memory access. On many other archictures, there is no predefined bus
for such communication and all communication with hardware is done via memory-mapped I/O. This
is also increasingly what happens on modern x86 hardware.

# `OUT` and `IN`

The x86 `out` command is being used to output to a port. `out` transfers a data byte or word 
from the register (`AL`, `AX`, `EAX`, or `RAX`) given as the second operand to the output
port numbered by the first operand. 

The x86 `in` command is being used to receive input from a port. `in` transfers a data byte or
word from the port numbered by the second operand into the register (`AL`, `AX`, `EAX` or `RAX`)
specified by the first operand.