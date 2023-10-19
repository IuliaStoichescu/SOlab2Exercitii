#!/bin/bash

dir=$1
out=$2
cnt=0
total=0

for arg in "$dir"/*.txt
do
    cnt=$( wc -m < "$arg" )
    echo "$arg" "$cnt" >> "$2"
    total=$((total + cnt))
done 

echo "TOTAL : $total" >> "$2"
