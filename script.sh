#!/bin/bash
#ex1
if test $# -gt 3
  then echo "e ok"
else "Prea putine argumente"
fi

#ex2+3
count=0
sum=0
file=$1
dir=$2
shift 2
for arg
do
    if test "$arg" -gt 10
    then $((count++))
    fi
    sum=`expr $sum + $arg`
done

#ex2
echo "Nr de argm mai mari ca 10 este : $count"
#ex3
echo "Suma este: $sum"

#ex4
echo $sum|wc -L

#ex5
if test -f "$file"
then echo "$count $sum"
fi >file.txt

#ex6
for file in "$dir"/*.txt
do
    if test -f "$file"
    then 
	 cat "$file"
    fi
done

  
