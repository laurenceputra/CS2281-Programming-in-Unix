#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
	int option, searchpathSize = 0;
	char **searchpaths, **tmpSearchPaths, *logfile, *scriptFile, *tmpSearchPath;
	char command[255];
	int stopOnError = 0, searchArg = 0, logArg = 0;
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

	return 0;
}