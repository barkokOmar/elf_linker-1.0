#!/bin/bash


EXAMPLES=Examples_loader/*.o
for file in $EXAMPLES 
    do
    RESULTARM=$(arm-none-eabi-readelf -h $file | sed -e '14 s/, Version5 EABI//g' | sed '6,7d' | sed '7d')
    RESULT=$(./my_test_affichages $file | sed '1d' | sed '7d')
    echo "$RESULT" > result.txt
    echo "$RESULTARM" > resultarm.txt
    
    diff -w result.txt resultarm.txt
    done
