#!/bin/bash

EXAMPLES=Examples_loader/*.o
for file in $EXAMPLES
    do
    RESULTARM=$(arm-none-eabi-readelf -s $file | sed '1d' | sed '/UND/d' | awk 'NR==1 || NF<8 {print $0} NR>1 && NF>=8 {$8=""; print $0}' | sed 's/^ *//')
    RESULT=$(./my_readelf -s $file | sed '3d' | awk 'NR==1 || NF<8 {print $0} NR>1 && NF>=8 {$8=""; print $0}' | sed 's/^ *//')
    echo "$RESULT" > result.txt
    echo "$RESULTARM" > resultarm.txt

    diff -w result.txt resultarm.txt
    echo "------------"
    done