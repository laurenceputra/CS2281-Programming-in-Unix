#include <stdio.h>
#include <stdlib.h>
typedef struct record {
    unsigned int account;
    char name[30];
    unsigned char date;
    unsigned char month;
    unsigned short year;
    char op[5];
    unsigned long long transaction;
    unsigned long long amount_left;
} record;

int mybswap16(int val){
	return (val << 8) | (val >> 8);
}
int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Please enter a filename in the command line\n");
		return 2;
	}
	FILE *binFile = fopen(argv[1], "rb");
	int i = 0, padding;
	record *cur = malloc(sizeof(record));
	while(fread(&cur->account, 1, 4, binFile)){
		fread(cur->name, 1, 30, binFile);
		fread(&cur->date, 1, 1, binFile);
		fread(&cur->month, 1, 1, binFile);
		fread(&cur->year, 1, 2, binFile);
		fread(&padding, 1, 2, binFile);
		fread(cur->op, 1, 4, binFile);
		cur->op[4] = '\0';
		fread(&padding, 1, 4, binFile);
		fread(&cur->transaction, 1, 8, binFile);
		fread(&cur->amount_left, 1, 8, binFile);
		cur->account = __builtin_bswap32(cur->account);
		cur->year = mybswap16(cur->year);
		cur->transaction = __builtin_bswap64(cur->transaction);
		cur->amount_left = __builtin_bswap64(cur->amount_left);
		printf("Account: %d\nName: %s\n", cur->account, cur->name);
		printf("Date: %d/%d/%d\n", cur->date, cur->month, cur->year);
		printf("Operation: %s\nAmount Transfered: %lld\n Amount Remaining: %lld\n\n", cur->op, cur->transaction, cur->amount_left);
	}
	return 0;
}