#!/bin/bash

#
# testeur de l'etape 5 compare les resultats de my_readelf -r avec ceux de arm-none-eabi-readelf
# renvoi rien si pas de difference donc succes sinon renvoi les lignes differentes
#


EXAMPLES=Examples_loader/*.o
for file in $EXAMPLES
    do
    RESULTARM=$(arm-none-eabi-readelf -r $file | sed '/There are no relocations in this file/d' | sed '1d' | sed '13d' )
    RESULT=$(./my_readelf -r $file )
    echo "$RESULT" > result.txt
    echo "$RESULTARM" > resultarm.txt
    diff -w result.txt resultarm.txt
    echo "------------"
    done