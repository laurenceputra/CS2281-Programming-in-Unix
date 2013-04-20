#!/bin/bash
for zipfile in "$@"
do
	list=""
	matric=`basename $zipfile .zip`
	for file in `unzip -l $zipfile | grep "[A-Z][0-9]\{0,\}\-task[1-9][0-9]\{0,\}.c$" | cut -d ' ' -f 12`
	do
		list+=$file" "
	done
	echo $zipfile
	echo $list
	unzip -oj $1 $list -d $matric
	#ls $matric | grep -v "^[A-Z][0-9]\{1,\}[A-Z][\-]task[1-9][0-9]\{0,\}.c$" | xargs -n 1 rm -rf
done	