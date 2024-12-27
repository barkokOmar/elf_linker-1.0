#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>   // pour isdigit()
#include "my_elf.h"



// Fonction pour afficher l'aide du programme
void help() {
    printf("Usage: ./program <options> <fichier_elf>\n");
    printf("\nOptions:\n");
    printf("  -h        Affiche l'en-tête du fichier ELF\n");
    printf("  -S        Affiche la table des sections du fichier ELF\n");
    printf("  -x <i>    Affiche le contenu de la section spécifiée par l'index i\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        help();
        return 1;
    }
    Elf32_Ehdr entete;
    Elf32_Shdr *shtable = NULL;
    char *shstrtab_data = NULL;

    int afficher_header = 0;
    int afficher_shtable = 0;
    int section_index = -1; 

    // Utilisation de getopt pour gérer les options -h et -S,etc...
    int opt;
    while ((opt = getopt(argc, argv, "hSx:")) != -1) {
        switch (opt) {
            case 'h':  // Option pour afficher l'entete
                afficher_header = 1;
                break;
            case 'S':  // Option pour afficher la table des sections
                afficher_shtable = 1;
                break;
            case 'x':  // Option pour afficher le contenu de la section
                // Vérification si l'argument passé est un entier
                if (optarg != NULL){
                        section_index = atoi(optarg);  // Convertir l'argument en entier
                }
                break;
            default:
                help();
                return 0;
        }
    }
   // Assurer qu'un fichier ELF est fourni après les options
    if (optind >= argc) {
        fprintf(stderr, "Erreur : Le fichier fourni n'est pas un fichier elf \n");
        help();
        return 1;
    }

    FILE *fichier = fopen(argv[optind], "rb");
    if (!fichier) {
        perror("Erreur d'ouverture du fichier ELF");
        return 1;
    }

    read_header(fichier, &entete);
    // Afficher l'entete si l'option -h est activée
    if (afficher_header)
        affiche_header(entete);


    read_shtable(fichier, &entete, &shtable, &shstrtab_data);
    // Afficher la table des sections si l'option -S est activée
    if (afficher_shtable)
        affiche_shtable(&entete, shtable, shstrtab_data);
    
    // Afficher le contenue de la section spécifié par l'index section_index
    if ( section_index != -1 ) {
        if (section_index == 0)
            printf("Section '' has no data to dump.\n");
        else if (section_index < 0 || section_index >= get_shnum(&entete) )
            printf("readelf: Warning: Section %d was not dumped because it does not exist!.\n",section_index);
        else
            affiche_contenu_section(fichier, shtable, shstrtab_data, section_index);
    }
    
    free(shtable);
    fclose(fichier);

    return 0;
}
