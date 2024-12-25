#ifndef __my_elf__
#define __my_elf__

#include <elf.h>
#include <stdint.h>




/*--- Interfaces Lecture --*/

/*
   read_header
   description : lit l'entete d'un fichier ELF
   paramètres : un fichier ELF precedemment ouvert en lecture bit a bit, l'adresse de 
   				la structure entete ou on souhaite stocker le resultat de la lecture
   valeur de retour : nombre d'octets correctement lus
   effets de bord : modifie la structure entete donnee en argument de fonction
*/
int read_header(FILE* f, Elf32_Ehdr *entete);

/* read_shtable : reads a section headers table */
void read_shtable(FILE *file, Elf32_Ehdr *elfhdr, Elf32_Shdr *shtable, char **shstrtab_data);

void read_shnames(FILE *file, Elf32_Ehdr *header, Elf32_Shdr *section_headers,char **shstrtab_data);


/*--- Interfaces Affichage --*/

/*
   affiche_header
   description : affiche l'entete donnee
   paramètres : l'entete que l'on souhaite afficher
   valeur de retour : aucune
   effets de bord : affiche sur la sortie standard
*/
void affiche_header(Elf32_Ehdr entete);
void affiche_shtable(Elf32_Ehdr *elfhdr, Elf32_Shdr *shtable, char *shstrtab_data);

/*--- Autre --*/

/*
	swap_endianess
	description : inverse l'endianess des valeurs de l'entete 
	paramètres : entete
	valeur de retour : aucune
	effet de bord : modifie la structure	
*/
void swap_endianess(Elf32_Ehdr *entete);
void swap_endianess_section_table(Elf32_Shdr *shtable);


Elf32_Half get_type(Elf32_Ehdr *entete);
Elf32_Half get_machine(Elf32_Ehdr *entete);
Elf32_Word get_version(Elf32_Ehdr *entete);
Elf32_Addr get_entry(Elf32_Ehdr *entete);
Elf32_Off get_phoff(Elf32_Ehdr *entete);
Elf32_Off get_shoff(Elf32_Ehdr *entete);
Elf32_Word get_flags(Elf32_Ehdr *entete);
Elf32_Half get_ehsize(Elf32_Ehdr *entete);
Elf32_Half get_phentsize(Elf32_Ehdr *entete);
Elf32_Half get_phnum(Elf32_Ehdr *entete);
Elf32_Half get_shentsize(Elf32_Ehdr *entete);
Elf32_Half get_shnum(Elf32_Ehdr *entete);
Elf32_Half get_shstrndx(Elf32_Ehdr *entete);


#endif