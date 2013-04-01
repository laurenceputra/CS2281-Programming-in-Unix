/***************************
 * Name : Laurence Putra
 * Matric : U096833E
***************************/

#include <stdio.h>
#include <procfs.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>

int checkIfUidExist(int newUid, int *uid, int numUid);

int main(int argc, char *argv[]){
	int optPid = 0, optUid = 0, optN = 0, option;
	while((option = getopt(argc, argv, "pnu")) != -1){
		switch(option){
			case 'p':
				optPid = 1;
				break;
			case 'n':
				optN = 1;
				break;
			case 'u':
				optUid = 1;
				break;
			default:
				fprintf(stderr, "Invalid Option: %c \nUsage: %s [-p] [-n] [-u]  \n", option, argv[0]);
				exit(EXIT_FAILURE);
		}
	}

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
	int *uid = malloc(0);
	int numUid = 0;
	if(optUid == 1){
		printf("     UID\n");
	}
	else if(optPid == 1 && optN == 1){
		printf("     UID   PID   N           FNAME STIME\n");
	}
	else if(optPid == 1){
		printf("     UID   PID            FNAME STIME\n");
	}
	else if(optN == 1){
		printf("     UID   N           FNAME STIME\n");
	}
	else{
		printf("     UID            FNAME STIME\n");
	}
	while((file = readdir(proc)) != NULL){
		//process only actual files
		if(file->d_name[0] != '.'){
			sprintf(filename, "/proc/%s/psinfo", file->d_name);
			procFile = fopen(filename, "r");
			//process only files you can open
			if(procFile != NULL){
				fread(&procInfo, sizeof(psinfo_t), 1, procFile);
				if(optUid == 1){
					if(checkIfUidExist(procInfo.pr_uid, uid, numUid) == 0){
						numUid++;
						printf("numUid: %d\n", numUid);
						uid = realloc(uid, numUid * sizeof(int));
						uid[numUid] = procInfo.pr_uid;
						userInfo = getpwuid(procInfo.pr_uid);
						printf("%8s\n", userInfo->pw_name);
					}
				}
				else{
					userInfo = getpwuid(procInfo.pr_uid);
					printf("%8s ", userInfo->pw_name);
					if (optPid == 1){
						printf("%5d ", procInfo.pr_pid);
					}
					printf("%16s %.24s\n", procInfo.pr_fname, ctime((time_t *)&procInfo.pr_start.tv_sec));
				}
				
			}
		}
	}
	free(uid);
}

int checkIfUidExist(int newUid, int *uid, int numUid){
	int i;
	for(i = 0; i < numUid; i++){
		printf("%d == %d\n", uid[i], newUid);
		if(uid[i] == newUid){
			return 1;
		}
	}
	return 0;
}