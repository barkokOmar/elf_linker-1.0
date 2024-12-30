//#define NDEBUG		// commenter cette ligne pour activer les asserts 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <elf.h>
#include <string.h>
#include "util.h"
#include "my_elf.h"

void swap_endianess_ehdr(Elf32_Ehdr *entete) {
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
	assert(taille_lue == sizeof(Elf32_Half));
	return taille_lue;
}
int read_Word(void *ptr, FILE *stream) {
	assert(ptr);
	int taille_lue = fread(ptr, 1, sizeof(Elf32_Word), stream);
	assert(taille_lue == sizeof(Elf32_Word));
	return taille_lue;
}
int read_Addr(void *ptr, FILE *stream) {
	assert(ptr);
	int taille_lue = fread(ptr, 1, sizeof(Elf32_Addr), stream);
	assert(taille_lue == sizeof(Elf32_Addr));
	return taille_lue;
}
int read_Off(void *ptr, FILE *stream) {
	assert(ptr);
	int taille_lue = fread(ptr, 1, sizeof(Elf32_Off), stream);
	assert(taille_lue == sizeof(Elf32_Off));
	return taille_lue;
}
int read_Sword(void *ptr, FILE *stream) {
	assert(ptr);
	int taille_lue = fread(ptr, 1, sizeof(Elf32_Sword), stream);
	assert(taille_lue == sizeof(Elf32_Sword));
	return taille_lue;
}


size_t read_header(FILE *fichier, Elf32_Ehdr *entete) {
	assert(fichier);	// le fichier doit etre ouvert en lecture bit a bit
	assert(entete);		// l'entete doit etre un pointeur valide

	size_t taille_lue = 0;

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

	if (!is_big_endian()) {
		swap_endianess_ehdr(entete);
	}

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
			swap_endianess_ehdr(&entete);
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
			printf("EXEC (Executable file)\n");
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
///////////////////////////////////////////////////////////////////
void swap_endianess_section_table(Elf32_Shdr *shtable) {
	assert(shtable);
	shtable->sh_name = reverse_4(shtable->sh_name);
	shtable->sh_type = reverse_4(shtable->sh_type);
	shtable->sh_flags = reverse_4(shtable->sh_flags);
	shtable->sh_addr = reverse_4(shtable->sh_addr);
	shtable->sh_offset = reverse_4(shtable->sh_offset);
	shtable->sh_size = reverse_4(shtable->sh_size);
	shtable->sh_link = reverse_4(shtable->sh_link);
	shtable->sh_info = reverse_4(shtable->sh_info);
	shtable->sh_addralign = reverse_4(shtable->sh_addralign);
	shtable->sh_entsize = reverse_4(shtable->sh_entsize);
}

const char* get_section_type(Elf32_Word type) {
    switch (type) {
        case SHT_NULL: return "NULL";
        case SHT_PROGBITS: return "PROGBITS";
        case SHT_SYMTAB: return "SYMTAB";
        case SHT_STRTAB: return "STRTAB";
        case SHT_RELA: return "RELA";
        case SHT_HASH: return "HASH";
        case SHT_DYNAMIC: return "DYNAMIC";
        case SHT_NOTE: return "NOTE";
        case SHT_NOBITS: return "NOBITS";
        case SHT_REL: return "REL";
        case SHT_SHLIB: return "SHLIB";
        case SHT_DYNSYM: return "DYNSYM";
        case SHT_LOPROC: return "LOPROC";
        case SHT_HIPROC: return "HIPROC";
        case SHT_LOUSER: return "LOUSER";
        case SHT_HIUSER: return "HIUSER";
        default: return "UNKNOWN";
    }
}

void afficher_strtable(char *shstrtab_data, Elf32_Shdr shtable) {
	printf("Affichage de la table des chaines de caractères\n");
	for (int i = 0; i < shtable.sh_size; i++)
		printf("%c", shstrtab_data[i]);
	printf("done.\n");
}

size_t read_strtab(FILE *file, Elf32 *elfdata, const char *section_name) { 
	assert(file);
	assert(elfdata);
	assert(elfdata->shtable);
	assert(section_name);
	assert(!strcmp(section_name, ".strtab") || !strcmp(section_name, ".shstrtab"));

	size_t taille_lue = 0;
	size_t taille_strtab = 0;
	int strtab_index = 0;
	int is_right_section = 0;
	Elf32_Shdr strtab_section;

	if (!strcmp(section_name, ".shstrtab")) {
		strtab_index = get_shstrndx(&(elfdata->elfhdr));
	} else { // on cherche l'indice de la bonne section
		assert(elfdata->sh_strtab);
		is_right_section = !strcmp(section_name, get_section_name(*elfdata, strtab_index));
		while ( strtab_index < get_shnum(&(elfdata->elfhdr)) && !is_right_section ) {
			strtab_index++;
			is_right_section = !strcmp(section_name, get_section_name(*elfdata, strtab_index));
		}
		assert(is_right_section);
	}
	
	strtab_section = elfdata->shtable[strtab_index];
	taille_strtab = strtab_section.sh_size;

	fseek(file, strtab_section.sh_offset, SEEK_SET);

	if (!strcmp(section_name, ".shstrtab")) {
		elfdata->sh_strtab = (char*)malloc(taille_strtab);
		assert(elfdata->sh_strtab);
		taille_lue = fread(elfdata->sh_strtab, 1, taille_strtab, file);
	} else {
		elfdata->sym_strtab = (char*)malloc(taille_strtab);
		assert(elfdata->sym_strtab);
		taille_lue = fread(elfdata->sym_strtab, 1, taille_strtab, file);
	}
	
	if(taille_lue != taille_strtab) {
		fprintf(stderr, "Erreur (read_strtab) durant la lecture de strtab\n");
		return 0;
	}

	return taille_lue;
}

/*
void read_shnames(FILE *file, Elf32_Half e_shstrndx, Elf32_Shdr *shtable, char **shstrtab_data) { 
	
	Elf32_Shdr shstrtab = shtable[e_shstrndx];

	*shstrtab_data = (char*)malloc(shstrtab.sh_size);
	if (!shstrtab_data) {
		perror("Erreur d'allocation de memoire pour Section Names");
		exit(1);
	}

	//fseek(file, shstrtab.sh_offset, SEEK_SET);
	fseek(file, 0x0000e8, SEEK_SET);

	if(fread(*shstrtab_data, 1, shstrtab.sh_size, file) != shstrtab.sh_size){
		perror("Erreur de lecture pour Section Name String Table");
		free(*shstrtab_data);
		exit(1);
	}

    afficher_strtable(*shstrtab_data, shstrtab);
}
*/



void read_shtable(FILE *file, Elf32_Ehdr *elfhdr, Elf32_Shdr **shtable) {
	assert(file);
	assert(elfhdr);

	fseek(file, get_shoff(elfhdr), SEEK_SET);

	Elf32_Half number_of_entries = get_shnum(elfhdr);
	Elf32_Half each_entry_size = get_shentsize(elfhdr);
	size_t taille_shtable = number_of_entries * each_entry_size;	// taille en octets

	*shtable = (Elf32_Shdr*)malloc(taille_shtable);
	assert(*shtable);

	if (fread(*shtable, each_entry_size, number_of_entries, file) != number_of_entries) {
		fprintf(stderr, "<read_shtable> Erreur de lecture de la table des sections\n");
		exit(1);
	}

    if (!is_big_endian()) {
		for (int i = 0; i < get_shnum(elfhdr); i++)
			swap_endianess_section_table(&(*shtable)[i]);
	}
}

void affiche_shtable(Elf32_Ehdr *elfhdr, Elf32_Shdr *shtable, char *shstrtab_data) {
	printf("There are %d section headers, starting at offset 0x%x:\n\n", elfhdr->e_shnum, elfhdr->e_shoff);
	printf("Section Headers:\n");
    printf("  [Nr] Name              Type            Addr     Off    Size\n");    
	for (int i = 0; i < get_shnum(elfhdr); i++) {
		printf("  [%2d] %-18s%-15s %08x %06x %06x\n", i, 
			&shstrtab_data[shtable[i].sh_name],get_section_type(shtable[i].sh_type),shtable[i].sh_addr,shtable[i].sh_offset,shtable[i].sh_size
		);
    }
} 

void affiche_contenu_section(FILE *file, Elf32_Shdr *shtable, char *shstrtab_data, int sectionIndex) {
	assert(file);
	assert(shtable);

    Elf32_Shdr section = shtable[sectionIndex];

    fseek(file, section.sh_offset, SEEK_SET);

    uint8_t *section_data = (uint8_t*)malloc(section.sh_size);
    if (section_data == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(1);
    }
    memset(section_data, 0, section.sh_size);
    if (fread(section_data, 1, section.sh_size, file) != section.sh_size) {
        perror("Erreur de lecture des données de la section");
        free(section_data);
        exit(1);
    }
	Elf32_Addr sh_addr = shtable[sectionIndex].sh_addr;
    printf("Hex dump of section '%s':\n", &shstrtab_data[section.sh_name]);
	printf("0x%08x ",sh_addr);
	int j=0;
    for (int i=0; i < section.sh_size; i++) {
        printf("%02x", section_data[i]);
		if ((i+1)%4==0){//pour faire un espace chaque 4 octet affiché
			printf(" ");
		}
		if ((i+1) == section.sh_size && (i + 1) % 16 != 0){
			//boucle pour ameliorer l'affichage 
			int e = i+1;
			while (e % 16 != 0){
				printf("  ");
				if ((e+1)%4==0){
					printf(" ");
				}
				e++;
			}
		}
        if ((i + 1) % 16 == 0 || (i+1) == section.sh_size ) {//(i + 1) % 16 == 0 : pour ne pas dépasser 16 octet par ligne
			while (j<=i){  
				// Si le caractère est imprimable, l'afficher, sinon afficher un point
				if (section_data[j]>= 32 && section_data[j] <= 126) {
					printf("%c", section_data[j]);
				} else {
					printf(".");
				}
				if ((j + 1) % 16 == 0 || (j+1) == section.sh_size) {
					if (j!=section.sh_size-1){
					printf("\n");
					sh_addr = sh_addr + 10;
					printf("0x%08x ",sh_addr);
					}
					else {
						printf("\n");
					}

				}
				j++;
			}
        }
    }
    free(section_data);
}

void swap_endianess_symtable(Elf32_Sym *symtable) {
	assert(symtable);
	symtable->st_name =	reverse_4(symtable->st_name);
	symtable->st_value = reverse_4(symtable->st_value);
	symtable->st_size =	reverse_4(symtable->st_size);
	//symtable->st_info =	reverse_4(symtable->st_info); 	// vue que un char = 1 octet, pas besoin de swap
	//symtable->st_other = reverse_4(symtable->st_other); 	// idem
	symtable->st_shndx = reverse_2(symtable->st_shndx);
}

int is_symatble(Elf32_Shdr *shtable, int index) {
	return SHT_SYMTAB == shtable[index].sh_type;
}

void read_symtable(FILE *file, Elf32_Sym **symtable, Elf32_Ehdr *elfhdr, Elf32_Shdr **shtable, int *symtabIndex) {

	assert(file);
	assert(shtable);

	// on cherche l'indice de la table de symboles dans la table des sections
	*symtabIndex = 0;
	while ( *symtabIndex < get_shnum(elfhdr) && !is_symatble(*shtable, *symtabIndex) )
		(*symtabIndex)++;
	
	Elf32_Shdr sh_symtable = (*shtable)[*symtabIndex];

	Elf32_Word symtable_size = sh_symtable.sh_size;
	int nombreDeSymboles = symtable_size / sizeof(Elf32_Sym);
	
	fseek(file, sh_symtable.sh_offset, SEEK_SET);

	*symtable = (Elf32_Sym*)malloc(symtable_size);
	assert(*symtable);

	// lecture de la table des symboles
	for (int i = 0; i < nombreDeSymboles; i++) {
		if (fread(&(*symtable)[i], 1, sizeof(Elf32_Sym), file) != sizeof(Elf32_Sym)) {
			fprintf(stderr, "Erreur de lecture de la table des symboles\n");
			exit(1);
		}
		if (!is_big_endian())
			swap_endianess_symtable(&(*symtable)[i]);
	}

}

void affiche_symtable(Elf32 elfdata, int symtabIndex) {
	Elf32_Shdr symtable_section = elfdata.shtable[symtabIndex];
	Elf32_Word symtable_size = symtable_section.sh_size ;
	Elf32_Sym symbole;
	int nombreDeSymboles = symtable_size / sizeof(Elf32_Sym);

	printf("Symbol table '%s' contains %d entries:\n", &elfdata.sh_strtab[symtable_section.sh_name], nombreDeSymboles);
    printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

	for (int i = 0; i < nombreDeSymboles; i++) {
		symbole = elfdata.symtable[i];
		printf("   %3d: %08x  %4d %-8s%-7sTOTOTOT  %3d %-s\n",
			i, symbole.st_value, symbole.st_size, get_symtype(symbole.st_info), 
			get_symbind(symbole.st_info), symbole.st_shndx, &(elfdata.sym_strtab[symbole.st_name])
		);
    }
} 
/*
void affiche_symtable(Elf32_Shdr *shtable, Elf32_Sym **symtable, char *strtab, int symtabIndex) {
	Elf32_Word symtable_size = shtable[symtabIndex].sh_size ;
	Elf32_Sym symbole;
	int nombreDeSymboles = symtable_size / sizeof(Elf32_Sym);

	printf("Symbol table '%s' contains %d entries:\n", &strtab[shtable[symtabIndex].sh_name], nombreDeSymboles);
    printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

	for (int i = 0; i < nombreDeSymboles; i++) {
		symbole = (*symtable)[i];
		printf("   %3d: %08x  %4d %-8s%-7sTOTOTOT  %3d %-s\n",
			i, symbole.st_value, symbole.st_size, get_symtype(symbole.st_info), 
			get_symbind(symbole.st_info), symbole.st_shndx, &strtab[symbole.st_name]
		);
    }
} 
*/






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

////////////////////////////////////

const char* get_symbind(unsigned char st_info) {
    switch (ELF32_ST_BIND(st_info)) {
        case STB_LOCAL:
            return "LOCAL";
        case STB_GLOBAL:
            return "GLOBAL";
        case STB_WEAK:
            return "WEAK";
        case STB_LOPROC:
            return "LOPROC";
        case STB_HIPROC:
            return "HIPROC";
        default:
            return "UNKNOWN";
    }
}
const char* get_symtype(unsigned char st_info) {
    switch (ELF32_ST_TYPE(st_info)){
        case STT_NOTYPE:
            return "NOTYPE";
        case STT_OBJECT:
            return "OBJECT";
        case STT_FUNC:
            return "FUNC";
        case STT_SECTION:
            return "SECTION";
        case STT_FILE:
            return "FILE";
        case STT_LOPROC:
            return "LOPROC";
        case STT_HIPROC:
            return "HIPROC";
        default:
            return "UNKNOWN";
    }
}

const char *get_section_name(Elf32 elfdata, int index) {
	Elf32_Half shnum = get_shnum(&(elfdata.elfhdr));
	assert(0 <= index && index < shnum);
	Elf32_Word sh_name = elfdata.shtable[index].sh_name;
	return &(elfdata.sh_strtab[sh_name]);
}