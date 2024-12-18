#ifndef __mon_elf__
#define __mon_elf__

#include <stdint.h>


//-------- MACROS --------//

/* TYPES */

#define	Elf32_Addr uint32_t*	//unsigned program address
#define	Elf32_Half uint16_t		//unsigned medium integer
#define	Elf32_Off uint32_t		//unsigned file offset 
#define	Elf32_Sword int32_t		//signed large integer
#define	Elf32_Word uint32_t		//unsigned large integer

/* INDEXES */

#define EI_MAG0 0 				//File identification
#define EI_MAG1 1 				//File identification
#define EI_MAG2 2 				//File identification
#define EI_MAG3 3 				//File identification
#define EI_CLASS 4 				//File class
#define EI_DATA 5 				//Data encoding
#define EI_VERSION 6 			//File version
#define EI_PAD 7 				//Start of padding bytes
#define EI_NIDENT 16 			//Size of e_ident[]

/* VALUES */

#define ET_NONE 0 				//No file type
#define ET_REL 1 				//Relocatable file
#define ET_EXEC 2 				//Executable file
#define ET_DYN 3 				//Shared object file
#define ET_CORE 4 				//Core file
#define ET_LOPROC 0xff00 		//Processor-specific
#define ET_HIPROC 0xffff 		//Processor-specific

#define EM_M32 1 				//AT&T WE 32100
#define EM_SPARC 2 				//SPARC
#define EM_386 3 				//Intel Architecture
#define EM_68K 4 				//Motorola 68000
#define EM_88K 5 				//Motorola 88000
#define EM_860 7 				//Intel 80860
#define EM_MIPS 8 				//MIPS RS3000 Big-Endian
#define EM_MIPS_RS4_BE 10 		//MIPS RS4000 Big-Endian

#define EV_NONE 0 				//Invalid versionn
#define EV_CURRENT 1 			//Current version

#define ELFMAG0 0x7f 			//e_ident[EI_MAG0]
#define ELFMAG1 'E' 			//e_ident[EI_MAG1]
#define ELFMAG2 'L' 			//e_ident[EI_MAG2]
#define ELFMAG3 'F' 			//e_ident[EI_MAG3]
#define ELFCLASSNONE 0 			//Invalid class
#define ELFCLASS32 1 			//32-bit objects
#define ELFCLASS64 2 			//64-bit objects
#define ELFDATANONE 0 			//Invalid data encoding
#define ELFDATA2LSB 1 			//Litte endian
#define ELFDATA2MSB 2			//Big endian



//-----------------------//

typedef struct {
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
} Elf32_Ehdr;



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




/*--- Interfaces Acces --*/


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




/*--- Interfaces Affichage --*/

/*
   affiche_en_tete_elf
   description : affiche l'entete donnee
   paramètres : l'entete que l'on souhaite afficher
   valeur de retour : aucune
   effets de bord : affiche sur la sortie standard
*/
void affiche_header(Elf32_Ehdr entete);



#endif
