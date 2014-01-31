/* This file is part of fotix
 * Copyright (c) 2013 Fotis Koutoulakis
 *
 * elf.c -- Defines the functions needed
 * to support elf file loading in the kernel.
 */

#include "common.h"
#include "elf.h"

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
