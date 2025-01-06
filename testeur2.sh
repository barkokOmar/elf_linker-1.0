#!/bin/bash

EXAMPLES=Examples_loader/*.o
for file in $EXAMPLES
    do
    RESULTARM=$(arm-none-eabi-readelf -S $file | sed '5d' | sed '/Key to Flags:/,$d' | awk -F' ' '{print $1,$2,$3,$4,$5,$6,$7}' | sed -e '4 s/ES//g')
    RESULT=$(./my_test_affichages -S $file | sed '5d' | awk -F' ' '{print $1,$2,$3,$4,$5,$6,$7}')
    echo "$RESULT" > result.txt
    echo "$RESULTARM" > resultarm.txt
   
    diff -w result.txt resultarm.txt
    done