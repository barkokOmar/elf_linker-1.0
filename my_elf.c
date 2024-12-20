#include <stdio.h>
#include <stdlib.h>
//#define NDEBUG		// commenter ce define pour activer les assert à la compilation
#include <assert.h>
#include <elf.h>

#include "util.h"
#include "my_elf.h"


void swap_endianess(Elf32_Ehdr *entete) {
	assert(entete);
	entete->e_type		 = reverse_2(entete->e_type);
	entete->e_machine	 = reverse_2(entete->e_machine);
	entete->e_version	 = reverse_4(entete->e_version);
	entete->e_entry		 = reverse_4(entete->e_entry);
	entete->e_phoff		 = reverse_4(entete->e_phoff);
	entete->e_shoff		 = reverse_4(entete->e_shoff);
	entete->e_flags		 = reverse_4(entete->e_flags);
	entete->e_ehsize	 = reverse_2(entete->e_ehsize);
	entete->e_phentsize	 = reverse_2(entete->e_phentsize);
	entete->e_phnum		 = reverse_2(entete->e_phnum);
	entete->e_shentsize	 = reverse_2(entete->e_shentsize);
	entete->e_shnum		 = reverse_2(entete->e_shnum);
	entete->e_shstrndx	 = reverse_2(entete->e_shstrndx);
}


/* lie le bon nombre d'octets selon la taille du type */
int read_Half(void *ptr, FILE *stream) {
	assert(ptr);
	int taille_lue = fread(ptr, 1, sizeof(Elf32_Half), stream);
	if (is_big_endian())
		*((Elf32_Half*)ptr) = reverse_2(*((Elf32_Half*)ptr));
	assert(taille_lue == sizeof(Elf32_Half));
	return taille_lue;
}
int read_Word(void *ptr, FILE *stream) {
	assert(ptr);
	int taille_lue = fread(ptr, 1, sizeof(Elf32_Word), stream);
	if (!is_big_endian())
		*((Elf32_Word*)ptr) = reverse_4(*((Elf32_Word*)ptr));
	assert(taille_lue == sizeof(Elf32_Word));
	return taille_lue;
}
int read_Addr(void *ptr, FILE *stream) {
	assert(ptr);
	int taille_lue = fread(ptr, 1, sizeof(Elf32_Addr), stream);
	if (!is_big_endian())
		*((Elf32_Addr*)ptr) = reverse_4(*((Elf32_Addr*)ptr));
	assert(taille_lue == sizeof(Elf32_Addr));
	return taille_lue;
}
int read_Off(void *ptr, FILE *stream) {
	assert(ptr);
	int taille_lue = fread(ptr, 1, sizeof(Elf32_Off), stream);
	if (!is_big_endian())
		*((Elf32_Off*)ptr) = reverse_4(*((Elf32_Off*)ptr));
	assert(taille_lue == sizeof(Elf32_Off));
	return taille_lue;
}
int read_Sword(void *ptr, FILE *stream) {
	assert(ptr);
	int taille_lue = fread(ptr, 1, sizeof(Elf32_Sword), stream);
	if (!is_big_endian())
		*((Elf32_Sword*)ptr) = reverse_4(*((Elf32_Sword*)ptr));
	assert(taille_lue == sizeof(Elf32_Sword));
	return taille_lue;
}

//int read_Shdr(void *ptr,FILE *stream)

int read_header(FILE *fichier, Elf32_Ehdr *entete) {
	assert(fichier);	// le fichier doit etre ouvert en lecture bit a bit
	assert(entete);		// l'entete doit etre un pointeur valide

	int taille_lue = 0;

    // lecture des 16 premiers octets (e_ident)
    taille_lue += fread(&(entete->e_ident), sizeof(unsigned char), EI_NIDENT, fichier);
    assert(sizeof(unsigned char)*EI_NIDENT == taille_lue);

	// lecture du reste de l'entete
	taille_lue += read_Half(&(entete->e_type), fichier);
	taille_lue += read_Half(&(entete->e_machine), fichier);
	taille_lue += read_Word(&(entete->e_version), fichier);
	taille_lue += read_Addr(&(entete->e_entry), fichier);
	taille_lue += read_Off(&(entete->e_phoff), fichier);
	taille_lue += read_Off(&(entete->e_shoff), fichier);
	taille_lue += read_Word(&(entete->e_flags), fichier);
	taille_lue += read_Half(&(entete->e_ehsize), fichier);
	taille_lue += read_Half(&(entete->e_phentsize), fichier);
	taille_lue += read_Half(&(entete->e_phnum), fichier);
	taille_lue += read_Half(&(entete->e_shentsize), fichier);
	taille_lue += read_Half(&(entete->e_shnum), fichier);
	taille_lue += read_Half(&(entete->e_shstrndx), fichier);

	return taille_lue;
}


void affiche_header(Elf32_Ehdr entete) {
	printf("we are in %s endian\n", is_big_endian() ? "big" : "little");

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
    if (entete.e_ident[EI_DATA] == ELFDATA2LSB) {
        printf("2's complement, little endian\n");
    } else if (entete.e_ident[EI_DATA] == ELFDATA2MSB) {
        printf("2's complement, big endian\n");
		/*
		if (!is_big_endian())	// ici on aura swap alors qu'elles sont déjà dans le bon boutisme...
			swap_endianess(&entete);
		*/
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
		case ET_HIPROC :
			printf("Processor-specific (HIPROC)\n");
			break;
		default:
			printf("No file type (should be 0, actual value: [0x%04x] )\n", reverse_2(entete.e_type));
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
		default:
			printf("Reserved value [0x%04x]\n", entete.e_machine);
	}


    //----------------e_version----------
	printf("  Version:\t\t\t\t0x%x\n", entete.e_version);

    //----------------Le Reste----------
    printf("  Entry point address:\t\t\t0x%02x\n", entete.e_entry);
    printf("  Start of program headers:\t\t%d (bytes into file)\n", entete.e_phoff);
    printf("  Start of section headers:\t\t%d (bytes into file)\n", entete.e_shoff);
    printf("  Flags:\t\t\t\t0x%x\n", entete.e_flags);
    printf("  Size of this header:\t\t\t%d (bytes)\n", entete.e_ehsize);
    printf("  Size of program headers:\t\t%d (bytes)\n", entete.e_phentsize);
    printf("  Number of program headers:\t\t%d\n", entete.e_phnum);
    printf("  Size of section headers:\t\t%d (bytes)\n", entete.e_shentsize);
    printf("  Number of section headers:\t\t%d\n", entete.e_shnum);
    printf("  Section header string table index:\t%d\n", entete.e_shstrndx);
}

uint8_t *get_shname(FILE *file, Elf32_Shdr *shtable, Elf32_Half shstrndx, int sectionIndex) {
	assert(file);

	Elf32_Word sh_name = shtable[sectionIndex].sh_name;
	Elf32_Off shstrOffset = shtable[shstrndx].sh_offset;

	fseek(file, shstrOffset + sh_name, SEEK_SET);
	///////////////////// a finir ///////////////////////
}

Elf32_Shdr *read_shtable(FILE *file, Elf32_Ehdr *elfhdr) {
	assert(file);
	assert(elfhdr);

	Elf32_Half taille_shtable = get_shnum(elfhdr);

	fseek(file, get_shoff(elfhdr), SEEK_SET);

	Elf32_Shdr *shtable = (Elf32_Shdr*)malloc(sizeof(Elf32_Shdr) * taille_shtable);
	assert(shtable);

	if (fread(shtable, sizeof(Elf32_Shdr), taille_shtable, file) != taille_shtable) {
		perror("Erreur dans la lecture de Section Header");
		exit(1);
	}

	printf("Section Headres:\n");
	printf("  [Nr] Name              Type            Addr     Off    Size \n");	
	for (int i = 0; i < elfhdr->e_shnum; i++){
		printf("  [ %d] %s               %s              %04x   %04x     %04x   ", 
		i, shtable[i].sh_name, get_shtype(shtable, i), shtable[i].sh_addr, shtable[i].sh_offset, shtable[i].sh_size);		
			// printf("Section %d", i);
			// printf("	Name: %u\n", shtable[i].sh_name);
			// printf("	Type: %u\n", shtable[i].sh_type);
			// printf("	Adresse: 0x%x\n", shtable[i].sh_addr);
			// printf("	Offset: 0x%x\n", shtable[i].sh_offset);
			// printf("	Size:	%u\n", shtable[i].sh_size);
	}
	printf(" Key to Flags:
		W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
		L (link order), O (extra OS processing required), G (group), T (TLS),
		C (compressed), x (unknown), o (OS specific), E (exclude),
		D (mbind), y (purecode), p (processor specific) "); 
	
	return shtable;
}

void read_shnames(FILE *file, Elf32_Ehdr *header, Elf32_Shdr *shtable) {
	
	Elf32_Shdr shstrtab = shtable[header->e_shstrndx];

	char *shstrtab_data = malloc(shstrtab.sh_size);
	if (!shstrtab_data){
		perror("Erruer d'allocation de memoire pour Section Names");
		exit(1);
	}

	fseek(file, shstrtab.sh_offset, SEEK_SET);
	if(fread(shstrtab_data, 1, shstrtab.sh_size, file) != shstrtab.sh_size){
		perror("Erreur de lecture pour Section Name String Table");
		free(shstrtab_data);
		exit(1);
	}

	printf("Section Names:\n");
	for(int i = 0; i < header->e_shnum; i++){
		printf("Section %d: %s\n", i, &shstrtab_data[shtable[i].sh_name]);
	}
	free(shstrtab_data);
}




Elf32_Half get_type(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_type; 
}

Elf32_Half get_machine(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_machine; 
}

Elf32_Word get_version(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_version; 
}

Elf32_Addr get_entry(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_entry; 
}

Elf32_Off get_phoff(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_phoff; 
}

Elf32_Off get_shoff(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_shoff; 
}

Elf32_Word get_flags(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_flags; 
}

Elf32_Half get_ehsize(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_ehsize; 
}

Elf32_Half get_phentsize(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_phentsize; 
}

Elf32_Half get_phnum(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_phnum; 
}

Elf32_Half get_shentsize(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_shentsize; 
}

Elf32_Half get_shnum(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_shnum; 
}

Elf32_Half get_shstrndx(Elf32_Ehdr *entete) { 
    assert(entete != NULL);
    return entete->e_shstrndx; 
}