#!/bin/bash

dir=$1
fis=$2
count=0
count_rec=0

if test "$#" -ne 2
   then echo "Not ok"
fi


for arg in "$dir"/*
do
    if test -h $arg
    then
	count=`expr $count + 1`
	 
    elif test -f "$arg"
    then
	var=`cat "$arg"|grep -E "^[A-Z][a-zA-Z0-9\ \,]+[^\.]\.{1}$"|grep -E -v "si[\ ]*\,"|grep -E -v "n(p|b)"|wc -l`
	if test "$var" -ne 0
	then echo "$arg - $var" >> "$2"
	fi

    elif test -d "$dir"
    then count_rec=`bash $0 $arg $fis`
	 count=` $count + $count_rec`
    fi   
done

echo "$count"


