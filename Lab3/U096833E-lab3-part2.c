#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 64

void my_memusage(char *);

int main(){
	char buffer[BUFFER_SIZE];
	size_t curBufferSize;
	size_t lineSize = 0; //null string
	char *line = malloc(sizeof(char) * 1);
	int palindrome, loopLimit, counter;

	my_memusage("INIT");

	while(fgets(buffer, BUFFER_SIZE, stdin)){
		curBufferSize = strlen(buffer);
		lineSize += curBufferSize;
		line = realloc(line, lineSize);
		if(line == NULL){
			printf("Realloc failed!\n");
			free(line);
			exit(-1);
		}
		strcat(line, buffer);
		if((curBufferSize > 0 && buffer[curBufferSize - 1] == '\n') || feof(stdin)){
			if(curBufferSize > 0 && buffer[curBufferSize - 1] == '\n'){
				lineSize--;
				line[lineSize] = '\0';
			}
			if (lineSize == 0){
				continue;
			}
			else{
				lineSize--;
				loopLimit = lineSize / 2;
				palindrome = 0;
				for (counter = 0; counter < loopLimit; counter++){
					if(line[counter] != line[lineSize - counter]){
						palindrome = 1;
						break;
					}
				}
				if (palindrome == 0){
					fputs(line, stdout);
					fputs("\n", stdout);
				}
			}
			free(line);
			lineSize = 0;
			line = malloc(sizeof(char) * 1);
		}
	}

	my_memusage("END");
	return 0;
}
