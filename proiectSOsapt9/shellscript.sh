#!/bin/bash

if test "$#" -ne 1
then
    echo "Not ok"
    exit -1
fi

var=0

while read line
do
    var=$(echo "$line" | grep -E "^[A-Z][a-zA-Z0-9\ \,\]+[^\.][.!?]$" | grep -E -v "si[\ ]*\," | grep -E -v "n(p|b)" | grep -E "$1"| wc -l)
    if test "$var" -ne 0
    then
	echo "$var"
    fi
done
