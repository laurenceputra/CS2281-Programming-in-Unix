# /bin/sh


for dir in *
do
	#for everything in CWD, check that its a directory
	if [[ -d $dir ]]
	then
		#check every C file for the comment, make sure it contains the directory's matric
		for file in $dir/*.c
		do
		!	egrep -s '\/\*.*Matric:.*'$dir'.*\*\/' $file && echo $file
		done
	fi
done

