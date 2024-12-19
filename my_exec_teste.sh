#!/bin/bash

example=$1

make 
echo "============================= Resultat Attendu:"
arm-none-eabi-readelf -h $example 
echo "============================= Resultat Obtenu:"
./my_test_affichages $example
