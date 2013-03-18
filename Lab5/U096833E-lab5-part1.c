#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int splitCommand(char **args, char *command);

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
						searchpaths[searchpathSize - 1] = tmpSearchPath;
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
				token = strtok(command, " ");
				while(token != NULL){
					args[argSize] = strdup(token);
					argSize++;
					token = strtok(NULL, " ");
				}
				args[argSize] = NULL;
				int i;
				for(i = 0; i < 128; i++){
					if(args[i] != NULL){
						printf("%s\n", args[i]);
					}
					else{
						printf("\n");
						break;
					}
				}
				int execRet = execv(args[0], args);
				printf("execv fail:%d %s\n", execRet, args[0]);
				exit(execRet);
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

/***********************
 * Command is passed in through command variable
 * args variable is overwritten
***********************/
int splitCommand(char **args, char *command){
	int argSize = 0;
	char *tmp = strtok(command, " ");
	char **tmpargs;
	args = malloc(0);
	while(tmp != NULL){
		argSize++;
		tmpargs = realloc(args, argSize * sizeof(char*));
		if(tmpargs != NULL){
			args = tmpargs;
			args[argSize - 1] = strdup(tmp);
		}
		else{
			exit(EXIT_FAILURE);
		}
		printf("%s\n", args[0]);

		tmp = strtok(NULL, " ");
	}
	argSize++;
	tmpargs = realloc(args, argSize * sizeof(char*));
	if(tmpargs != NULL){
		args = tmpargs;
		args[argSize - 1] = NULL;
	}
	else{ 
		exit(EXIT_FAILURE);
	}
	return argSize;
}
