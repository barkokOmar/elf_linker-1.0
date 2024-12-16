#include <stdio.h>
#include <elf.h>
#include "stdlib.h"
#include "assert.h"

#define SUCCES 1
#define FAIL 2
#define MEMORYFAIL 4

Elf32_Ehdr read_header(FILE* fichier){
    Elf32_Ehdr entete;
    size_t taille_lue;

    // lire les 16 premier octets pour elf identification
    taille_lue = fread(entete.e_ident, 1, 16, fichier);
    assert(taille_lue == 16);

    // lire 2 cases de 2 octets
    fread(&entete.e_type, 1, 2, fichier);
    fread(&entete.e_machine, 1, 2, fichier);
    
    // lire les cases à 4 octets
    fread(&entete.e_version, 1, 4, fichier);
    fread(&entete.e_entry, 1, 4, fichier);
    fread(&entete.e_phoff, 1, 4, fichier);
    
    fread(&entete.e_shoff, 1, 4, fichier);
    fread(&entete.e_flags, 1, 4, fichier);


    // lire le reste des cases à 2 octets
    fread(&entete.e_ehsize, 1, 2, fichier);
    fread(&entete.e_phentsize, 1, 2, fichier);
    fread(&entete.e_phnum, 1, 2, fichier);
    fread(&entete.e_shentsize, 1, 2, fichier);
    fread(&entete.e_shnum, 1, 2, fichier);
    fread(&entete.e_shstrndx, 1, 2, fichier);

    return entete;
    
}

int affiche_header(Elf32_Ehdr entete){
    printf("ELF header:\n");
    printf("  Magic:    ");
    for(int i=0;i<16;i++){
        printf("%x ",entete.e_ident[i]);
    }
    printf("\n");

     //---------------EI_CLASS----------
    printf("  Class:\t\t\t\t\t");
    if(entete.e_ident[4]==ELFCLASS32){
        printf("ELF32\n");
    }
    else {
        printf("pas elf_32\n");
    }

    //---------------EI_DATA----------
    printf("  Data:\t\t\t\t\t");
    if(entete.e_ident[5]==ELFDATA2LSB){
        printf("2's complement, little endian\n");
    }
    else if(entete.e_ident[5]==ELFDATA2MSB)  {
        printf("2's complement, big endian\n");
    }
    else {
        printf("\n");
    }
     //---------------EI_VERSION----------
    printf("  Version:\t\t\t\t%x\n",entete.e_ident[6]);

    //----------------e_type----------
    printf("  Type:\t\t\t\t ");
    if(entete.e_type==ET_REL){
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

