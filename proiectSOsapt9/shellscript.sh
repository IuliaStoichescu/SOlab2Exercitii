#!/bin/bash

if test "$#" -ne 1
then
    echo "Not ok"
    exit -1
fi

while read line
do
    var=$(grep -E "^[A-Z][a-zA-Z0-9\ \,\$1]+[.!?]{1}$" | grep -E -v "si[\ ]*\," | grep -E -v "n(p|b)" | wc -l)
    if test "$var" -ne 0
    then
        echo "$var"
    fi
done


