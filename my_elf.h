#ifndef __mon_elf__
#define __mon_elf__

#include <elf.h>
#include <stdint.h>


typedef enum {
	FAIL = 0,
	SUCCESS = 1
} erreur_t ;


/*--- Interfaces Lecture --*/

/*
   read_header
   description : lit l'entete d'un fichier ELF
   paramètres : un fichier ELF precedemment ouvert en lecture bit a bit, l'adresse de 
   				la structure entete ou on souhaite stocker le resultat de la lecture
   valeur de retour : SUCCESS si tout se passe bien, FAIL sinon
   effets de bord : modifie la structure entete donnee en argument de fonction
*/
erreur_t read_header(FILE* f, Elf32_Ehdr *entete);


/*--- Interfaces Affichage --*/

/*
   affiche_en_tete_elf
   description : affiche l'entete donnee
   paramètres : l'entete que l'on souhaite afficher
   valeur de retour : aucune
   effets de bord : affiche sur la sortie standard
*/
void affiche_header(Elf32_Ehdr entete);


/*--- Autre --*/

/*
	swap_endian
	description : inverse l'endianess des valeurs de l'entete donnee
	paramètres : entete
	valeur de retour : aucune
	effet de bord : modifie la structure	
*/
void swap_endian(Elf32_Ehdr *entete);


/* On pourra les implémenter éventuellement si pertinents...
unsigned char* get_indent(Elf32_Ehdr entete);
Elf32_Half get_type(Elf32_Ehdr entete);
Elf32_Half get_machine(Elf32_Ehdr entete);
Elf32_Word get_version(Elf32_Ehdr entete);
Elf32_Addr get_entry(Elf32_Ehdr entete);
Elf32_Off get_phoff(Elf32_Ehdr entete);
Elf32_Off get_shoff(Elf32_Ehdr entete);
Elf32_Word get_flags(Elf32_Ehdr entete);
Elf32_Half get_ehsize(Elf32_Ehdr entete);
Elf32_Half get_phentsize(Elf32_Ehdr entete);
Elf32_Half get_phnum(Elf32_Ehdr entete);
Elf32_Half get_shentsize(Elf32_Ehdr entete);
Elf32_Half get_shnum(Elf32_Ehdr entete);
Elf32_Half get_shstrndx(Elf32_Ehdr entete);
*/

Elf32_Shdr read_section_headers(FILE *file, Elf32_Ehdr *header);

void read_section_names(FILE *file, Elf32_Ehdr *header, Elf32_Shdr *section_headers);

#endif