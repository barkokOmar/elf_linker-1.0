#!/bin/bash

EXAMPLES=Examples_loader/*.o
for file in $EXAMPLES
    do
    for i in {1..8} 
        do
        RESULTARM=$(arm-none-eabi-readelf -x $i $file | sed '1d' | sed '/NOTE:/d' | awk '{$1=""; print $0}' | sed 's/^ *//' )
        RESULT=$(./my_readelf -x $i $file | awk '{$1=""; print $0}' | sed 's/^ *//' )
        echo "$RESULT" > result.txt
        echo "$RESULTARM" > resultarm.txt
        
        diff -w result.txt resultarm.txt
        echo "---------------------------------"
        done
        echo " "
        echo " "
        echo " " 
    done