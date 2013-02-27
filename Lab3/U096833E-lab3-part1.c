#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 512

typedef struct part {
    char strChunk[CHUNK_SIZE]; 
    struct part *next; 
    struct part *before;
} part;

int main(){
	part *head = (part *)malloc(sizeof(part));
	head->before = NULL;
	head->next = NULL;
	part *tail = head;
	part *cur_head;
	int palindrome, loopLimit, counter, headCounter, tailCounter;
	size_t lineSize = CHUNK_SIZE, curLineSize = 0, curChunkSize = 0, curChunks = 1;

	while(!feof(stdin)){
		fgets(tail->strChunk, CHUNK_SIZE, stdin);
		curChunkSize = strlen(tail->strChunk);
		curLineSize += curChunkSize;
		if((curLineSize > 0 && tail->strChunk[curChunkSize - 1] == '\n') || feof(stdin)){
			//line gotten
			if(curLineSize > 0 && tail->strChunk[curChunkSize - 1] == '\n'){
				curLineSize--;
				curChunkSize--;
				tail->strChunk[curChunkSize] = '\0';
			}
			if(curLineSize != 0){
				curChunkSize--;
				curLineSize--;
				loopLimit = curLineSize / 2 + 1;
				cur_head = head;
				headCounter = 0;
				tailCounter = curChunkSize;
				if(headCounter == CHUNK_SIZE - 1){
					cur_head = cur_head->next;
					headCounter = 0;
				}
				if(tailCounter == -1){
					tail = tail->before;
					tailCounter = CHUNK_SIZE - 2;
				}
				palindrome = 0;
				for (counter = 0; counter < loopLimit; counter++){
					if(cur_head->strChunk[headCounter] != tail->strChunk[tailCounter]){
						palindrome = 1;
						break;
					}
					headCounter++;
					if(headCounter == CHUNK_SIZE - 1){
						cur_head = cur_head->next;
						headCounter = 0;
					}
					tailCounter--;
					if(tailCounter == -1){
						tail = tail->before;
						tailCounter = CHUNK_SIZE - 2;
					}
				}
				if (palindrome == 0){
					cur_head = head;
					while(curChunks != -1){
						fputs(cur_head->strChunk, stdout);
						curChunks--;
						cur_head = cur_head->next;
					}
					fputs("\n", stdout);
				}
				curChunks = 0;
				curLineSize = 0;
				curChunkSize = 0;
				tail = head;
			}
		}
		else{
			if(tail->next == NULL){
				//allocate new block and continue processing
				tail->next = (part *)malloc(sizeof(part));
				tail->next->next = NULL;
				tail->next->before = tail;
				lineSize += CHUNK_SIZE;
			}
			tail = tail->next;
			curChunks += 1;
		}
	}
	cur_head = head;
	while(cur_head->next != NULL){
		cur_head = cur_head->next;
		free(cur_head->before);
	}
	return 0;
}