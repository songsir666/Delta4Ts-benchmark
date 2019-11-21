#!/bin/bash

for file in `ls *.txt`
do
	cat $file | while read line ; do
	echo "$file $line" >> result.cvs
	done
done
