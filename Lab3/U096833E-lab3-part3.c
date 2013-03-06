#include <stdio.h>
#include <stdlib.h>
typedef struct record {
    int account;
    char name[30];
    int date;
    int month;
    int year;
    int padding;
    char op[5];
    long long transaction;
    long long amount_left;
} record;

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Please enter a filename in the command line\n");
		return 2;
	}
	FILE *binFile = fopen(argv[1], "rb");
	char buffer[64];
	unsigned char temp[64];
	char tmp;
	int i = 0, data[64];
	record *cur = malloc(sizeof(record));
	while(fread(&cur->account, 1, 4, binFile)){
		fread(cur->name, 1, 30, binFile);
		fread(&cur->date, 1, 1, binFile);
		fread(&cur->month, 1, 1, binFile);
		fread(&cur->year, 1, 2, binFile);
		fread(&cur->padding, 1, 2, binFile);
		fread(cur->op, 1, 4, binFile);
		cur->op[4] = '\0';
		fread(&cur->padding, 1, 4, binFile);
		fread(&cur->transaction, 1, 8, binFile);
		fread(&cur->amount_left, 1, 8, binFile);
		printf("%d %s\n", cur->account, cur->name);
		printf("%d %d %d\n", cur->date, cur->month, cur->year);
		printf("%s %ld %ld\n DONE!\n", cur->op, cur->transaction, cur->amount_left);
	}
	return 0;
}