# /bin/sh

for arg in "$@"
do
	list=""
	#chop whatever is in front of . to get the file extension
	if [[ ${arg##*.} = "zip" ]]
	then
		#create new directory in CWD
		mkdir ${arg%%.zip}
		#check what files in the zip, extract files that match regex
		for f in $(unzip -l $arg | tr -s ' ' '\n' | egrep [A-Za-z0-9]+-task[1-9]+.c | cut -d ' ' -f 3) 
		do
			#append each matched file to $list
			list+=$f" "
		done
	fi
	echo $list
	#extract only files in $list
	unzip -oq $arg $list -d ${arg%%.zip}
done
