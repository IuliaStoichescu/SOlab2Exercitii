#!/bin/bash
while read line
do echo $line |grep -E '^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/[0-9]{4}\ (0[0-9]|1[0-9]|2[0-4]):([012345][0-9]):([012345][0-9])$'
 done

