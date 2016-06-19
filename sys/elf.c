/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * elf.c -- Defines the functions needed
 * to support elf file loading in the kernel.
 */

#include "include/common.h"
#include "include/elf.h"

/** Functions to access the section headers **/
static inline Elf32_shdr *
elf_sheader (Elf32_elfhdr *header)
{
    return (Elf32_shdr *) ((int) header + header->elf_shoff);
}

static inline Elf32_shdr *
elf_section (Elf32_elfhdr *header, int index)
{
    return &elf_sheader(hdr)[index];
}

/** Functions to access the section names **/
static inline char *
elf_str_table (Elf32_elfhdr *header)
{
    if (header->elf_shstrndx == SHN_UNDEF) return NULL;
    return (char *) header + elf_section(header, 
                        header->elf_shstrndx)->sh_offset;
}

static inline char *
elf_lookup_string (Elf32_elfhdr *header, int offset)
{
    char *strtab = elf_str_table (hdr);
    if (strtab == NULL) return NULL;
    return strtab + offset;
}

/** Functions to access the value of a symbol **/
static int
elf_get_symval (Elf32_elfhdr *header, int table, u32int index)
{
    if (table == SHN_UNDEF || index == SHN_UNDEF) return 0;
    Elf32_shdr *symtab = elf_section (header, table);

    if (index >= symtab->sh_size) {
        ERROR ("Symbol index out of Range (%d:%u).\n", table, index);
        return ELF_RELOC_ERR;
    }

    int symaddr = (int) header + symtab->sh_offset;
    Elf32_Sym *symbol = &((Elf32_Sym *) symaddr)[idx];

    if (symbol->st_shndx == SHN_UNDEF) {
        /* External symbol, lookup value */
        Elf32_shdr *strtab = elf_section (header, symtab->sh_link);
        const char *name = (const char *) header + strtab->sh_offset +
                           symbol->st_name;

        extern void *elf_lookup_symbol (const char *name);
        void *targe = elf_lookup_symbol (name);

        if (target == NULL) {
            /* Extern symbol not found */
            if (Elf32_ST_BIND (symbol->st_info) & STB_WEAK) {
                /* Weak symbol initialised as 0 */
                return 0;
            } else {
                ERROR ("Undefined External Symbol : %s.\n", name);
                return ELF_RELOC_ERR;
            }
        } else {
            return (int) target;
        } 
    } else if (symbol->st_shndx == SHN_ABS) {
            /* Absolute value */
            return symbol->st_value;
    } else {
            /* Internally defined symbol */
            Elf32_shdr *target = elf_section (hdr, symbol->st_shndx);
            return (int) header + symbol->st_value + target->sh_offset;
    }
}

        

bool
elf_check_valid (Elf32_elfhdr *header)
{
    if (!header) return false;

    if (headerr->elf_ident[EI_MAG0] != ELFMAG0) {
        ERROR ("ELF Header EI_MAG0 incorrect.\n");
        return false;
    }

    if (header->elf_ident[EI_MAG1] != ELFMAG1) {
        ERROR ("ELF Header EI_MAG1 incorrect.\n");
        return false;
    }

    if (header->elf_ident[EI_MAG2] != ELFMAG2) {
        ERROR ("ELF Header EI_MAG2 incorrect.\n");
        return false;
    }

    if (header->elf_ident[EI_MAG3] != ELFMAG3) {
        ERROR ("ELF Header EI_MAG3 incorrect.\n");
        return false;
    }

    return true;
}
 
bool
elf_check_supported (Elf32_elfhdr *header)
{
    if (!validate_elf (header)) {
        ERROR ("Invalid ELF File.\n");
        return false;
    }

    if (header->elf_ident[EI_CLASS] != ELFCLASS32) {
        ERROR ("Unsupported ELF File Class\n");
        return false;
    }

    if (header->elf_ident[EI_DATA] != ELFDATA2LSB) {
        ERROR ("Unsupported ELF File Byte order\n");
        return false;
    }

    if (header->elf_machine != EM_386) {
        ERROR ("Unsupported ELF File target.\n");
        return false;
    }

    if (header->elf_ident[EI_VERSION] != EV_CURRENT) {
        ERROR ("Unsupported ELF File version.\n");
        return false;
    }

    if (header->elf_type != ET_REL && header->elf_type != ET_EXEC) {
        ERROR ("Unsupported ELF File type.\n");
        return false;
    }

    return true;
}

static void *
elf_load_rel (Elf32_elfhdr *header)
{
    u32int result;
    result = elf_load_stage1 (header);
    if (result == ELF_RELOC_ERR) {
        ERROR ("Unable to load ELF file.\n");
        return NULL;
    }

    result = elf_load_stage2 (header);
    if (result == ELF_RELOC_ERR) {
        ERROR ("Unable to load ELF file.\n");
        return NULL;
    }

    /* TODO: parse the program header. */
    return (void *) header->elf_entry;
}

void *
elf_load_file (void *file)
{
    Elf32_elfhdr *header = (Elf32_elfhdr *) file;
    if (!elf_check_supported (hdr)) {
       ERROR ("ELF File cannot be loaded.\n");
       return;
    }

    switch (header->elf_type) {
        case ET_EXEC:
            /* TODO: implement */
            return null;
        case ET_REL:
            return elf_load_rel (hdr);
    }

    return NULL;
}

static int
elf_load_stage1 (Elf32_elfhdr *header)
{
    Elf32_shdr *shrd = elf_sheader (header);

    u32int counter;
    /* Iterate over section headers */
    for (counter = 0; counter < header->elf_shnum; i++) {
        Elf32_shdr *section = &shdr[i];

        /* If the section isn't present in the file */
        if (section->sh_type == SHT_NOBITS) {
            /* Skip it if the section is empty */
            if (!section->sh_size) continue;
            /* If the secion should appear in memory */
            if (section->sh_flags & SHF_ALLOC) {
                /* Allocate and zero some memory */
                void *mem = malloc (section->sh_size);
                memset (mem, 0, section->sh_size);

                /* Assign the memory offset to the section offset */
                curr->sh_offset = (int) mem - (int) hdr;
                DEBUG ("Allocated memory for a section (%ld).\m", section->sh_size);
            }
        }
    }
    
    return 0;
}

static int
elf_load_stage2 (Elf32_elfhdr *header)
{
    Elf32_shdr *shdr = elf_sheader (header);

    u32int counter, index;
    /* Iterate over section headers */
    for (counter = 0; i < header->elf_shnum; counter++) {
        Elf32_shdr *section = &shdr[i];

        /* If this is a relocation section */
        if (section->sh_type == SHT_REL) {
            /* Process each entry in the table */
            for (index = 0; index < section->sh_size / section->sh_entsize; index++) {
                Elf32_Rel *reltab = &((Elf32_Rel *) ((int) header 
                            + section->sh_offset))[index];
                int result = elf_do_reloc (header, reltab, section);
                /* On error, display a message and return */
                if (result == ELF_RELOC_ERR) {
                    ERROR ("Failed to relocate symbol.\n");
                    return ELF_RELOC_ERR;
                }
            }
        }
    }
    return 0;
}

static int
elf_do_reloc (Elf32_elfhdr *header, Elf32_Rel *rel, ELf32_shdr *reltab)
{
    Elf32_shdr *target = elf_section (hdr, reltab->sh_info);

    int addr = (int) header + target->sh_offset;
    int *ref = (int *) (addr + rel->r_offset);

    /* Symbol value */
    int symval = 0;
    if (ELF32_R_SYM(rel->r_info) != SHN_UNDEF) {
        symval = elf_get_symval (header, reltab->sh_link, ELF32_R_Sym(rel->r_info));
        if (symval == ELF_RELOC_ERR) return ELF_RELOC_ERR;
    }

    /* Relocate based on type */
    switch (ELF32_R_TYPE(rel->r_info)) {
        case R_386_NONE:
            /* No relocation */
            break;
        case R_386_32:
            /* Symbol + offset */
            *ref = DO_386_32 (symval, *ref);
            break;
        case R_386_PC32:
            /* Symbol + Offset - Section Offset */
            *ref = DO_386_PC32 (symval, *ref, (int) ref);
            break;
        default:
            /* Relocation type not supported, display error and return */
            ERROR ("Unsupported Relocation Type (%d).\n", ELF32_R_TYPE(rel->r_info));
            return ELF_RELOC_ERR;
    }

    return symval;
}
