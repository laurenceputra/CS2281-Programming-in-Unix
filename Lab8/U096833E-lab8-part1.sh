#!/bin/bash
for file in "$@"
do
	matric=`basename $file .zip`
	unzip -j $1 "[A-Z][0-9]*[A-Z][\-]task[1-9][0-9]*.c" -d $matric
	#ls $matric | grep -v "^[A-Z][0-9]\{1,\}[A-Z][\-]task[1-9][0-9]\{0,\}.c$" | xargs -n 1 rm -rf
done	