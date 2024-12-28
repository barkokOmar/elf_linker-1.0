#!/bin/bash


make &>/dev/null

if [[ $# -eq 2 ]]
then
	if [[ "$2" == "-?" ]]; then
		fichier="Examples_loader/example1.o"
		option="$2"
		restArgs=""
	else
		fichier="$2"
		option="-h"
		restArgs=""
	fi
elif [[ $# -ge 3 ]]
then
	fichier="$2"
	option="$3"
	restArgs="${@:4}"
else
	exit 1
fi


echo "---------------------------- Resultat Attendu:"
arm-none-eabi-readelf "$fichier" "$option" "$restArgs"

echo "---------------------------- Resultat Obtenu:"
"$0" "$fichier" "$option" "$restArgs" 


