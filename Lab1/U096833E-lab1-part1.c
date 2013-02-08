#include <stdio.h>

int main(){
	int i = 0, j = 0, size = 0;
	scanf("%d", &size);
	printf("#include <stdio.h>\n");
	printf("int main(){");
	printf("printf(\"");
	for(i = 1; i <= size; i++){
		for(j = 0; j < i; j++){
			printf("*");
		}
		printf("\\n");
	}
	printf("\");");
	printf("}");
}