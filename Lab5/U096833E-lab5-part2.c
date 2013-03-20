#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFERSIZE 256

int compareFiles(char *fileOne, char *fileTwo);

int main(int argc, char** argv){
	int option, configOn = 0, inputStart = 0, outputStart = 0, newline = 0, numArgs = 0, numApps = 0, i;
	int outputCompare = 0, status;
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
	for(i = 0; i < numApps; i++){
		FILE *configFile = fopen(config, "r");
		FILE *stdInput, *stdOutput;
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
				stdInput = fopen(inputFile, "w");
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
					fclose(stdInput);
				}
				else{
					fputs(tmp, stdInput);
				}
				
				continue;
			}
			memcpy(compare, tmp, 6);
			compare[6] = '\0';
			if(outputStart == 0 && strcmp(compare, "output") == 0){
				stdOutput = fopen(outputFile, "w");
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
					fclose(stdOutput);
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
						waitpid(pID, &status, 0);
						//compare
						outputCompare = compareFiles(stdOutputFile, outputFile);
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
				}
			}
		}
		fprintf(stdout, "\n");
		fclose(configFile);
		if(configOn == 1){
			//remove(config);
		}
		//remove(inputFile);
		//remove(outputFile);
		//remove(stdOutputFile);
	}

	return status;
}


//Compare 2 files
int compareFiles(char *fileOne, char *fileTwo){
	FILE *fileLinkOne = fopen(fileOne, "r");
	FILE *fileLinkTwo = fopen(fileTwo, "r");
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