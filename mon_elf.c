#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mon_elf.h"

#define MEMORYFAIL 4

erreur_t read_header(FILE *fichier, Elf32_Ehdr *entete) {
	assert(fichier);	//le fichier doit etre ouvert en lecture bit a bit
	assert(entete);		//l'entete doit etre un pointeur valide

    size_t taille_lue;	// utilisee pour verifier le nombre d'octets lues

    // lire les 16 premier octets pour elf identification
    taille_lue = fread(entete->e_ident, sizeof(unsigned char), EI_NIDENT, fichier);
    assert(sizeof(unsigned char)*EI_NIDENET == taille_lue);

    // lire e_type
    taille_lue = fread(entete->e_type, sizeof(Elf32_Half), 1, fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	// lire e_machine
    taille_lue = fread(entete->e_machine, sizeof(Elf32_Half), 1, fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

    // lire e_version
    taille_lue = fread(entete->e_version, sizeof(Elf32_Word), 1, fichier);
	assert(sizeof(Elf32_Word) == taille_lue);

	// lire e_entry
    taille_lue = fread(entete->e_entry, sizeof(Elf32_Addr), 1, fichier);
	assert(sizeof(Elf32_Addr) == taille_lue);
	
	// lire e_phoff
    taille_lue = fread(entete->e_phoff, sizeof(Elf32_Off), 1, fichier);
	assert(sizeof(Elf32_Off) == taille_lue);
	
    // lire e_shoff 
    taille_lue = fread(entete->e_shoff, sizeof(Elf32_Off), 1, fichier);
	assert(sizeof(Elf32_Off) == taille_lue);

	// lire e_flags
    taille_lue = fread(entete->e_flags, sizeof(Elf32_Word), 1, fichier);
	assert(sizeof(Elf32_Word) == taille_lue);

    // lire e_ehsize
    taille_lue = fread(entete->e_ehsize, sizeof(Elf32_Half), 1, fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	// lire e_phentisize
    taille_lue = fread(entete->e_phentsize, sizeof(Elf32_Half), 1, fichier);
	assert(sizeof(Elf32_Half) == taille_lue);
	
	// lire e_phnum
    taille_lue = fread(entete->e_phnum, sizeof(Elf32_Half), 1, fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	// lire e_shentsize
    taille_lue = fread(entete->e_shentsize, sizeof(Elf32_Half), 1, fichier);
	assert(sizeof(Elf32_Half) == taille_lue);
	
	// lire e_shnum
    taille_lue = fread(entete->e_shnum, sizeof(Elf32_Half), 1, fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	// lire e_shstrndx
    taille_lue = fread(entete->e_shstrndx, sizeof(Elf32_Half), 1, fichier);
	assert(sizeof(Elf32_Half) == taille_lue);

	return (erreur_t)SUCCESS;
}


void affiche_en_tete_elf(Elf32_Ehdr entete) {
    printf("ELF header:\n");
    printf("  Magic:    ");
    for(int i=0; i < EI_NIDENT; i++) {
        printf("%02x ", entete.e_ident[i]);
    }
    printf("\n");

     //---------------EI_CLASS----------
    printf("  Class:\t\t\t\t\t");
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
		assert(ELFDATANONE == entete.e_indent[EI_DATA]);
        printf("Invalid data encoding\n");
    }
	
    //---------------EI_VERSION----------
    printf("  Version:\t\t\t\t%x\n", entete.e_ident[EI_VERSION]);

    //----------------e_type----------
    printf("  Type:\t\t\t\t ");
    if(entete.e_type == ET_REL){
        printf("REL (Relocatable file)\n");
    }
    else if(entete.e_type==ET_EXEC)  {
        printf("EXEC (Executable file)\n");
    }
    else {
        printf("\n");
    }
    //----------------e_machine----------

    //----------------e_version----------

    //----------------e_entry----------
    printf("  Entry point address:\t\t\t%x\n",entete.e_entry);
    printf("  Start of program headers:\t\t%d (bytes into file)\n",entete.e_phoff);
    printf("  Start of section headers:\t\t%d (bytes into file)\n",entete.e_shoff);
    printf("  Flags:\t\t\t\t0x%x (bytes into file)\n",entete.e_flags);
    printf("  Size of this header:\t\t\t%d(bytes)\n",entete.e_ehsize);
    printf("  Size of program headers:\t\t%x(bytes)\n",entete.e_phentsize);
    printf("  Number of program headers:\t\t%d\n",entete.e_phnum);
    printf("  Size of section headers:\t\t%d(bytes)\n",entete.e_shentsize);
    printf("  Number of section headers:\t\t%d\n",entete.e_shnum);
    printf("  Section header string table index:\t%d\n",entete.e_shstrndx);
    return SUCCES;
}


int main (int argc, char *argv[]){
    FILE* fichier= fopen(argv[1], "rb");
    if (fichier==NULL){
        printf("erreur de lecture du fichier\n");
        return FAIL;
    }
    Elf32_Ehdr tete;
    tete=read_header(fichier);
    affiche_header(tete);
    fclose(fichier);
    return 0;
}

=======
#include <stdint.h>
#include "elf.h"

void affiche_en_tete_elf(FILE *file) {
    /*On suppose que le fichier est deja ouvert en mode lecture binaire*/
    //int nb_entier_1,nb_octet_16,nb_octet_4,nb_bit=0;
    int nb_bit=0;
    Elf32_Ehdr header_elf;
    fread(header_elf.e_ident,16,1,file);
    /*e_ident*/
    /*1- Magic*/
    printf("Magic: ");//probleme magic peut contenir jusqua 16 octet et la en stock que 1 octet de cette valeur
    for (int i=0; i<16; i++){
        printf("%02x ",header_elf.e_ident[i]);
    }
    /*2- Class*/
    printf("\nClass: %c%c%c",  header_elf.e_ident[1],
                             header_elf.e_ident[2],
                             header_elf.e_ident[3]);
    switch (header_elf.e_ident[4]){
        case 1:
                nb_bit=32;
                printf("%d\n",nb_bit);    
                break;
        case 2:
                nb_bit=64;
                printf("%d\n",nb_bit);
                break;
        default:
                printf("Invalid class\n");
                break;
    }
    /*3- Data*/
     switch (header_elf.e_ident[5]){
        case 1:
                printf("DATA: 2's complement, little endian\n");    
                break;
        case 2:
                printf("DATA: 2's complement, big endian\n");    
                break;        
        default:
                printf("DATA: invalid data encoding\n");
                break;
    }
    /*4- Version*/
     switch (header_elf.e_ident[6]){
        case 1:
                printf("Version: 1 (current)\n");    
                break;
        
        default:
                printf("Version: 0 (invalid)\n");
                break;
    }    /*5-PAD*/
        printf("PAD: %d\n",header_elf.e_ident[7]);
        /*Type*/
        //fseek(file,16,SEEK_SET);
        taille_lue = fread(&header_elf.e_type,1,1,file);
        printf("%04x\n",header_elf.e_type);

     switch (header_elf.e_type){
        case 1:
                printf("Type: relocatable file\n");    
                break;
        case 2:
                printf("Type: executable file\n");    
                break; 
        case 3:
                printf("Type: shared object file\n");    
                break;
        case 4:
                printf("Type: core file\n");    
                break;
        case 0xff00:
                printf("Type: processor-specific\n");    
                break;
        case 0xffff:
                printf("Type: processor-specific\n");    
                break;                      
        default:
                printf("Type: No file type\n");
                break;
    }
}
>>>>>>> origin/marouane-etape1
