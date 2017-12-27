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

#include "pthLocker.h"
#include "prime_stack.h"
#include "public.h"

#define lockerPort 2100
#define mainPort 2150
#define pthLocker_buff_len 50

void* pthLocker_run(void *arg){
	int sock_fd;
	int flag;
	int ret;
	int option;		
	int *retval;    // pthread return value 
	char *buff;		// the receive buff
    struct sockaddr_in udpaddr;	//recv addr
	struct sockaddr_in client;	//client addr is sames as main_addr
	struct sockaddr_in main_addr; // it's used to communicate main with pthLocker

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
	udpaddr.sin_port = htons(lockerPort);
	
	memset(&main_addr,0,sizeof(struct sockaddr_in));
	main_addr.sin_family = AF_INET;
	main_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* the ports which we can register for our service are in the range [1024~49151] */
	main_addr.sin_port = htons(mainPort);

	if(bind(sock_fd,(struct sockaddr*)&udpaddr,sizeof(struct sockaddr_in)) < 0){
		close(sock_fd);
		pthPrint();
		perror("pthLocker_run bind failed: ");
		exit(1);
	}

	buff = (char*)malloc(pthLocker_buff_len);
	if(buff == NULL){
		pthPrint();
		fprintf(stderr,"malloc failed, line: %d\n",__LINE__);
		close(sock_fd);
		exit(1);
	}

	while(1){
		int client_len;
		int main_addr_len;
		
		client_len = sizeof(client);
		main_addr_len = sizeof(main_addr);
		memset(buff,0,pthLocker_buff_len);
		ret = recvfrom(sock_fd,buff,pthLocker_buff_len,0,(struct sockaddr*)&client,&client_len);
		
		pthread_mutex_lock(&mutex_output);
		pthPrint();
		printf("ret: %d, recv buff: %s\n",ret,buff);
		pthread_mutex_unlock(&mutex_output);

		if(strcmp(buff,"encrypt") == 0){			
			pthread_mutex_lock(&mutex_output);
			pthPrint();
			printf("start encrypt text file...\n");
			pthread_mutex_unlock(&mutex_output);

			if(pth_encrypt() == 0){
				memset(buff,0,pthLocker_buff_len);
				strcpy(buff,"encrypt ok");
				if(sendto(sock_fd,buff,pthLocker_buff_len,0,(struct sockaddr*)&main_addr,main_addr_len) < strlen(buff)){
					pthPrint();
					fprintf(stderr,"pthLocker -> main send msg \"encrypt ok\" failed.\n");
					exit(1);
				}
				break;
			}
			else {
				pthPrint();
				fprintf(stderr,"encrypt text failed.\n");
			}
		}
		else if(strcmp(buff,"decrypt") == 0){			
			pthread_mutex_lock(&mutex_output);
			pthPrint();
			printf("start decrypt text file...\n");
			pthread_mutex_unlock(&mutex_output);

			if(pth_decrypt() == 0){
				memset(buff,0,pthLocker_buff_len);
				strcpy(buff,"decrypt ok");
				if(sendto(sock_fd,buff,pthLocker_buff_len,0,(struct sockaddr*)&main_addr,main_addr_len) < strlen(buff)){
					pthPrint();
					fprintf(stderr,"pthLocker -> main send msg \"decrypt ok\" failed.\n");
					exit(1);
				}
				break;
			}
			else {
				pthPrint();
				fprintf(stderr,"decrypt text failed.\n");
			}
		}
	}
	
	close(sock_fd);
	free(buff);
	buff = NULL;
	retval = (int *)malloc(sizeof(int));
	*retval = 1;
	pthread_exit((void *)retval);
}

int pth_encrypt(){
	char *filename;
	char *folder;
	char *buff;
	char *tmp;
	FILE *text_f;
	long posEnd;
	long pos;
	char ch;
	int primer;
	int i;

	buff = NULL;
	tmp = NULL;
	filename = (char *)malloc(105*sizeof(char));
	folder = (char *)malloc(105*sizeof(char));
	memset(filename,0,sizeof(filename));
	memset(folder,0,sizeof(folder));
	
	pthread_mutex_lock(&mutex_output);
	pthPrint();
	printf("please enter filename to enrypt which file is put in folder: \n");
	scanf("%s",filename);
	pthread_mutex_unlock(&mutex_output);

	strcpy(folder,"folder/");
	tmp = filename;
	filename = strcat(folder,filename);
	if(filename == NULL){
		free(filename);
		free(folder);
		perror("filename strcat: ");
		return -1;
	}
#ifdef DEBUG
	pthread_mutex_lock(&mutex_output);
	pthPrint();
	printf("the file need to encrypt is %s\n",filename);
	pthread_mutex_unlock(&mutex_output);
#endif

	text_f = fopen(filename,"r+");
	if(text_f == NULL){
		pthPrint();
		perror("fopen file need to encrypt failed: ");
		fclose(text_f);
		return -1;
	}
	
	fseek(text_f,0L,SEEK_END);
	posEnd = ftell(text_f);
	fseek(text_f,0L,SEEK_SET);
	pos = ftell(text_f);
	i = 0;

	while(pos < posEnd-1){
		fscanf(text_f,"%c",&ch);
		primer = prime_stack[i++];
		if(i == prime_stack_cnt){ 
			i = 0;	
		}
		ch = ch^primer;
		fseek(text_f,-1,SEEK_CUR);
		fprintf(text_f,"%c",ch);
		pos = ftell(text_f);
	}
	buff = "encrypt work finished.\n";
	pthread_mutex_lock(&mutex_output);
	pthPrint();
	printf("%s",buff);
	pthread_mutex_unlock(&mutex_output);
	fclose(text_f);
	free(filename);
	free(tmp);
	filename = NULL;
	tmp = NULL;
	return 0;
}

int pth_decrypt(){
	char *filename;
	char *folder;
	char *buff;
	char *tmp;
	FILE *text_f;
	long posEnd;
	long pos;
	char ch;
	int primer;
	int i;

	buff = NULL;
	filename = (char *)malloc(105*sizeof(char));
	folder = (char *)malloc(105*sizeof(char));
	memset(filename,0,sizeof(filename));
	memset(folder,0,sizeof(folder));
	
	pthread_mutex_lock(&mutex_output);
	pthPrint();
	printf("please enter filename to decrypt which file is put in folder: \n");
	scanf("%s",filename);
	pthread_mutex_unlock(&mutex_output);

	strcpy(folder,"folder/");
	tmp = filename;
	filename = strcat(folder,filename);
	if(filename == NULL){
		free(filename);
		free(folder);
		perror("filename strcat: ");
		return -1;
	}
#ifdef DEBUG
	pthread_mutex_lock(&mutex_output);
	pthPrint();
	printf("the file need to encrypt is %s\n",filename);
	pthread_mutex_unlock(&mutex_output);
#endif

	text_f = fopen(filename,"r+");
	if(text_f == NULL){
		pthPrint();
		perror("fopen file need to decrypt failed: ");
		fclose(text_f);
		return -1;
	}
	
	fseek(text_f,0L,SEEK_END);
	posEnd = ftell(text_f);
	fseek(text_f,0L,SEEK_SET);
	pos = ftell(text_f);
	i = 0;

	while(pos < posEnd-1){
		fscanf(text_f,"%c",&ch);
		primer = prime_stack[i++];
		if(i == prime_stack_cnt){ 
			i = 0;	
		}
		ch = ch^primer;
		fseek(text_f,-1,SEEK_CUR);
		fprintf(text_f,"%c",ch);
		pos = ftell(text_f);
	}
	buff = "decrypt work finished.\n";
	pthread_mutex_lock(&mutex_output);
	pthPrint();
	printf("%s",buff);
	pthread_mutex_unlock(&mutex_output);
	fclose(text_f);
	free(filename);
	free(tmp);
	filename = NULL;
	tmp = NULL;
	return 0;
}
static void pthPrint(){
	printf("\033[0;35m[lockerThread]\033[0m "); 
}
