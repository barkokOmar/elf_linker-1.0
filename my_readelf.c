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
    printf("  -R <dest> Copie dans dest fichier_elf en suupprimant les sections contenant des tables de réimplantations\n");
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
    FILE *fichier_elf = NULL;
    FILE *fichier_dest = NULL;
    Elf32_Word section_type;
    
    char *endptr = NULL;
    char *sh_strtab = NULL;
    char *sym_strtab = NULL;
    const char *section_name = NULL;

    int afficher_header = 0;
    int afficher_shtable = 0;
    int afficher_symboles = 0;
    int x_opt = 0;
    int afficher_reltab = 0;
    int section_index = -1; 
    int symtabIndex = -1;
    int supprime_rel = 0;
    int affiche_section_par_str = 0;
    // Utilisation de getopt pour gérer les options -h et -S,etc...
    int opt;
    while ((opt = getopt(argc, argv, "hSx:srR:")) != -1) {
        switch (opt) {
            case 'h':  // Option pour afficher l'entete
                afficher_header = 1;
                break;
            case 'S':  // Option pour afficher la table des sections
                afficher_shtable = 1;
                break;
            case 'x':  // Option pour afficher le contenu de la section
                if (optarg != NULL) {
                    x_opt = 1;
                    section_index = (int)strtol(optarg, &endptr, 10);  // Convertir l'argument en entier
                    if (*endptr != '\0' || *optarg == '\0') {
                        // fprintf(stderr, "Erreur : L'index de section doit être un entier\n");
                        // help();
                        // return 1;
                        affiche_section_par_str = 1;
                        section_name = optarg;
                    }
                }
                break;
            case 's':  // Option pour afficher la table des sections
                afficher_symboles = 1;
                break;
            case 'r':  // Option pour afficher la table des sections
                afficher_reltab = 1;
                break;
            case 'R':  // Option pour supprimer les sections de type SHT_REL
                supprime_rel = 1;
                if (optarg != NULL) {
                    fichier_dest = fopen(optarg, "wb");
                    if (!fichier_dest) {
                        fprintf(stderr, "Erreur durant l'ouverture du fichier %s en écriture\n", optarg);
                        return 1;
                    }
                }
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

    fichier_elf = fopen(argv[optind], "rb");
    if (!fichier_elf) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s en lecture\n", argv[optind]);
        return 1;
    }

    Elf32 elfdata = {*elfhdr, shtable, symtable, reltab, sh_strtab, sym_strtab, symtabIndex};

    // lecture de l'en-tête du fichier ELF
    read_header(fichier_elf, &(elfdata.elfhdr));
    if((elfdata.elfhdr).e_ident[EI_CLASS] != ELFCLASS32){
        printf("Erreur, Pas ELF32 !\n");
        return 1;
    }
    if (afficher_header)
        affiche_header(elfdata.elfhdr);

    // lecture de la table des sections
    read_shtable(fichier_elf, &(elfdata.elfhdr), &(elfdata.shtable));
    // On lit les tables des chaines de caractères .shstrtab et .strtab
    read_strtab(fichier_elf, &elfdata, ".shstrtab");
    read_strtab(fichier_elf, &elfdata, ".strtab");
    if (afficher_shtable)
        affiche_shtable(&(elfdata.elfhdr), elfdata.shtable, elfdata.sh_strtab);
    
    // Afficher le contenue de la section spécifié par l'index section_index
    if (x_opt) {
        if (affiche_section_par_str)   // Afficher le contenue de la section spécifié par le nom de la section 
            section_index = get_section_index(elfdata, section_name);
        if (section_index < 0 || section_index >= get_shnum(&elfdata.elfhdr)){
            if (affiche_section_par_str)
                printf("readelf: Warning: Section %s was not dumped because it does not exist!.\n", section_name);
            else 
                printf("readelf: Warning: Section %d was not dumped because it does not exist!.\n", section_index);
        }
        else {
            section_type = elfdata.shtable[section_index].sh_type;
            if (section_type== SHT_NULL || section_type == SHT_NOBITS)
                printf("Section '' has no data to dump.\n");
            else
                affiche_contenu_section(fichier_elf, elfdata.shtable, elfdata.sh_strtab, section_index);
        }
    }
    
    // lecture de la table des symboles
    read_symtable(fichier_elf, &(elfdata.symtable), &(elfdata.elfhdr), &(elfdata.shtable), &elfdata.symtabIndex);
    if (afficher_symboles)
        affiche_symtable(elfdata);

    // lecture de la table des relocations
    read_reltab(fichier_elf, &elfdata);
    if (afficher_reltab)
        affiche_reltab(elfdata);
    
    // Supprimer les sections qui contiennent des tables de réimplantations
    if (supprime_rel) {
        copy_file(fichier_elf, fichier_dest);

        Elf32_Addr addr_text = 0x00000020;
        Elf32_Addr addr_data = 0x00002800;
        
        appliquer_relocations(fichier_elf, fichier_dest, &elfdata, addr_text, addr_data);
        /*
        */
        // int index_text = get_section_index(elfdata, ".text");
        // int index_data = get_section_index(elfdata, ".data");
        // Elf32_Addr addr_text = elfdata.shtable[index_text].sh_addr;
        // Elf32_Addr addr_data = elfdata.shtable[index_data].sh_addr;
        corriger_symboles(fichier_elf, fichier_dest, &elfdata, addr_text, addr_data);

        supprime_rel_sections(fichier_elf, fichier_dest, &elfdata);
        
        fclose(fichier_dest);
    }




    free(elfhdr);
    free(shtable);
    free(symtable);
    free(reltab.entries);
    free(sh_strtab);
    free(sym_strtab);

    fclose(fichier_elf);

    return 0;
}
