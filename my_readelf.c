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
    printf("  -s        Affiche la table de symbole du fichier ELF\n");
    printf("  -r        Affiche la table de relocation du fichier ELF\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        help();
        return 1;
    }

    //Elf32_Ehdr elfhdr;
    Elf32_Ehdr *elfhdr = (Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr));
    Elf32_Shdr *shtable = NULL;
    Elf32_Sym *symtable = NULL;
    Elf32_Reltab reltab = {NULL, 0};

    char *sh_strtab = NULL;
    char *sym_strtab = NULL;

    int afficher_header = 0;
    int afficher_shtable = 0;
    int afficher_symboles = 0;
    int afficher_reltab = 0;
    int section_index = -1; 
    int symtabIndex = -1;

    // Utilisation de getopt pour gérer les options -h et -S,etc...
    int opt;
    while ((opt = getopt(argc, argv, "hSx:sr")) != -1) {
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
            case 's':  // Option pour afficher la table des sections
                afficher_symboles = 1;
                break;
            case 'r':  // Option pour afficher la table des sections
                afficher_reltab = 1;
                break;
            default:
                help();
                return 0;
        }
    }

    // Assurer qu'un fichier ELF est fourni après les options
    if (optind >= argc) {
        fprintf(stderr, "Erreur : Le fichier fourni n'est pas un fichier ELF\n");
        help();
        return 1;
    }

    FILE *fichier = fopen(argv[optind], "rb");
    if (!fichier) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", argv[optind]);
        return 1;
    }

    Elf32 elfdata = {*elfhdr, shtable, symtable, reltab, sh_strtab, sym_strtab};

    read_header(fichier, &(elfdata.elfhdr));
    if((elfdata.elfhdr).e_ident[EI_CLASS] != ELFCLASS32){
        printf("Erreur, Pas ELF32 !\n");
        return 1;
    }
    if (afficher_header)
        affiche_header(elfdata.elfhdr);

    read_shtable(fichier, &(elfdata.elfhdr), &(elfdata.shtable));
    // On lit les tables des chaines de caractères .shstrtab et .strtab
    read_strtab(fichier, &elfdata, ".shstrtab");
    read_strtab(fichier, &elfdata, ".strtab");
    if (afficher_shtable)
        affiche_shtable(&(elfdata.elfhdr), elfdata.shtable, elfdata.sh_strtab);
    

    // Afficher le contenue de la section spécifié par l'index section_index
    if ( section_index != -1 ) {
        if (section_index == 0)
            printf("Section '' has no data to dump.\n");
        else if (section_index < 0 || section_index >= get_shnum(&elfdata.elfhdr) )
            printf("readelf: Warning: Section %d was not dumped because it does not exist!.\n", section_index);
        else
            affiche_contenu_section(fichier, elfdata.shtable, elfdata.sh_strtab, section_index);
    }

    read_symtable(fichier, &(elfdata.symtable), &(elfdata.elfhdr), &(elfdata.shtable), &symtabIndex);
    if (afficher_symboles)
        affiche_symtable(elfdata, symtabIndex);

    read_reltab(fichier, &elfdata);
    if (afficher_reltab)
        affiche_reltab(elfdata);
    
    free(elfhdr);
    free(shtable);
    free(symtable);
    free(reltab.entries);
    free(sh_strtab);
    free(sym_strtab);

    fclose(fichier);

    return 0;
}
