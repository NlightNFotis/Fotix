; This file is part of fotix
; Copyright (c) 2013 Fotis Koutoulakis
;

[GLOBAL copy_page_physical]
[GLOBAL read_eip]

copy_page_physical:
   push ebx             ; According to __cdecl, we must preserve the contents of EBX.
   pushf               ; push EFLAGS, so we can pop it, and reenable interrupts.
                        ; later, if they were enabled anyway.
   cli                  ; Disable interrupts, so that we aren't interrupted. 
                        ; Load these in BEFORE we disable paging!
   mov ebx, [esp+12]    ; Source address
   mov ecx, [esp+16]    ; Desitantion address
   
   mov edx, cr0         ; Get the control register...
   and edx, 0x7fffffff  ; and...
   mov cr0, edx         ; Disable paging.

   mov edx, 1024        ; 1024 * 4bytes = 4096 bytes to copy

.loop:
   mov eax, [ebx]       ; Get the word at the source address
   mov [ecx], eax       ; Store it at the dest address
   add ebx, 4           ; Source address += sizeof (word)
   add ecx, 4           ; Dest address  += sizeof (word)
   dec edx              ; One less word to do
   jnz .loop

   mov edx, cr0         ; Get the control register again...
   or  edx, 0x80000000  ; and...
   mov cr0, edx         ; Enable paging.

   popf                 ; Pop EFLAGS back.
   pop ebx              ; Get the original value of EBX back.
   ret

read_eip:
   ; This is a rather clever way of reading the current instruction pointer.
   ; When read_eip is called, the current instruction location is pushed
   ; on the stack. Normally we use "ret" to return from a function.
   ; This instruction pops the value from the stack and jumps to it.
   ; Here however, we pop the value ourselves into EAX (EAX is the 
   ; 'return value' register for the __cdecl calling convention), then jump to it.
   pop eax
   jmp eax
