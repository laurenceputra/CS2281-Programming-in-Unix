#include <stdio.h>

int main(){
	int i = 0, j = 0, size = 0;
	scanf("%d", &size);
	printf("#include <stdio.h>\n");
	printf("int main(){\n");
	printf("int sequence[%d];\n", size);
	printf("int diff;\n");
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
		printf("%s%d%s", "&sequence[", i, "]");
		if(i != size - 1){
			printf(", ");
		}
		else{
			printf(");\n");
		}
	}
	printf("if((sequence[1]/sequence[0])==(sequence[2]/sequence[1])){\n");
	printf("diff = sequence[1]/sequence[0];\n");
	printf("printf(\"%s\", sequence[%d] * diff, sequence[%d] * diff * diff);\n", "%d %d", size - 1, size - 1);
	printf("}");
	printf("else if((sequence[1]-sequence[0])==(sequence[2]-sequence[1])){\n");
	printf("diff = sequence[1] - sequence[0];\n");
	printf("printf(\"%s\", sequence[%d] + diff, sequence[%d] + diff + diff);\n", "%d %d", size - 1, size - 1);
	printf("}");
	printf("}");
}
