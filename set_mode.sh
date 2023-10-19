#!/bin/bash

dir=$1
c=$2

for entry in "$dir"/*.txt
do
 chmod +"$c" "$entry"
  echo "$entry"
done

for entry in "$dir"/*
do
    if test -d "$entry"
  then bash "$0" "$entry" "$2"
    fi
    done
    
