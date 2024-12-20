#include <stdio.h>
#include <assert.h>
#include "my_elf.h"

int main (int argc, char *argv[]){
	assert(argc > 1);
	
    FILE* fichier = fopen(argv[1], "rb");
    assert(fichier);

    Elf32_Ehdr entete;

    read_header(fichier, &entete);
    affiche_header(entete);

    read_shtable(fichier, &entete);

    fclose(fichier);
    return 0;
}

