/* Matric: U096833E */
 /* Name : Laurence Putra
 * Matric : U096833E
 * Email : laurenceputra@gmail.com
***************************/

#include <stdio.h>
#include <procfs.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
int main(int argc, char *argv[]){
	DIR *proc = opendir("/proc");
	if(proc == NULL){
		printf("Unable to open /proc directory\n");
		return 1;
	}
	struct dirent *file;
	char filename[300];
	FILE *procFile;
	psinfo_t procInfo;
	struct passwd *userInfo;
	printf("     UID   PID            FNAME STIME\n");
	while((file = readdir(proc)) != NULL){
		//process only actual files
		if(file->d_name[0] != '.'){
			sprintf(filename, "/proc/%s/psinfo", file->d_name);
			procFile = fopen(filename, "r");
			//process only files you can open
			if(procFile != NULL){
				fread(&procInfo, sizeof(psinfo_t), 1, procFile);
				userInfo = getpwuid(procInfo.pr_uid);
				printf("%8s %5d %16s %.24s\n", userInfo->pw_name, procInfo.pr_pid, procInfo.pr_fname, ctime((time_t *)&procInfo.pr_start.tv_sec));
			}
		}
	}


}