# Name: Leon Ho Jian Yang
# Matric: U096828R
# Email: leonho@nus.edu.sg

# /bin/sh

for dir in *
do
	#for everything in CWD, check that its a directory
	if [[ -d $dir ]]
	then
		#check every C file for the comment, make sure it contains the directory's matric
		for file in $dir/*.c
		do
			if [[ ! $file =~ $dir-task[1-9]+.c ]]
			then
				echo $file
			fi
		done
	fi
done

