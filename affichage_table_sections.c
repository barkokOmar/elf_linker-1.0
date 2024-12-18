#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> // Pour strcmp


int main(int argc, char **argv){

    if(argc != 2){
        fprintf(stderr, "Usage: %s <fichiers_elf>\n", argv[0]);
        exit(1);
    }

    /* Ouvrir le fichier ELF */
    int fd = open(argv[1], O_RDONLY);

    if(fd == -1){
        perror("Erreur lors de l'ouverture du fichier");    
        exit(1);
    }


    /* Lire l'entête ELF */
    Elf32_Ehdr header;
    if(read(fd,&header, sizeof(header)) != sizeof(header)){
        perror("Erreur lors de la lecture de l'entête");
        exit(1);
    }


    /* Vérifier si c'est un fichier ELF 32 bits */
    if(header.e_ident[EI_CLASS] != ELFCLASS32){
        fprintf(stderr, "Erreur : Le fichier n'est pas un ELF 32 bits.\n");
        exit(1);
    }

    /* Allouer la mémoire pour la table des sections */
    Elf32_Shdr *section_headers = malloc(header.e_shnum * header.e_shentsize);
    if(section_headers == NULL){
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    /* Se positionner au début de la tabla des sections */
    lseek(fd, header.e_shoff, SEEK_SET);

    /* Lire la table des sections */
    if(read(fd, section_headers, header.e_shnum * header.e_shentsize) != header.e_shnum * header.e_shentsize){
        perror("Erreur lors de la lecture de la table des sections");
        exit(1);
    }

    /* Obtenir la table des chaine de noms des sections */
    lseek(fd, section_headers[header.e_shstrndx].sh_offset, SEEK_SET);
    char *section_names = malloc(section_headers[header.e_shstrndx].sh_size);
    
    if(section_names == NULL){
        perror("Erreur d'allocation de mémoire");
    }


    if(read(fd, section_names, section_headers[header.e_shstrndx].sh_size) != section_headers[header.e_shstrndx].sh_size){
        perror("Erreur lors de la lecture de la table des noms des sections");
        exit(1);
    }


    /* Afficher la table des sections */
    printf("Table des sections:\n");
    printf("[NR]  Nom            Type            Adresse            Offset            Taille            Entsize            Flags            Lien                Info            Alignement\n");
    
    for(int i = 0; i < header.e_shnum; i++){
        Elf32_Shdr section = section_headers[i];
        printf("[%1d]  %-16s  %-16d  %-16d  %-16ld  %-16ld  %-16ld  %-16ld  %-16d  %-16d  %d\n",
        i,
        section_names + section.sh_name,
        section.sh_type,
        section.sh_addr,
        (unsigned long)section.sh_offset,
        (unsigned long)section.sh_size,
        (unsigned long)section.sh_entsize,
        (unsigned long)section.sh_flags,
        section.sh_link,
        section.sh_info,
        section.sh_addralign);

    }

    free(section_headers);

    close(fd);
    
    return 0;
}