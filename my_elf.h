#ifndef __my_elf__
#define __my_elf__

#include <elf.h>
#include <stdint.h>

/*--- Structures --*/

 
typedef struct {
   Elf32_Rel *rel;
   Elf32_Rela *rela;  
   int shndx;        // indice de la section a laquelle la relocation s'applique
   int relnum;       // nombre de relocations dans la table
} Elf32_RelEntry;

typedef struct {
   Elf32_RelEntry *entries;   
   int entrynum;                // number of relocation entries (taille du tableau)
} Elf32_Reltab;

typedef struct {
   Elf32_Ehdr elfhdr;
   Elf32_Shdr *shtable;
   Elf32_Sym *symtable;
   Elf32_Reltab reltab;
   char *sh_strtab;
   char *sym_strtab;
   int symtabIndex;
} Elf32;

/*--- Interfaces Lecture --*/

/* read_header
   description : lit l'entete d'un fichier ELF
   paramètres : un fichier ELF precedemment ouvert en lecture bit a bit, l'adresse de 
   				la structure entete ou on souhaite stocker le resultat de la lecture
   valeur de retour : nombre d'octets correctement lus
   effets de bord : modifie la structure entete donnee en argument de fonction
*/
size_t read_header(FILE* f, Elf32_Ehdr *elfhdr);
void read_shtable(FILE *file, Elf32_Ehdr *elfhdr, Elf32_Shdr **shtable);
size_t read_strtab(FILE *file, Elf32 *elfdata, const char *section_name);
void read_symtable(FILE *file, Elf32_Sym **symtable, Elf32_Ehdr *elfhdr, Elf32_Shdr **shtable, int *symtabIndex);
void read_reltab(FILE *file, Elf32 *elfdata);

/*--- Interfaces Affichage --*/

/* affiche_header
   description : affiche l'entete donnee
   paramètres : l'entete que l'on souhaite afficher
   valeur de retour : aucune
   effets de bord : affiche sur la sortie standard
*/
void affiche_header(Elf32_Ehdr entete);
void affiche_shtable(Elf32_Ehdr *elfhdr, Elf32_Shdr *shtable, char *shstrtab_data);
void affiche_contenu_section(FILE *file, Elf32_Shdr *shtable, char *shstrtab_data, int sectionIndex);
void affiche_symtable(Elf32 elfdata);
int affiche_reltab(Elf32 elfdata);

/*--- Interfaces Phase2 --*/

/* supprime_sh
   description : supprime la section d'indice donnee du fichier ELF
   paramètres : un fichier ELF ouvert en ecriture, la structure Elf32 contenant les donnees
   				du fichier, l'indice de la section a supprimer
   valeur de retour : 0 si tout s'est bien passe, -1 sinon
   effets de bord : modifie le fichier ELF et la structure Elf32
*/
int supprime_sh(FILE *file, Elf32 *elfdata, int index, Elf32_Addr addr_text, Elf32_Addr addr_data);

/* supprime_relsh
   description : supprime les sections de type SHT_REL du fichier ELF
   paramètres : un fichier ELF ouvert en ecriture, la structure Elf32 contenant les donnees
   				du fichier
   valeur de retour : 0 si tout s'est bien passe, -1 sinon
   effets de bord : modifie le fichier ELF et la structure Elf32
*/
int supprime_rel_sections(FILE *source_stream, FILE *dest_stream, Elf32 *elfdata,  Elf32_Addr addr_text, Elf32_Addr addr_data);

/*--- Autre --*/

/* swap_endianess_XXX
	description : inverse l'endianess des valeurs de la structure donnee
	paramètres : un pointeur vers la structure a modifier
	valeur de retour : aucune
	effet de bord : modifie la structure	
*/
void swap_endianess_elfhdr(Elf32_Ehdr *entete);
void swap_endianess_shtable(Elf32_Shdr *shtable);
void swap_endianess_symtable(Elf32_Sym *symtable);
void swap_endianess_elfdata(Elf32 *elfdata);


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

int is_symatble(Elf32_Shdr *shtable, int index);
const char* get_st_shndx(Elf32_Half st_shndx);
const char* get_st_visibility(unsigned char st_other);

const char* get_symtype(unsigned char st_info);
const char* get_symbind(unsigned char st_info);
const char* get_section_name(Elf32 elfdata, int index);
const char *get_reloc_type(Elf32_Word type);

/* find_shstrtab_index: renvoie l'indice de la table des strings des sections (pas vraiment équiv à entente.e_shstrndx) */
int find_section_index(Elf32 elfdata, const char *section_name);
/* get_file_size: renvoie la taille en octets de file ouvert en lecture ou ecriture */
size_t get_file_size(FILE *file);
/* copy_file: copie le fichier source dans dest et renvoie le nombre d'octets écrites correctement */
size_t copy_file(FILE *source, FILE *dest);
/* update_sym_shndx: remplace les symboles qui on un numéros de section old_shndx par new_shndx dans la  table des symboles */
int update_sym_shndx(Elf32 *elfdata, int old_shndx, int new_shndx);

#endif
