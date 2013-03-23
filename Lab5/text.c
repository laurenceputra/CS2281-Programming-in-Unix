#include <stdio.h>
int main(){

	while(1){
		usleep(5000000);
		printf("%d\n", utime(NULL));
	}
}
