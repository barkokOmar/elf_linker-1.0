#include <stdio.h>
#include "elf.h"
int main(int argc,char *argv[]){
	FILE *f;
	f=fopen(argv[1],"rb");
	affiche_en_tete_elf(f);
	return 0;
}
