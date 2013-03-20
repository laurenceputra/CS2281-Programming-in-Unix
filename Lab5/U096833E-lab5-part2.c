#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int splitCommand(char **args, char *command);

int main(int argc, char** argv){
	int option, configOn = 0;
	char *prog, *config = NULL;


	while((option = getopt(argc, argv, "c:")) != -1){
		switch(option){
			case 'c':
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
		char tmp[255];
		while(fgets(tmp, 255 stdin) != NULL){
			fputs(tmp, tmpConfigFile);
		}
		fclose(tmpConfigFile);
	}
	prog = argv[argc - 1];

	return status;
}
