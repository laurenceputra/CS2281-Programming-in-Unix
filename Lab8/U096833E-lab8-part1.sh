# Name: Laurence Putra
# Matric: U096833E
# Email: laurenceputra@gmail.com
#!/bin/bash
for zipfile in "$@"
do
	list=""
	matric=`basename $zipfile .zip`
	for file in `unzip -l $zipfile | tr -s ' ' '\n' | grep "[A-Z][0-9]\{0,\}\-task[1-9][0-9]\{0,\}.c$"`
	do
		list+=$file" "
	done
	unzip -ojq $1 $list -d $matric
done	