#!/bin/bash
directories=`ls -d */`
check=`echo $dir | grep "such file or directory" | wc -l`
if [ $check -eq 0 ]
	then
	for dir in $directories
	do
		files=`ls $dir`
		check=`echo $files | wc -l`
		matric=`basename $dir`
		if [ $check -ne 0 ]
			then
			for file in $files
			do
				check=`grep "^/* Matric: $matric \*\/$" $dir$file | wc -l`
				if [ $check -eq 0 ]
					then
					echo "$file"
				fi
			done
		fi
	done
fi