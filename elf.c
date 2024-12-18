#include <stdio.h>
#include <stdint.h>
#include "elf.h"

void affiche_en_tete_elf(FILE *file){
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
        fread(&header_elf.e_type,1,1,file);
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
