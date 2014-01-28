/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * elf.h -- Defines the format of an elf file
 * (Executable and Linking Format)
 */


/* HUGE TODO: I need to implement the whole of the elf definition, 
 * and also define its types and not just the parts that I need 
 * immediately */


#ifndef _ELF_H
#define _ELF_H

/* Following information according to ELF_FORMAT
 * reference available at 
 * http://www.skyfree.org/linux/references/ELF_Format.pdf
 */

#include "common.h"

/* The first 16 bytes of the file, serving to identify it. */
#define SIZE_ELF_IDENT 16 

/* The "magic number" identifying the file as an ELF object file.
 * It's the 4 first entries in elf_ident
 */
#define ELF_MAGIC 0x464C457FU   /* "0x7ELF" in little endian */

/** ELF file header **/
typedef struct
{
    /* Initial bytes of the file mark it as an object file. */
    u8int  elf_ident[SIZE_ELF_IDENT];

    /* This member identifies the object file type */
    u16int elf_type;

    /* This specifies the required machine architecture for the file */
    u16int elf_machine;

    /* This identifies the object file version  (0 - 1, invalid - current) */
    u32int elf_version;

    /* The virtual address to which the system first transfer control */
    u32int elf_entry;

    /* The program header table's file offset in bytes */
    u32int elf_phoff;

    /* The section header table's file offset in bytes */
    u32int elf_shoff;

    /* Processor specific flags associated with the file */
    u32int elf_flags;

    /* ELF Header's size in bytes */
    u16int elf_ehsize;

    /* This holds the size (in bytes) of one entry in the file's program 
     * header table; all the entries are the same size */
    u16int elf_phentsize;

    /* The total number of entries in the program header table */
    u16int elf_phnum;

    /* This holds the size (in bytes) of a section header */
    u16int elf_shentsize;

    /* This holds the total number of entries in the section header
     * table */
    u16int elf_shnum;

    /* This holds the section header table index of the entry associated
     * with the section name string table.
     */
    u16int elf_shstrndx;
} elfhdr_t;

/** Program Section **/
typedef struct 
{
    /* This tells what kind of segment this array element describes */
    u32int p_type;
    
    /* This gives us the offset from the beginning of the file at which
     * the first byte of the segment resides */
    u32int p_offset;

    /* The virtual address at which the first byte of the segment
     * resides in memory */
    u32int p_vaddr;

    /* This is reserved for the segment's physical address */
    u32int p_paddr;

    /* This gives us the number of bytes in the file image
     * of the segment. */
    u32int p_filesz;

    /* This gives us the number of bytes in the memory image of the segment */
    u32int p_memsz;
    
    /* This gives us flags relevant to the segment */
    u32int p_flags;

    /* This gives us the value to which the segments are aligned in memory and in file */
    u32int p_align;
} proghdr_t;

/* The value of the type in proghdr_t that signs it as
 * a loadable segment*/
#define PROG_LOAD 1

/* Flag bits for Proghdf flags */
#define PROG_FLAG_EXEC  1
#define PROG_FLAG_WRITE 2
#define PROG_FLAG_READ  4

#endif
