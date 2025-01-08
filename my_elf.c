//#define NDEBUG		// commenter cette ligne pour activer les asserts 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <elf.h>
#include <string.h>
#include "util.h"
#include "my_elf.h"

void swap_endianess_elfhdr(Elf32_Ehdr *entete) {
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

	if (!is_big_endian())
		swap_endianess_elfhdr(entete);

	return taille_lue;
}


void affiche_header(Elf32_Ehdr entete) {

    printf("ELF Header:\n");
    printf("  Magic:   ");
    for(int i=0; i < EI_NIDENT; i++)
        printf("%02x ", entete.e_ident[i]);
    printf("\n");
    //---------------EI_CLASS----------
    printf("  Class:                             ");
    if(entete.e_ident[EI_CLASS] == ELFCLASS32)
        printf("ELF32\n");
    else
        printf("pas ELF32\n");
    //---------------EI_DATA----------
    printf("  Data:                              ");
    if (entete.e_ident[EI_DATA] == ELFDATA2LSB)
        printf("2's complement, little endian\n");
    else if (entete.e_ident[EI_DATA] == ELFDATA2MSB)
        printf("2's complement, big endian\n");
    else {
		assert(ELFDATANONE == entete.e_ident[EI_DATA]);
        printf("Invalid data encoding\n");
    }
    //---------------EI_VERSION----------
    printf("  Version:                           ");
	if (entete.e_ident[EI_VERSION] == EV_CURRENT)
		printf("1 (current)\n");
	else
		printf("invalid versionn\n");
    //----------------e_type----------
    printf("  Type:                              ");
	switch (entete.e_type) {
		case ET_REL :
			printf("REL (Relocatable file)\n");
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
	printf("  Machine:                           ");
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
	printf("  Version:                           0x%x\n", entete.e_version);

    //----------------Le Reste----------
    printf("  Entry point address:               0x%x\n", entete.e_entry);
    printf("  Start of program headers:          %d (bytes into file)\n", entete.e_phoff);
    printf("  Start of section headers:          %d (bytes into file)\n", entete.e_shoff);
    printf("  Flags:                             0x%x\n", entete.e_flags);
    printf("  Size of this header:               %d (bytes)\n", entete.e_ehsize);
    printf("  Size of program headers:           %d (bytes)\n", entete.e_phentsize);
    printf("  Number of program headers:         %d\n", entete.e_phnum);
    printf("  Size of section headers:           %d (bytes)\n", entete.e_shentsize);
    printf("  Number of section headers:         %d\n", entete.e_shnum);
    printf("  Section header string table index: %d\n", entete.e_shstrndx);
}
///////////////////////////////////////////////////////////////////
void swap_endianess_shtable(Elf32_Shdr *shtable) {
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
		case SHT_ARM_ATTRIBUTES: return "ARM_ATTRIBUTES";
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
	Elf32_Shdr strtab_section;

	if (!strcmp(section_name, ".shstrtab")) {
		strtab_index = get_shstrndx(&(elfdata->elfhdr));
	} else { // on cherche l'indice de la bonne section
		assert(elfdata->sh_strtab);
		strtab_index = get_section_index(*elfdata, section_name);
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
			swap_endianess_shtable(&(*shtable)[i]);
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
			int e = i+1;
			while (e % 16 != 0){//boucle pour ameliorer l'affichage
				printf("  ");
				if ((e+1)%4==0) { 
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

void swap_endianess_symbole(Elf32_Sym *symtable) {
	assert(symtable);
	symtable->st_name =	reverse_4(symtable->st_name);
	symtable->st_value = reverse_4(symtable->st_value);
	symtable->st_size =	reverse_4(symtable->st_size);
	//symtable->st_info =	reverse_4(symtable->st_info); 	// vue que un char = 1 octet, pas besoin de swap
	//symtable->st_other = reverse_4(symtable->st_other); 	// idem
	symtable->st_shndx = reverse_2(symtable->st_shndx);
}

int is_symtable(Elf32_Shdr *shtable, int index) { return SHT_SYMTAB == shtable[index].sh_type; }

void read_symtable(FILE *file, Elf32_Sym **symtable, Elf32_Ehdr *elfhdr, Elf32_Shdr **shtable, int *symtabIndex) {
	assert(file);
	assert(shtable);

	// on cherche l'indice de la table de symboles dans la table des sections
	*symtabIndex = 0;
	while ( *symtabIndex < get_shnum(elfhdr) && !is_symtable(*shtable, *symtabIndex) )
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
			swap_endianess_symbole(&(*symtable)[i]);
	}

}

void affiche_symtable(Elf32 elfdata) {
	assert(elfdata.symtable);
	assert(elfdata.shtable);
	assert(elfdata.sh_strtab);
	assert(elfdata.sym_strtab);

	int symtabIndex = elfdata.symtabIndex;
	Elf32_Shdr symtable_section = elfdata.shtable[symtabIndex];
	Elf32_Word symtable_size = symtable_section.sh_size ;
	Elf32_Sym symbole;
	Elf32_Half st_shndx_int;
	const char * st_shndx_str;
	int nombreDeSymboles = symtable_size / sizeof(Elf32_Sym);

	printf("Symbol table '%s' contains %d entries:\n", &elfdata.sh_strtab[symtable_section.sh_name], nombreDeSymboles);
    printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

	for (int i = 0; i < nombreDeSymboles; i++) {
		symbole = elfdata.symtable[i];
		if (get_st_shndx(symbole.st_shndx) == NULL) {
			st_shndx_int = symbole.st_shndx;
			printf("   %3d: %08x  %4d %-8s%-7s%-9s%3d %-s\n",
				i, symbole.st_value, symbole.st_size, get_symtype(symbole.st_info), 
				get_symbind(symbole.st_info), get_st_visibility(symbole.st_other),
				st_shndx_int, &(elfdata.sym_strtab[symbole.st_name])
			);
	    } else {
			st_shndx_str = get_st_shndx(symbole.st_shndx);
			printf("   %3d: %08x  %4d %-8s%-7s%-9s%-3s %-s\n",
				i, symbole.st_value, symbole.st_size, get_symtype(symbole.st_info), 
				get_symbind(symbole.st_info), get_st_visibility(symbole.st_other),
				st_shndx_str, &(elfdata.sym_strtab[symbole.st_name])
			);
		}
    }
} 


void init_relenrty(Elf32_RelEntry *entry) {
	assert(entry);
	entry->rel = NULL;
	entry->rela = NULL;
	entry->relnum = 0;
	entry->shndx = -1;
}

int is_rela(Elf32_Word type) { return SHT_RELA == type; }
int is_rel(Elf32_Word type) { return SHT_REL == type; }

void swap_endianess_rel(Elf32_Rel *rel, int nbrel) {
	assert(rel);
	for (int i = 0; i < nbrel; i++) {
		rel[i].r_offset = reverse_4(rel[i].r_offset);
		rel[i].r_info = reverse_4(rel[i].r_info);
	}
}
void swap_endianess_rela(Elf32_Rela *rela, int nbrel) {
	assert(rela);
	for (int i = 0; i < nbrel; i++) {
		rela[i].r_offset = reverse_4(rela[i].r_offset);
		rela[i].r_info = reverse_4(rela[i].r_info);
		rela[i].r_addend = reverse_4(rela[i].r_addend);
	}
}

void read_reltab(FILE *file, Elf32 *elfdata) {
	assert(file);
	assert(elfdata);
	assert(elfdata->shtable);
	
	Elf32_Shdr *shtable = elfdata->shtable;
	Elf32_Ehdr *elfhdr = &(elfdata->elfhdr);
	Elf32_Word reltab_size = 0; 
	Elf32_Shdr sh_reltab;

	// on initialise la table des relocations
	elfdata->reltab.entries = NULL;
	int *taille_tabrel = &(elfdata->reltab.entrynum);
	*taille_tabrel = 0;
	int is_rel_or_rela;
	int nombreDeRelocations = 0;
	size_t taille_lue = 0;

	// on cherche les tables de relocations
	for (int i=0; i < get_shnum(elfhdr); i++) {
		sh_reltab = shtable[i];
		Elf32_Word section_type = sh_reltab.sh_type;
		is_rel_or_rela = is_rel(section_type) || is_rela(section_type) ;

		if (!is_rel_or_rela)
			continue;
		
		reltab_size = sh_reltab.sh_size;
		nombreDeRelocations = reltab_size / sizeof(Elf32_Rel);
		*taille_tabrel += 1;	// on incrémente le nombre de tables de relocations

		// on realloue la mémoire pour la table des relocations (pour rajouter une nouvelle entrée)
		elfdata->reltab.entries = (Elf32_RelEntry*)realloc(elfdata->reltab.entries, *taille_tabrel * sizeof(Elf32_RelEntry));
		assert(elfdata->reltab.entries);

		// on initialise la nouvelle entrée
		init_relenrty(&(elfdata->reltab.entries[*taille_tabrel-1]));
		elfdata->reltab.entries[*taille_tabrel-1].relnum = nombreDeRelocations;
		elfdata->reltab.entries[*taille_tabrel-1].shndx = i;
		elfdata->reltab.entries[*taille_tabrel-1].rel_type = section_type;

		fseek(file, sh_reltab.sh_offset, SEEK_SET);

		// on alloue la bonne mémoire pour la table des relocations de la section courante
		if (sh_reltab.sh_type == SHT_REL) {
			elfdata->reltab.entries[*taille_tabrel-1].rel = (Elf32_Rel*)malloc(reltab_size);
			assert(elfdata->reltab.entries[*taille_tabrel-1].rel);
			taille_lue = fread(elfdata->reltab.entries[*taille_tabrel-1].rel, 1, reltab_size, file);
			assert(taille_lue == reltab_size);
			swap_endianess_rel(elfdata->reltab.entries[*taille_tabrel-1].rel, nombreDeRelocations);
		} else {
			elfdata->reltab.entries[*taille_tabrel-1].rela = (Elf32_Rela*)malloc(reltab_size);
			assert(elfdata->reltab.entries[*taille_tabrel-1].rela);
			taille_lue = fread(elfdata->reltab.entries[*taille_tabrel-1].rela, 1, reltab_size, file);
			assert(taille_lue == reltab_size);
			swap_endianess_rela(elfdata->reltab.entries[*taille_tabrel-1].rela, nombreDeRelocations);
		}
	}
}

int affiche_reltab(Elf32 elfdata) {
	//printf("bonjour :)\n");
	if (elfdata.reltab.entrynum == 0) {
		//fprintf(stderr, "There are no relocations in this file.\n");
		return 0;
	}

	Elf32_RelEntry *entries = elfdata.reltab.entries;
	Elf32_Rel *rel;
	Elf32_Rela *rela;
	Elf32_Shdr *shtable = elfdata.shtable;
	Elf32_Sym *symtable = elfdata.symtable;
	char *sym_strtab = elfdata.sym_strtab;
	char *sh_strtab = elfdata.sh_strtab;
	char *SymName;

	for (int i = 0; i < elfdata.reltab.entrynum; i++) {
		printf("Relocation section '%s' at offset 0x%x contains %d entries:\n",
			&sh_strtab[shtable[entries[i].shndx].sh_name], shtable[entries[i].shndx].sh_offset, entries[i].relnum
		);
		if (is_rel(shtable[entries[i].shndx].sh_type)) {
			printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
			rel = entries[i].rel;
			for (int j = 0; j < entries[i].relnum; j++) {
				if (ELF32_ST_TYPE(symtable[ELF32_R_SYM(rel[j].r_info)].st_info) == STT_SECTION )
					SymName = &sh_strtab[shtable[symtable[ELF32_R_SYM(rel[j].r_info)].st_shndx].sh_name];
				else 
					SymName = &sym_strtab[symtable[ELF32_R_SYM(rel[j].r_info)].st_name];
	
				printf("%08x  %08x %-15s   %08x   %s\n",
					rel[j].r_offset, rel[j].r_info, get_reloc_type(rel[j].r_info), symtable[ELF32_R_SYM(rel[j].r_info)].st_value, SymName
				);
			}
		} else { // section de type SHT_RELA: on affiche les addends
			printf(" Offset     Info    Type            Sym.Value  Sym. Name + Addend\n");
			rela = entries[i].rela;
			for (int j = 0; j < entries[i].relnum; j++) {
				if (ELF32_ST_TYPE(symtable[ELF32_R_SYM(rela[j].r_info)].st_info) == STT_SECTION )
					SymName = &sh_strtab[shtable[symtable[ELF32_R_SYM(rela[j].r_info)].st_shndx].sh_name];
				else 
					SymName = &sym_strtab[symtable[ELF32_R_SYM(rela[j].r_info)].st_name];
	
				printf("%08x  %08x %-15s   %08x   %s + %x\n",
					rela[j].r_offset, rela[j].r_info, get_reloc_type(rela[j].r_info), symtable[ELF32_R_SYM(rela[j].r_info)].st_value, SymName, rela[j].r_addend);

			}     
		}
	}
	return 0;
}

///////////// Phase 2 /////////////

int get_section_index(Elf32 elfdata, const char *section_name) {
	assert(elfdata.shtable);
	assert(elfdata.sh_strtab);

	Elf32_Shdr *shtable = elfdata.shtable;
	Elf32_Ehdr *elfhdr = &(elfdata.elfhdr);
	const char *sh_strtab = elfdata.sh_strtab;
	int section_index = 0;
	int is_right_section = 0;

	while ( section_index < get_shnum(elfhdr) && !is_right_section ) {
		is_right_section = !strcmp(section_name, &sh_strtab[shtable[section_index].sh_name]);
		section_index++;
	}
	section_index--;	// on décrémente pour avoir le bon index
	if (!is_right_section) {
		section_index = -1;
	}

	return section_index ;
}

int update_sym_shndx(Elf32 *elfdata, int old_shndx, int new_shndx) {
	assert(elfdata);
	assert(elfdata->shtable);
	assert(elfdata->symtable);

	Elf32_Sym *symtable = elfdata->symtable;
	Elf32_Shdr *shtable = elfdata->shtable;

	int symtabIndex = elfdata->symtabIndex;
	Elf32_Shdr symtable_section = shtable[symtabIndex];
	Elf32_Word symtable_size = symtable_section.sh_size ;
	int nombreDeSymboles = symtable_size / sizeof(Elf32_Sym);
	
	for (int i = 0; i < nombreDeSymboles; i++) {
		if (symtable[i].st_shndx == old_shndx)
			symtable[i].st_shndx = new_shndx;
	}

	return 0;	// a revoir pour la valeur de retour
}

int supprime_sh(FILE *file, Elf32 *elfdata, int index) {
	assert(file);
	assert(elfdata);
	assert(elfdata->shtable);

	Elf32_Off shoff;
	Elf32_Half shnum;
	Elf32_Off symoff;
	Elf32_Word symtab_size;
	Elf32_Ehdr *elfhdr = &(elfdata->elfhdr);
	assert(index >= 0 && index < get_shnum(elfhdr));
	size_t taille_ecrite = 0;
	int old_shndx, new_shndx, nombreDeSymboles;
	int i = 0;

	// supprimer la section dans shtable de elfdata
	for (i=index; i < get_shnum(elfhdr); i++) {
		old_shndx = i+1;
		new_shndx = i;
		elfdata->shtable[new_shndx] = elfdata->shtable[old_shndx];
		if (is_symtable(elfdata->shtable, new_shndx))
			elfdata->symtabIndex = new_shndx;
		// on met à jour les index des tables des sections des symboles
		update_sym_shndx(elfdata, old_shndx, new_shndx);	
	}

	elfdata->shtable = (Elf32_Shdr*)realloc(elfdata->shtable, (get_shnum(elfhdr)-1) * sizeof(Elf32_Shdr));
	assert(elfdata->shtable);

	// mettre à jour : shnum, shstrndx de l'entete
	elfhdr->e_shnum -= 1;
	elfhdr->e_shstrndx = get_section_index(*elfdata, ".shstrtab");
     
	met_a_jour_shlink(elfdata);

	// ecrire la nouvelle entete et shtable dans le fichier le fichier destination
	shoff = get_shoff(elfhdr);	// on stock avant de changer l'endianess
	shnum = get_shnum(elfhdr);
	symoff = elfdata->shtable[elfdata->symtabIndex].sh_offset;
	symtab_size = elfdata->shtable[elfdata->symtabIndex].sh_size;
	nombreDeSymboles = symtab_size / sizeof(Elf32_Sym);
	if (!is_big_endian()) {		// on swap les octets si la machine est en little endian
		swap_endianess_elfhdr(elfhdr);
		for (int i = 0; i < shnum; i++)
			swap_endianess_shtable(&(elfdata->shtable[i]));

		for (int i = 0; i < nombreDeSymboles; i++)
			swap_endianess_symbole(&(elfdata->symtable[i]));
	}
	
	// on commence par ecrire l'entete
	fseek(file, 0, SEEK_SET);
	taille_ecrite = fwrite(elfhdr, sizeof(Elf32_Ehdr), 1, file);
	assert(taille_ecrite == 1);
	// puis shtable
	fseek(file, shoff, SEEK_SET);
	taille_ecrite = fwrite(elfdata->shtable, sizeof(Elf32_Shdr), shnum, file);
	assert(taille_ecrite == shnum);
	// symtable
	fseek(file, symoff, SEEK_SET);
	taille_ecrite = fwrite(elfdata->symtable, 1, symtab_size, file);
	assert(taille_ecrite == symtab_size);

	if (!is_big_endian()) {// on reswap les octets pour revenir à l'etat initial
		swap_endianess_elfhdr(elfhdr);
		for (int i = 0; i < shnum; i++)
			swap_endianess_shtable(&(elfdata->shtable[i]));

		for (int i = 0; i < nombreDeSymboles; i++)
			swap_endianess_symbole(&(elfdata->symtable[i]));
	}

	return 0;	// a revoir pour la valeur de retour
}

int supprime_rel_sections(FILE *source, FILE *dest, Elf32 *elfdata) {
	assert(source);
	assert(dest);
	assert(elfdata);
	assert(elfdata->shtable);

	Elf32_Word section_type;

	// on cherche les tables de relocations
	for (int i=0; i < get_shnum(&elfdata->elfhdr); i++) {
		section_type = elfdata->shtable[i].sh_type;
		if (is_rel(section_type)) {
			supprime_sh(dest, elfdata, i);
			i--;	// on décrémente pour ne pas sauter une section
		}
	}

	return 0;
}

int corriger_symboles(FILE *source, FILE *dest, Elf32 *elfdata, Elf32_Addr addr_text, Elf32_Addr addr_data) {
	assert(source);
	assert(dest);
	assert(elfdata);

	Elf32_Word symtab_size = elfdata->shtable[elfdata->symtabIndex].sh_size;
	Elf32_Off symoff = elfdata->shtable[elfdata->symtabIndex].sh_offset;
	size_t taille_ecrite = 0;
	int nombreDeSymboles = symtab_size / sizeof(Elf32_Sym);
   
	for (int i = 0; i < nombreDeSymboles; i++) {
		Elf32_Sym *symbole = &(elfdata->symtable[i]);
		int section_index = symbole->st_shndx;
		if (section_index == SHN_UNDEF || section_index == SHN_ABS)
			continue;
		if (section_index == get_section_index(*elfdata, ".text"))
			symbole->st_value += addr_text; 
		if (section_index == get_section_index(*elfdata, ".data"))
			symbole->st_value += addr_data;
	}
	
	if (!is_big_endian()) {
		for (int i = 0; i < nombreDeSymboles; i++)
			swap_endianess_symbole(&(elfdata->symtable[i]));

	}

	fseek(dest, symoff, SEEK_SET);	
	taille_ecrite = fwrite(elfdata->symtable, 1, symtab_size, dest);
	assert(taille_ecrite == symtab_size);

	if (!is_big_endian()) {
		for (int i = 0; i < nombreDeSymboles; i++)
			swap_endianess_symbole(&(elfdata->symtable[i]));
	}
	return 0;
}

int fwrite_sh_addr_test_data(FILE *file, Elf32 *elfdata, Elf32_Addr addr_text, Elf32_Addr addr_data){
	assert(file);
	assert(elfdata);

	Elf32_Off shoff;
	Elf32_Half shnum;
	size_t taille_ecrite = 0;

	elfdata->shtable[get_section_index(*elfdata, ".text")].sh_addr = addr_text;
	elfdata->shtable[get_section_index(*elfdata, ".data")].sh_addr = addr_data;
	shnum = get_shnum(&elfdata->elfhdr);
	shoff = get_shoff(&elfdata->elfhdr);	// on stock avant de changer l'endianess
	if (!is_big_endian()) {
		for (int i = 0; i < shnum; i++)
			swap_endianess_shtable(&(elfdata->shtable[i]));
	}
	//ecrire shtable
	fseek(file, shoff, SEEK_SET);
	taille_ecrite = fwrite(elfdata->shtable, sizeof(Elf32_Shdr), shnum, file);
	assert(taille_ecrite == shnum);

	if (!is_big_endian()) {
		for (int i = 0; i < shnum; i++)
			swap_endianess_shtable(&(elfdata->shtable[i]));
	}

    return 0;
}
void appliquer_relocations(FILE *source_stream, FILE *dest_stream, Elf32 *elfdata, Elf32_Addr addr_text, Elf32_Addr addr_data) {
	assert(elfdata);
	assert(elfdata->shtable);
	assert(elfdata->symtable);
	assert(source_stream);
	assert(dest_stream);

    fwrite_sh_addr_test_data(dest_stream, elfdata, addr_text, addr_data);//mettre à jour les adresses de .data et .text

	Elf32_RelEntry *entries = elfdata->reltab.entries;
	Elf32_Shdr *shtable = elfdata->shtable;
	Elf32_Sym *symtable = elfdata->symtable;
	Elf32_RelEntry *rel_section;
	Elf32_Rel rel_section_entry;
	unsigned char rel_section_type;
	int nombreDeSectionsRel = elfdata->reltab.entrynum;
	size_t taille_ecrite = 0;
	size_t taille_lue = 0;

	for (int i = 0; i < nombreDeSectionsRel; i++) {
        rel_section = &entries[i];
    
		rel_section_type = rel_section->rel_type;
		if (!is_rel(rel_section_type))
			continue;
        
		for (int j = 0; j < rel_section->relnum; j++) {
			rel_section_entry = rel_section->rel[j];
        
			// on recupère les informations de la relocation
			Elf32_Addr rel_offset = rel_section_entry.r_offset;
			Elf32_Word rel_type = ELF32_R_TYPE(rel_section_entry.r_info);
			Elf32_Word rel_sym_index = ELF32_R_SYM(rel_section_entry.r_info);
			
			Elf32_Sym symbole = symtable[rel_sym_index];
			Elf32_Addr adresse_absolue_symbole = symbole.st_value;
			Elf32_Shdr *sh_cible_ptr = &shtable[symbole.st_shndx];


			// Calcul de l'adresse absolue du symbole
            if (symbole.st_shndx != SHN_ABS && symbole.st_shndx != SHN_UNDEF) {
				if (get_section_index(*elfdata, ".text") == symbole.st_shndx)
					adresse_absolue_symbole += addr_text;
				if (get_section_index(*elfdata, ".data") == symbole.st_shndx)
					adresse_absolue_symbole += addr_data;				
            }

            // Lecture de la valeur actuelle à l'offset donné dans la section cible
            Elf32_Addr current_value = 0;
            fseek(source_stream, sh_cible_ptr->sh_offset + rel_offset, SEEK_SET);
            taille_lue = fread(&current_value, 1, sizeof(Elf32_Addr), source_stream);	
			assert(taille_lue == sizeof(Elf32_Addr));
			if (!is_big_endian())
				current_value = reverse_4(current_value);

            // Application de la relocation
            switch (rel_type) {
                case R_ARM_ABS32:
                    current_value += adresse_absolue_symbole;
                    break;
                case R_ARM_ABS16:
                    current_value = (adresse_absolue_symbole + current_value) & 0xFFFF;
                    break;
                case R_ARM_ABS8:
                    current_value = (adresse_absolue_symbole + current_value) & 0xFF;
                    break;
				case R_ARM_JUMP24:
				case R_ARM_CALL:
					current_value += (adresse_absolue_symbole - rel_offset);
					break;
                default:
                    fprintf(stderr, "Unsupported relocation type: 0x%x\n", rel_type);
                    continue;
            }
			if (!is_big_endian())
				current_value = reverse_4(current_value);	
            // Écriture de la valeur corrigée dans le fichier de destination
            fseek(dest_stream, sh_cible_ptr->sh_offset + rel_offset, SEEK_SET);
            taille_ecrite = fwrite(&current_value, sizeof(Elf32_Addr), 1, dest_stream); 
			assert(taille_ecrite == 1);

		}
    }
}









////////////////// autres fonctions //////////////////////

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

const char* get_st_shndx(Elf32_Half st_shndx) {
    switch (st_shndx) {
        case SHN_UNDEF:
            return "UND";
        case SHN_LORESERVE:
            return "LOR";
        case SHN_HIPROC:
            return "HIP";
        case SHN_ABS:
            return "ABS";
        case SHN_COMMON:
            return "COM";
        case SHN_HIRESERVE:
            return "HIR";
        default:
            return NULL;
    }
}
const char* get_st_visibility(unsigned char st_other) {
    switch (ELF32_ST_VISIBILITY(st_other)) {
        case STV_DEFAULT:
            return "DEFAULT";
        case STV_INTERNAL:
            return "INTERNAL";
        case STV_HIDDEN:
            return "HIDDEN";
        case STV_PROTECTED:
            return "PROTECTED";
        default:
            return "UNKNOWN";
    }
}
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

const char *get_reloc_type(Elf32_Word type) {
	switch (ELF32_R_TYPE(type)) {
		case R_ARM_ABS32:
			return "R_ARM_ABS32";
		case R_ARM_ABS16:
			return "R_ARM_ABS16";
		case R_ARM_ABS8:
			return "R_ARM_ABS8";
		case R_ARM_CALL:
			return "R_ARM_CALL";
		case R_ARM_JUMP24:
			return "R_ARM_JUMP24";
		default:
			return "UNKNOWN";
	}
}

size_t get_file_size(FILE *file) {
	assert(file);
	fseek(file, 0, SEEK_END);
	size_t taille = ftell(file);
	rewind(file);
	return taille;
}

size_t copy_file(FILE *source, FILE *dest) {
	assert(source);
	assert(dest);
	size_t taille_lue = 0;
	size_t taille_ecrite = 0;
	const int taille_buffer = get_file_size(source);
	char buffer[taille_buffer];
	
	taille_lue = fread(buffer, 1, taille_buffer, source);
	assert(taille_lue == taille_buffer);

	taille_ecrite = fwrite(buffer, 1, taille_buffer, dest);
	assert(taille_ecrite == taille_buffer);

	assert(taille_lue == taille_ecrite);
	rewind(source);
	rewind(dest);

	return taille_lue;
}


void met_a_jour_shlink(Elf32 *elfdata) {
	assert(elfdata);
	assert(elfdata->shtable);

	Elf32_Word strtabIndex = (Elf32_Word)get_section_index(*elfdata, ".strtab");

	for (int i=0; i < get_shnum(&(elfdata->elfhdr)); i++) {
		switch (elfdata->shtable[i].sh_type) {
			case SHT_SYMTAB:
				elfdata->shtable[i].sh_link = strtabIndex;
				break;
			case SHT_SYMTAB_SHNDX:
			case SHT_REL:
				elfdata->shtable[i].sh_link = (Elf32_Word)elfdata->symtabIndex;
				break;
			default:
				break;
		}
	}
}