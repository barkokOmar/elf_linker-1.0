/*
fichier contenant les structures elf et fonction de manipulation de elf necessaire
*/

#ifndef __ELF_H__
#define __ELF_H__
#include <stdint.h>

#define SUCCES 1
#define FAIL 2
#define MEMORYFAIL 4

#define EI_NIDENT 16
typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
 } Elf_header;


#endif