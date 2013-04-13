#!/bin/bash
for (( c=0; c<$#; c++))
do
	matric=`basename $1 .zip`
	unzip $1 -d $matric
	ls $matric | grep -v -e "^[A-Z][0-9]\{1,\}[A-Z][\-][task]" | xargs -n 1 echo
done	