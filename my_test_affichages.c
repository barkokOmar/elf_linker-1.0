#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <getopt.h>
#include "my_elf.h"

// Fonction pour afficher l'aide du programme
void help() {
    printf("Usage: ./program <options> <fichier_elf>\n");
    printf("\nOptions:\n");
    printf("  -h        Affiche l'en-tête du fichier ELF\n");
    printf("  -S        Affiche la table des sections du fichier ELF\n");
    printf("  -h -S     Affiche à la fois l'en-tête et la table des sections\n");
    printf("  -?        Affiche ce message d'aide\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        help();
        return 1;
    }
    int afficher_header = 0;
    int afficher_shtable = 0;
    // Utilisation de getopt pour gérer les options -h et -S
    int opt;
    while ((opt = getopt(argc, argv, "hS?")) != -1) {
        switch (opt) {
            case 'h':  // Option pour afficher l'entete
                afficher_header = 1;
                break;
            case 'S':  // Option pour afficher la table des sections
                afficher_shtable = 1;
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
    Elf32_Ehdr entete; 
    read_header(fichier, &entete);

    // Afficher l'entete si l'option -h est activée
    if (afficher_header) {
        affiche_header(entete);
    }

    // Afficher la table des sections si l'option -S est activée
    if (afficher_shtable) {
        read_shtable(fichier, &entete);

    }

    fclose(fichier);

    return 0;
}

