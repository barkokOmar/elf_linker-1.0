#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]){
/*Ouvrir le fichier ELF*/
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

    /* Afficher les informations de l'entête */
    printf("Plateforme cible: ");
    switch (header.e_machine){
        case EM_386: printf("x86\n"); break;
        case EM_ARM: printf("ARM\n"); break;
        default: printf("Inconnu\n"); 
    }
    
    printf("Taille des mots : %d bits\n", (header.e_ident[EI_CLASS]==ELFCLASS32) ? 32 : 64);

    printf("Type de fichier ELF : ");

    switch (header.e_type){
        case ET_REL: printf("Relocatable\n"); break;
        case ET_EXEC: printf("Executable\n"); break;
        case ET_DYN: printf("Shared object\n"); break;
        default: printf("Inconnu\n");
    }
    
    printf("Table des sections : \n");
    printf("Offset : %d\n", header.e_shoff);
    printf("Taille globale : %d\n", header.e_shnum * header.e_shentsize);
    printf("Nombre d'entrees : %d\n", header.e_shnum);

    printf("Index de la table des chaines de noms de sections : %d\n", header.e_shstrndx);

    printf("Taille de l'entête : %d\n", header.e_ehsize);

    close(fd);

    return 0;
}