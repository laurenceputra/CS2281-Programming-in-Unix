//When I wrote this, only the compiler and I understood what I was doing
//Now, only the compiler knows
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFERSIZE 256

int compareFiles(FILE *fileLinkOne, char* fileOne, FILE *fileLinkTwo, char *fileTwo);

int main(int argc, char** argv){
	int option, configOn = 0, inputStart = 0, outputStart = 0, newline = 0, numArgs = 0, numApps = 0, i;
	int outputCompare = 0, status, fileFailCounter, waitPidStatus, childStatus, timePassed;
	char *prog, *config = NULL;
	char *stdOutputFile = "__stdOutputFile__", *inputFile = "__inputFile__", *outputFile = "__outputFile__";
	char *delimiter, *tmpDelimiter;
	char **progList, **tmpProgList;
	char tmp[BUFFERSIZE], compare[10];
	pid_t pID = 0;

	while((option = getopt(argc, argv, "c:")) != -1){
		switch(option){
			case 'c':
				numArgs++;
				configOn = 1;
				config = optarg;
				break;

			default:
				fprintf(stderr, "Invalid Option: %c \nUsage: %s [-c testconfig] [prog ...] \n", option, argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	//dump stdin into a config file
	if(config == NULL){
		config = "tmpConfig";
		FILE *tmpConfigFile = fopen(config, "w");
		while(fgets(tmp, BUFFERSIZE, stdin) != NULL){
			fputs(tmp, tmpConfigFile);
		}
		fclose(tmpConfigFile);
	}
	progList = malloc(0);
	for(i = (numArgs * 2) + 1; i < argc; i++){
		numApps++;
		tmpProgList = realloc(progList, numApps*sizeof(char*));
		if(tmpProgList == NULL){
			fprintf(stderr, "Unable to realloc!\n");
			exit(EXIT_FAILURE);
		}
		progList = tmpProgList;
		progList[numApps - 1] = argv[i];
	}
	FILE *configFile = fopen(config, "r");
	FILE *stdInput = fopen(inputFile, "w"), *stdOutput = fopen(outputFile, "w");
	FILE *stdOutputFileDescriptor = fopen(stdOutputFile, "r");
	for(i = 0; i < numApps; i++){
		configFile = fopen(config, "r");
		fprintf(stdout, "%s:", progList[i]);
		fflush(stdout);
		while(fgets(tmp, BUFFERSIZE, configFile)){
			if(tmp[0] == '#'){
				continue;
			}
			memcpy(compare, tmp, 4);
			compare[4] = '\0';
			if(strcmp(compare, "test") == 0){
				continue;
			}
			memcpy(compare, tmp, 5);
			compare[5] = '\0';
			if(inputStart == 0 && strcmp(compare, "input") == 0){
				stdInput = freopen(inputFile, "w", stdInput);
				inputStart = 1;
				tmpDelimiter = strtok(tmp, "<");
				while(tmpDelimiter != NULL){
					delimiter = strdup(tmpDelimiter);
					tmpDelimiter = strtok(NULL, "<");
				}
				continue;
			}
			if(inputStart == 1){
				if(strcmp(tmp, delimiter) == 0){
					inputStart = 0;
				}
				else{
					fputs(tmp, stdInput);
				}
				fflush(stdInput);
				
				continue;
			}
			memcpy(compare, tmp, 6);
			compare[6] = '\0';
			if(outputStart == 0 && strcmp(compare, "output") == 0){
				stdOutput = freopen(outputFile, "w", stdOutput);
				if(stdOutput == NULL){
					fprintf(stderr, "Output file cannot be opened!\n");
				}
				outputStart = 1;
				tmpDelimiter = strtok(tmp, "<");
				while(tmpDelimiter != NULL){
					delimiter = strdup(tmpDelimiter);
					tmpDelimiter = strtok(NULL, "<");
				}
				continue;
			}
			if(outputStart == 1){
				if(strcmp(tmp, delimiter) == 0){
					outputStart = 0;
					//start running
					pID = fork();
					if(pID == 0){
						//child process does stuff
						//printf("%s\n", strcat(strcat(strdup(progList[i]), strcat(strdup(" < "), strdup(inputFile))) , strcat(strdup(" > "), stdOutputFile)));
						system(strcat(strcat(strdup(progList[i]), strcat(strdup(" < "), strdup(inputFile))) , strcat(strdup(" > "), stdOutputFile)));
						return 0;
					}
					else if(pID < 0){
						fprintf(stderr, "Forking failed\nTerminating\n");
						exit(EXIT_FAILURE);
					}
					else{
						//wait for child to be done, and compare
						timePassed = 0;
						waitPidStatus = waitpid(pID, &childStatus, WNOHANG);
						while(timePassed < timeoutLimit && waitPidStatus == 0){
							timePassed++;
							sleep(1);
							waitPidStatus = waitpid(pID, &childStatus, WNOHANG);
						}
						if(waitPidStatus == 0){
							kill(pID, SIGABRT);
						}
						//compare
						outputCompare = compareFiles(stdOutput, outputFile, stdOutputFileDescriptor, stdOutputFile);
						if(outputCompare == 0){
							fprintf(stdout, " %d", 1);
						}
						else{
							fprintf(stdout, " %d", 0);
						}
						fflush(stdout);
					}
					
				}
				else{
					fputs(tmp, stdOutput);
					fflush(stdOutput);
				}
			}
		}
		fprintf(stdout, "\n");
		remove(stdOutputFile);
	}
	fclose(configFile);
	if(configOn != 1){
		remove(config);
	}
	fclose(stdOutputFileDescriptor);
	fclose(stdInput);
	fclose(stdOutput);
	remove(inputFile);
	remove(outputFile);

	return status;
}


//Compare 2 files
int compareFiles(FILE *fileLinkOne, char* fileOne, FILE *fileLinkTwo, char *fileTwo){
	fileLinkOne = freopen(fileOne, "r", fileLinkOne);
	fileLinkTwo = freopen(fileTwo, "r", fileLinkTwo);
	char tmp1[255], tmp2[255];
	char *readOne, *readTwo;
	readOne = fgets(tmp1, 255, fileLinkOne);
	readTwo = fgets(tmp2, 255, fileLinkTwo);
	while(readOne != NULL && readTwo != NULL){
		if(strcmp(tmp1, tmp2) != 0){
			return 1;
		}
		readOne = fgets(tmp1, 255, fileLinkOne);
		readTwo = fgets(tmp2, 255, fileLinkTwo);
	}
	if(readOne != NULL || readTwo != NULL){
		return 1;
	}
	return 0;
}