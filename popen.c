#include <stdio.h>

int main(){
	// popen create pipe and run command in shell
	FILE *fp = popen("ls","r+");
	if(fp == NULL){
		perror("fopen ");
		return -1;
	}
	// to fetch command output
	char buff[1024];
	if(fread(buff,1,1024,fp)){
		printf("%s",buff);
	}
	// close fp
	pclose(fp);
	return 0;
}
