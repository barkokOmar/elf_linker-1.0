#!/bin/bash

if [[ $# -lt 1 ]]; then
	example="Examples_loader/example1"
else
	example="$1"
fi

make 
echo "============================= Resultat Attendu:"
arm-none-eabi-readelf -h "$example"

echo "============================= Resultat Obtenu:"
./my_test_affichages "$example" 


