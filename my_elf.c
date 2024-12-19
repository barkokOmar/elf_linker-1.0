#include <stdio.h>
#include <stdlib.h>
#define NDEBUG		// commenter pour ignorer les assert à la compilation
#include <assert.h>

#include "util.h"
#include "my_elf.h"




erreur_t read_header(FILE *fichier, Elf32_Ehdr *entete) {
	assert(fichier);	// le fichier doit etre ouvert en lecture bit a bit
	assert(entete);		// l'entete doit etre un pointeur valide

    size_t taille_lue;	// utilisee pour verifier le nombre d'octets lues
	taille_lue = 0;
	printf("taille_lue: %ld\n", taille_lue);

    // lire les 16 premier octets pour elf identification
    taille_lue = fread(&(entete->e_ident), sizeof(unsigned char), EI_NIDENT, fichier);
    assert(sizeof(unsigned char)*EI_NIDENT == taille_lue);

    // lire e_type
    taille_lue = fread(&(entete->e_type), 1, sizeof(Elf32_Half), fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	// lire e_machine
    taille_lue = fread(&(entete->e_machine), 1, sizeof(Elf32_Half), fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

    // lire e_version
    taille_lue = fread(&(entete->e_version), 1, sizeof(Elf32_Word), fichier);
	assert(sizeof(Elf32_Word) == taille_lue);

	// lire e_entry
    taille_lue = fread(&(entete->e_entry), 1, sizeof(Elf32_Addr), fichier);
	assert(sizeof(Elf32_Addr) == taille_lue);
	
	// lire e_phoff
    taille_lue = fread(&(entete->e_phoff), 1, sizeof(Elf32_Off), fichier);
	assert(sizeof(Elf32_Off) == taille_lue);
	
    // lire e_shoff 
    taille_lue = fread(&(entete->e_shoff), 1, sizeof(Elf32_Off), fichier);
	assert(sizeof(Elf32_Off) == taille_lue);

	// lire e_flags
    taille_lue = fread(&(entete->e_flags), 1, sizeof(Elf32_Word), fichier);
	assert(sizeof(Elf32_Word) == taille_lue);

    // lire e_ehsize
    taille_lue = fread(&(entete->e_ehsize), 1, sizeof(Elf32_Half), fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	// lire e_phentisize
    taille_lue = fread(&(entete->e_phentsize), 1, sizeof(Elf32_Half), fichier);
	assert(sizeof(Elf32_Half) == taille_lue);
	
	// lire e_phnum
    taille_lue = fread(&(entete->e_phnum), 1, sizeof(Elf32_Half), fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	// lire e_shentsize
    taille_lue = fread(&(entete->e_shentsize), 1, sizeof(Elf32_Half), fichier);
	assert(sizeof(Elf32_Half) == taille_lue);
	
	// lire e_shnum
    taille_lue = fread(&(entete->e_shnum), 1, sizeof(Elf32_Half), fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	// lire e_shstrndx
    taille_lue = fread(&(entete->e_shstrndx), 1, sizeof(Elf32_Half), fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	return (erreur_t)SUCCESS;		// pour l'instant ca sert à rien
}


void affiche_header(Elf32_Ehdr entete) {
    printf("ELF header:\n");
    printf("  Magic:    ");
    for(int i=0; i < EI_NIDENT; i++) {
        printf("%02x ", entete.e_ident[i]);
    }
    printf("\n");

     //---------------EI_CLASS----------
    printf("  Class:\t\t\t\t");
    if(entete.e_ident[EI_CLASS] == ELFCLASS32){
        printf("ELF32\n");
    } else {
        printf("pas ELF32\n");
    }

    //---------------EI_DATA----------
    printf("  Data:\t\t\t\t\t");
    if(entete.e_ident[EI_DATA] == ELFDATA2LSB){
        printf("2's complement, little endian\n");
    } else if(entete.e_ident[EI_DATA] == ELFDATA2MSB)  {
        printf("2's complement, big endian\n");
    } else {
		assert(ELFDATANONE == entete.e_ident[EI_DATA]);
        printf("Invalid data encoding\n");
    }
	
    //---------------EI_VERSION----------
    printf("  Version:\t\t\t\t");
	if (entete.e_ident[EI_VERSION] == EV_CURRENT) {
		printf("1 (current)\n");
	} else {
		printf("invalid versionn\n");
	}

    //----------------e_type----------
    printf("  Type:\t\t\t\t\t");
	switch (entete.e_type) {
		case ET_REL :
			printf("Relocatable file\n");
			break;
		case ET_EXEC :
			printf("Executable file\n");
			break;
		case ET_DYN :
			printf("Shared object file\n");
			break;
		case ET_CORE :
			printf("Core file\n");
			break;
		case ET_LOPROC:
			printf("Processor-specific (LOPROC)\n");
			break;
		case ET_HIPROC :
			printf("Processor-specific (HIPROC)\n");
			break;
		default:
			printf("No file type (should be 0, actual value: [%d])\n", entete.e_type);
			assert(0 == entete.e_type);
	}
    //----------------e_machine----------
	printf("  Machine:\t\t\t\t");
	switch (entete.e_machine) {
		case ET_NONE :
			printf("No machine\n");
			break;
		case EM_M32 : 
			printf("AT&T WE 32100\n");
			break;
		case EM_SPARC : 
			printf("SPARC\n");
			break;
		case EM_386 : 
			printf("Intel Architecture\n");
			break;
		case EM_68K : 
			printf("Motorola 68000\n");
			break;
		case EM_88K : 
			printf("Motorola 88000\n");
			break;
		case EM_860 : 
			printf("Intel 80860\n");
			break;
		case EM_MIPS : 
			printf("MIPS RS3000 Big-Endian\n");
			break;
		case EM_MIPS_RS4_BE : 
			printf("MIPS RS4000 Big-Endian\n");
			break;
		default:
			printf("Reserved value [%04x]\n", entete.e_machine);
	}


    //----------------e_version----------
	printf("  Version:\t\t\t\t0x%x\n", entete.e_version);

    //----------------Le Reste----------
    printf("  Entry point address:\t\t\t%d\n", entete.e_entry);
    printf("  Start of program headers:\t\t%d (bytes into file)\n", entete.e_phoff);
    printf("  Start of section headers:\t\t%d (bytes into file)\n", entete.e_shoff);
    printf("  Flags:\t\t\t\t%x (bytes into file)\n", entete.e_flags);
    printf("  Size of this header:\t\t\t%d(bytes)\n", entete.e_ehsize);
    printf("  Size of program headers:\t\t%x(bytes)\n", entete.e_phentsize);
    printf("  Number of program headers:\t\t%d\n", entete.e_phnum);
    printf("  Size of section headers:\t\t%d(bytes)\n", entete.e_shentsize);
    printf("  Number of section headers:\t\t%d\n", entete.e_shnum);
    printf("  Section header string table index:\t%d\n", entete.e_shstrndx);
}

