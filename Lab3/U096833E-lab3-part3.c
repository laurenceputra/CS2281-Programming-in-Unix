#include <stdio.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Please enter a filename in the command line\n");
		return 2;
	}
	FILE *binFile = fopen(argv[1], "rb");
	char buffer[64];
	unsigned char temp[64];
	int i = 0, tmp;
	do{
		tmp = fgetc(binFile);
		printf("%c", tmp);
	}while(tmp != EOF);
	return 0;
}