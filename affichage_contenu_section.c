#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>


/* Ce programme lit un fichier ELF 32 bits, trouve une section pas nom,
puis affiche son contenu en hexadecimal.
Parametres: <nom_section> <fichier ELF> */

static void afficher_hexa(const unsigned char *data, size_t size){

    for(size_t i = 0; i < size; i++){

        if(i % 16 == 0){
            printf("\n 0x%08lx ", (unsigned long)i);
        }
        printf("%02x", data[i]);
    }
    printf("\n");

}

int main(int argc, char **argv){

    if(argc != 3){
        fprintf(stderr, "Usage: %s <nom_section> <fichier ELF>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *nom_section = argv[1];
    const char *nom_fichier = argv[2];

    int fd = open(nom_fichier, O_RDONLY);
    
    if(fd < 0){
        perror("open");
        return EXIT_FAILURE;
    }

    struct stat st;
    
    if(fstat(fd,&st) < 0){
        perror("fstat");
        close(fd);
        return EXIT_FAILURE;
    }

    /* Projection en mémoire du fichier ELF */
    void *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if(map == MAP_FAILED){
        perror("mmap");
        return EXIT_FAILURE;
    }

    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)map;

    /* Verification ELF */
    if(memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0){
        fprintf(stderr, "Ce fichier n'est pas un fichier ELF valide.\n");
        munmap(map,st.st_size);
        return EXIT_FAILURE;
    }


    /* Verification classe (32 bits) */
    if(ehdr->e_ident[EI_CLASS] != ELFCLASS32){
        fprintf(stderr, "Ce programme ne gere pas les ELF 32 bits.\n");
        munmap(map, st.st_size);
        return EXIT_FAILURE;
    }

    Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)map + ehdr->e_shoff);
    Elf32_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
    const char *sh_strtab_p = (const char *)map + sh_strtab->sh_offset;


    /* Recherche de la section par nom */
    int section_index = -1;
    for(int i = 0; i < ehdr->e_shnum; i++){
        const char *nom = sh_strtab_p + shdr[i].sh_name;
        
        if(strcmp(nom, nom_section) == 0){
            section_index = i;
            break;
        }
    }

    if(section_index == -1){
        fprintf(stderr, "Section '%s' introuvable.\n", nom_section);
        munmap(map, st.st_size);
        return EXIT_FAILURE;
    }


    Elf32_Shdr *target_sh = &shdr[section_index];
    const unsigned char *data_section = (const unsigned char *)map + target_sh->sh_offset;
    size_t section_size = target_sh->sh_size;

    printf("Affichage de la section '%s' (index %d, offset=0x%lx, size=0x%lx) :\n", 
        sh_strtab_p + target_sh->sh_name,
        section_index,
        (unsigned long)target_sh->sh_offset,
        (unsigned long)section_size);

    afficher_hexa(data_section, section_size);

    munmap(map,st.st_size);

    return EXIT_SUCCESS;


}