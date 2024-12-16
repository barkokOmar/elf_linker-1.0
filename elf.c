#include <stdio.h>
#include "elf.h"
#include "stdlib.h"
#include "assert.h"

Elf_header read_header(FILE* fichier){
    Elf_header entete;
    uint8_t octets[4];
    size_t taille_lue;
    //lire les 16 premier octets du magic number
    for (int i=0; i<16;i++){
        taille_lue=fread(octets,1,1,fichier);
        assert(taille_lue==1);
        entete.e_ident[i]=octets[0];
    }

    
}

int affiche_header(){
    
}

int main (int argc, char *argv[]){
    FILE* fichier= fopen(argv[1], "rb");
    if (fichier==NULL){
        printf("erreur de lecture du fichier\n");
        return FAIL;
    }
    Elf_header tete;
    tete=read_header(fichier);
    affiche_header(tete);
    return 0;
}

