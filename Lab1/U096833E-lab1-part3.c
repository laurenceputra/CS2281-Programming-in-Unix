#include <stdio.h>

int main(){
	int i = 0, j = 0, size = 0;
	scanf("%d", &size);
	printf("#include <stdio.h>\n");
	printf("int main(){");
	printf("int numbers[%d];", size);
	printf("int med_index, temp, size=%d;", size);
	printf("scanf(\"");
	for(i = 0; i < size; i++){
		printf("%s", "%d");
		if(i != size - 1){
			printf(" ");
		}
		else{
			printf("\", ");
		}
	}
	for(i = 0; i < size; i++){
		printf("%s%d%s", "&numbers[", i, "]");
		if(i != size - 1){
			printf(", ");
		}
		else{
			printf(");");
		}
	}
	for(i = 0; i < size; i++){
		for(j = 0; j < size - 1; j++){
			printf("if(numbers[%d] > numbers[%d]){", j, j+1);
			printf("temp = numbers[%d];", j);
			printf("numbers[%d] = numbers[%d];", j, j+1);
			printf("numbers[%d] = temp;", j+1);
			printf("}");
		}
	}
	printf("med_index = (size / 2);");
	printf("printf(\"%s\\n\", numbers[med_index]);", "%d");
	printf("return 0;}");
	return 0;
}
