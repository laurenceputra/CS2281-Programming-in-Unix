#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv){
	int option, searchpathSize = 0;
	char **searchpaths, **tmpSearchPaths, *logfile, *scriptFile, *tmpSearchPath;
	char command[255];
	int stopOnError = 0, searchArg = 0, logArg = 0;
	logfile = "lab5-part1.log";
	while((option = getopt(argc, argv, "p:a::l:")) != -1){
		switch(option){
			case 'p':
				searchArg = 1;
				searchpaths = malloc(0);
				tmpSearchPath = strtok(optarg, ":");
				while(tmpSearchPath != NULL){
					searchpathSize++;
					tmpSearchPaths = realloc(searchpaths, searchpathSize * sizeof(char*));
					if(tmpSearchPath != NULL){
						searchpaths = tmpSearchPaths;
						searchpaths[searchpathSize - 1] = strcat(strdup(tmpSearchPath), "/");
					}
					
					tmpSearchPath = strtok(NULL, ":");
				}
				break;

			case 'a':
				stopOnError = 1;
				break;

			case 'l':
				logArg = 1;
				logfile = optarg;
				break;

			default:
				fprintf(stderr, "Invalid Option: %c \nUsage: %s [-p searchpath] [-a] [-l logfile] [_scriptfile_]\n", option, argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	scriptFile = argv[argc - 1];
	FILE *script = fopen(scriptFile, "r");
	FILE *logFileOutput = fopen(logfile, "w");
	pid_t pID = 0;
	int status;
	int argSize;
	char *args[128];
	char *token;
	int length;
	while(fgets(command, 255, script) != NULL){
		if(command[0] == '#' || strlen(command) == 1){
			continue;
		}
		else{
			length = strlen(command);
			command[length - 1] = '\0';
			pID = fork();
			if(pID == 0){
				//child process, run
				argSize = 0;
				char *remaining;
				token = strtok_r(command, " ", &remaining);
				while(token != NULL){
					args[argSize] = strdup(token);
					argSize++;
					token = strtok_r(NULL, " ", &remaining);
				}
				args[argSize] = NULL;
				char *tmpChar = strchr(args[0], '/');
				if (tmpChar == NULL){
					int counter;
					for(counter = 0; counter < searchpathSize; counter++){
						execv(strcat(strdup(searchpaths[counter]), args[0]), args);
					}
				}
				else{
					token = strtok(command, "/");
					while(token != NULL){
						tmpChar = token;
						token = strtok(NULL, "/");
					}
					token = strdup(args[0]);
					args[0] = strdup(tmpChar);
					execv(token, args);
				}
				exit(127);
			}
			else if(pID < 0){
				fprintf(stderr, "Forking failed\nTerminating\n");
				exit(EXIT_FAILURE);
			}
			else if(pID > 0){
				waitpid(pID, &status, 0);
				fprintf(logFileOutput, "%d: %s\n", WEXITSTATUS(status), command);
				fflush(logFileOutput);
				if(stopOnError != 0 && WEXITSTATUS(status) != 0){
					printf("%d\n", status);
					exit(status);
				}
			}
		}
	}
	fclose(script);
	fclose(logFileOutput);
	return status;
}
