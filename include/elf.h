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

/* Common elf types */
typedef u16int Elf32_Half;
typedef u32int Elf32_Off;
typedef u32int Elf32_Addr;
typedef u32int Elf32_Word;
typedef s32int Elf32_Sword;


/* The first 16 bytes of the file, serving to identify it. */
#define SIZE_ELF_IDENT 16

/* ELF file magic */
#define ELFMAG0     0x7
#define ELFMAG1     'E'
#define ELFMAG2     'L'
#define ELFMAG3     'F'

/* The following are used in elf.c for elf file
 * validation */
#define ELFDATA2LSB (1) /* Little endian */
#define ELFCLASS32  (1) /* 32-bit arch */
#define EM_386      (3) /* x86 machine type */
#define EV_CURRENT  (1) /* ELF Current Version */

#define ELF_RELOC_ERR -1

/** ELF file header **/
typedef struct {
    /* Initial bytes of the file mark it as an object file. */
    u8int elf_ident[SIZE_ELF_IDENT];

    /* This member identifies the object file type */
    Elf32_Half elf_type;

    /* This specifies the required machine architecture for the file */
    Elf32_Half elf_machine;

    /* This identifies the object file version  (0 - 1, invalid - current) */
    Elf32_Word elf_version;

    /* The virtual address to which the system first transfer control */
    Elf32_Addr elf_entry;

    /* The program header table's file offset in bytes */
    Elf32_Off elf_phoff;

    /* The section header table's file offset in bytes */
    Elf32_Off elf_shoff;

    /* Processor specific flags associated with the file */
    Elf32_Word elf_flags;

    /* ELF Header's size in bytes */
    Elf32_Half elf_ehsize;

    /* This holds the size (in bytes) of one entry in the file's program 
     * header table; all the entries are the same size */
    Elf32_Half elf_phentsize;

    /* The total number of entries in the program header table */
    Elf32_Half elf_phnum;

    /* This holds the size (in bytes) of a section header */
    Elf32_Half elf_shentsize;

    /* This holds the total number of entries in the section header
     * table */
    Elf32_Half elf_shnum;

    /* This holds the section header table index of the entry associated
     * with the section name string table.
     */
    Elf32_Half elf_shstrndx;
} Elf32_elfhdr;

enum elf_identification {
    EI_MAG0,       /* 0x7F */
    EI_MAG1,       /* 'E'  */
    EI_MAG2,       /* 'L'  */
    EI_MAG3,       /* 'F'  */
    EI_CLASS,      /* Architecture (32/64) */
    EI_DATA,       /* Byte order */
    EI_VERSION,    /* ELF version */
    EI_OSABI,      /* OS specific */
    EI_ABIVERSION, /* OS specific */
    EI_PAD         /* Padding */
};

enum elf_file_type {
    ET_NONE,    /* Unknown type */
    ET_REL,     /* Relocatable file */
    ET_EXEC     /* Executable file */
};

/** Program header **/
typedef struct {
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
} Elf32_proghdr;

/** Section header **/
typedef struct {
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} Elf32_shdr;

#define SHN_UNDEF   (0x00) /* Undefined/ Not present */

enum sht_types {
    SHT_NULL = 0,    /* Null section */
    SHT_PROGBITS = 1,    /* Program information */
    SHT_SYMTAB = 2,    /* Symbol table */
    SHT_STRTAB = 3,    /* String table */
    SHT_RELA = 4,    /* Relocation (w/ addend) */
    SHT_NOBITS = 8,    /* Not present in the file */
    SHT_REL = 9     /* Relocation (no addend) */
};

enum sht_attributes {
    SHF_WRITE = 0x01, /* Writeable section */
    SHF_ALLOC = 0x02  /* Exists in memory */
};

typedef struct {
    Elf32_Word st_name;
    Elf32_Addr st_value;
    Elf32_Word st_size;
    u8int st_info;
    u8int st_other;
    Elf32_Half st_shndx;
} Elf32_sym;

#define ELF32_ST_BIND(INFO) ((INFO >> 4))
#define ELF32_ST_TYPE(INFO) ((INFO) & 0x0F)

enum StT_Bindings {
    STB_LOCAL,  /* Local Scope */
    STB_GLOBAL, /* Global Scope */
    STB_WEAK    /* Weak (ie. __attribute__ ((weak))) */
};

enum StT_Types {
    STT_NOTYPE,  /* No Type */
    STT_OBJECT,  /* Variables, arrays, etc. */
    STT_FUNC     /* Methods or functions */
};

/* Relocation sections */
typedef struct {
    Elf32_Addr r_offset;
    Elf32_Word r_info;
    Elf32_Sword r_addend;
} Elf32_Rel;

#define ELF32_R_SYM(INFO)  ((INFO) >> 8)
#define ELF32_R_TYPE(INFO) ((u8int) (INFO))

enum RtT_Types {
    R_386_NONE, /* No relocation */
    R_386_32,   /* Symbol + Offset */
    R_386_PC32  /* Symbol + Offset - Section Offset */
};

/* The value of the type in proghdr_t that signs it as
 * a loadable segment*/
#define PROG_LOAD 1

/* Flag bits for Proghdf flags */
#define PROG_FLAG_EXEC  1
#define PROG_FLAG_WRITE 2
#define PROG_FLAG_READ  4

#define DO_386_32(S, A)      ((S) + (A))
#define DO_386_PC32(S, A, P) ((S) + (A) - (P))

#endif
