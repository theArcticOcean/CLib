#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE* text_f = fopen("../folder/txt","r+");
	if(text_f == NULL){
		perror("text_f fopen: ");
		exit(1);
	}
	long pos, posEnd;
	char ch;

	fseek(text_f,0L,SEEK_END);
	posEnd = ftell(text_f);
	fseek(text_f,0L,SEEK_SET);
	pos = ftell(text_f);
	printf("len is %d\n",posEnd-pos);
	printf("pos is %d\n",pos);
	int primer_stack[] ={2, 2, 23, 10501};  //2 2 23 10501
	int primer_cnt = 4;
	int i = 0;
	while(pos < posEnd-1){
		fscanf(text_f,"%c",&ch);
		int primer = primer_stack[i++];
		if(i == primer_cnt){
			i = 0;
		}
		ch = ch^primer;
		fseek(text_f,-1,SEEK_CUR);
		fprintf(text_f,"%c",ch);
		pos = ftell(text_f);
		printf("pos is %d\n",pos);
	}
	return 0;
}
