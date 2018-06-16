#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "pthTextRW.h"
#include "prime_stack.h"
#include "public.h"

#define pthTextRW_buff_len 50
#define textRWPort 2050

void* pthTextRW_run(void *argv){
	int sock_fd;
	int flag;
	int ret;
	int option;		
	int *retval;    // pthread return value 
	char *buff;		// the receive buff
    struct sockaddr_in udpaddr;	//recv addr
	struct sockaddr_in client;	//client addr
	
	key_path = "./folder/.key";  // the file path works based on gcc - Makefile.	
	retval = NULL;
	buff = NULL;
	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(sock_fd < 0){
		pthPrint();
		perror("sock_fd create failed: ");
		exit(1);
	}

	flag = fcntl(sock_fd,F_GETFL);
	/* sock_fd not blocked */
	// fcntl(sock_fd,F_SETTL,flag|O_NONBLOCK);
	ret = setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option));
	if(ret < 0){
		pthPrint();
		perror("sock_fd setsockopt failed: ");
		exit(1);
	}
	
	memset(&udpaddr,0,sizeof(struct sockaddr_in));
	udpaddr.sin_family = AF_INET;
	udpaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* the ports which we can register for our service are in the range [1024~49151] */
	udpaddr.sin_port = htons(textRWPort);
	if(bind(sock_fd,(struct sockaddr*)&udpaddr,sizeof(struct sockaddr_in)) < 0){
		close(sock_fd);
		pthPrint();
		perror("pthTextRW_run bind failed: ");
		exit(1);
	}

	buff = (char*)malloc(pthTextRW_buff_len);
	if(buff == NULL){
		pthPrint();
		fprintf(stderr,"malloc failed, line: %d\n",__LINE__);
		close(sock_fd);
		exit(1);
	}

	while(1){
		int client_len;
		client_len = sizeof(client);
		memset(buff,0,pthTextRW_buff_len);
		ret = recvfrom(sock_fd,buff,pthTextRW_buff_len,0,(struct sockaddr*)&client,(socklen_t *)&client_len);
		
		pthread_mutex_lock(&mutex_output);
		pthPrint();
		printf("ret: %d, recv buff: %s\n",ret,buff);
		pthread_mutex_unlock(&mutex_output);

		if(strcmp(buff,"write to .key") == 0){
			prime_write();
			memset(buff,0,pthTextRW_buff_len);
			strcpy(buff,"write ok");
			if(sendto(sock_fd,buff,pthTextRW_buff_len,0,(struct sockaddr*)&client,client_len) < strlen(buff)){
				pthPrint();
				perror("pthTextRW -> main \"write ok\" ");
			}
			break;
		}
		else if(strcmp(buff,"decrypt") == 0){
			prime_read();
			memset(buff,0,pthTextRW_buff_len);
			strcpy(buff,"read ok");
			ret = sendto(sock_fd,buff,pthTextRW_buff_len,0,(struct sockaddr *)&client,client_len);
			if(ret < strlen(buff)){
				pthPrint();
				perror("pthTextRW -> main \"primer read ok\": ");
			}
			break;
		}
	}

end_tag:
	close(sock_fd);
	free(buff);
	buff = NULL;
	retval = (int *)malloc(sizeof(int));
	*retval = 1;
	pthread_exit((void*)retval);
}


void prime_write(){
	FILE *key_p = NULL;
	int i;

	key_p  = fopen(key_path,"w");
	if(key_p == NULL){
		pthPrint();
		perror("key path w fopen: ");
		exit(1);
	}
	for (i=0; i<prime_stack_cnt; i++){
		if(fwrite(&prime_stack[i],sizeof(int),1,key_p) != 1){
			pthPrint();
			perror("fwrite: ");
			exit(1);
		}
	}
	fclose(key_p);
}

void prime_read(){
	FILE *key_p;
	int i;

	key_p = fopen(key_path,"r");
	i=0;
	if(key_p == NULL){
		pthPrint();
		perror("key_p r fope: ");
		exit(1);
	}
	while(!feof(key_p)){
		fread(&prime_stack[i++],sizeof(int),1,key_p);
	}
	fclose(key_p);

	prime_stack_cnt = i-1;	
	pthread_mutex_lock(&mutex_output);	
	int j;
	for (j=0; j<i-1; j++){
		printf("\033[0;32m%d\033[0m ",prime_stack[j]);
	}
	puts("");
	pthread_mutex_unlock(&mutex_output);
}
static void pthPrint(){
	printf("\033[0;32m[textRWThread]\033[0m ");
}
