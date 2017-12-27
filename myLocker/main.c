#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "pthTextCode.h"
#include "pthTextRW.h"
#include "pthLocker.h"

#define BUFF_LEN 50
#define textCodePort 2000
#define textRWPort 2050
#define lockerPort 2100
#define mainPort 2150

static void pthPrint(){
	fprintf(stderr,"\033[0;34m[main]\033[0m "); 
}

int main(){
	pthread_t pthTextCode;
	pthread_t pthTextRW;
	pthread_t pthLocker;

	int ret;
	int main_send_fd;
	int main_lock_fd; // main_lock_fd, main_addr, main_addr_len is used to communicate main with pthLocker.
	int flag;
	int option;
	void *retval;
    
	int textCode_addr_len;
	int textRW_addr_len;
	int locker_addr_len;
	int main_addr_len;
	int client_addr_len;

	int str_len;
	char *buff;

	struct sockaddr_in textCode_addr;
	struct sockaddr_in textRW_addr;
	struct sockaddr_in locker_addr;
	struct sockaddr_in main_addr;
	struct sockaddr_in client_addr; // it's used to recv msg when addr not sure.

	bool encrypt_ok = False;
	bool write_ok = False;	

	retval = NULL;
	pthread_mutex_init(&mutex_output,NULL); // init mutex for output

	/* pthTextCode create */
	ret = pthread_create(&pthTextCode,NULL,pthTextCode_run,NULL);
	if (ret != 0){
		pthPrint();
		perror("pthTextCode thread created error: ");
		exit(1);
	}
	else {
		pthread_mutex_lock(&mutex_output);
		pthPrint();
		printf("pthTextCode thread start...\n");
		pthread_mutex_unlock(&mutex_output);
	}

	/* pthTextRW create */
	ret = pthread_create(&pthTextRW,NULL,pthTextRW_run,NULL);
	if (ret != 0){
		pthPrint();
		perror("pthTextRW thread create error: ");
		exit(1);
	}
	else {
		pthread_mutex_lock(&mutex_output);
		pthPrint();
		printf("pthTextRW thread start...\n");
		pthread_mutex_unlock(&mutex_output);
	}

	/* pthLocker create */
	ret = pthread_create(&pthLocker,NULL,pthLocker_run,NULL);
	if (ret != 0){
		pthPrint();
		perror("pthLocker thread create error: ");
		exit(1);
	}
	else {
		pthread_mutex_lock(&mutex_output);
		pthPrint();
		printf("pthLocker thread start...\n");
		pthread_mutex_unlock(&mutex_output);
	}
	
	main_send_fd = socket(AF_INET,SOCK_DGRAM|SOCK_NONBLOCK,0);
	if(main_send_fd < 0){
		close(main_send_fd);
		pthPrint();
		perror("main_send_fd create failed: ");
		exit(1);
	}

	/* textCode send addr set */
	memset(&textCode_addr,0,sizeof(struct sockaddr_in));
	textCode_addr.sin_family = AF_INET;
	textCode_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* the ports which we can register for our service are in the range [1024~49151] */
	textCode_addr.sin_port = htons(textCodePort);

    /* textRW send addr set */
	memset(&textRW_addr,0,sizeof(struct sockaddr_in));
	textRW_addr.sin_family = AF_INET;
	textRW_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* the ports which we can register for our service are in the range [1024~49151] */
	textRW_addr.sin_port = htons(textRWPort);

    /* Locker send addr set */
	memset(&locker_addr,0,sizeof(struct sockaddr_in));
	locker_addr.sin_family = AF_INET;
	locker_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* the ports which we can register for our service are in the range [1024~49151] */
	locker_addr.sin_port = htons(lockerPort);
	
	/* main addr set */
	memset(&main_addr,0,sizeof(struct sockaddr_in));
	main_addr.sin_family = AF_INET;
	main_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* the ports which we can register for our service are in the range [1024~49151] */
	main_addr.sin_port = htons(mainPort);

	main_lock_fd = socket(AF_INET,SOCK_DGRAM|SOCK_NONBLOCK,0);
	ret = setsockopt(main_lock_fd,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option));
	if(ret < 0){
		pthPrint();
		perror("main_lock_fd setsockopt failed: ");
		exit(1);
	}
	if(bind(main_lock_fd,(struct sockaddr*)&main_addr,sizeof(struct sockaddr_in)) < 0){
		close(main_lock_fd);
		pthPrint();
		perror("main_socket_fd bind failed: ");
		exit(1);
	}

	buff = (char *)malloc(BUFF_LEN);
	memset(buff,0,BUFF_LEN);
	textCode_addr_len = sizeof(textCode_addr);
	textRW_addr_len = sizeof(textRW_addr);
	locker_addr_len = sizeof(locker_addr);
	client_addr_len = sizeof(client_addr);

loop:
	pthPrint();
	if(pthread_mutex_lock(&mutex_output)){
		pthPrint();
		perror("main pthread mutex lock failed: ");
	}

	printf("please enter your command (encrypt or decrypt): ");
	scanf("%s",buff);
	pthread_mutex_unlock(&mutex_output);

	/* sizeof(buff) is 4, buff is a pointer! */
	pthread_mutex_lock(&mutex_output);
	pthPrint();
	printf("ret: %d, buff: %s\n",ret,buff);
	pthread_mutex_unlock(&mutex_output);

    if(strcmp(buff,"encrypt") == 0){
		ret = sendto(main_send_fd,buff,BUFF_LEN,0,(struct sockaddr *)&textCode_addr,textCode_addr_len);
		if(ret < strlen(buff)){
			pthPrint();
			perror("encrypt send to: ");
			exit(1);
		}
	}
	else if(strcmp(buff,"decrypt") == 0){
		ret = sendto(main_send_fd,buff,BUFF_LEN,0,(struct sockaddr *)&textRW_addr,textRW_addr_len);	
		if(ret < strlen(buff)){
			pthPrint();
			perror("decrypt send to pthTextRW: ");
			exit(1);	
		}
		
		ret = sendto(main_send_fd,buff,BUFF_LEN,0,(struct sockaddr *)&textCode_addr,textCode_addr_len);	
		if(ret < strlen(buff)){
			pthPrint();
			perror("decrypt send to pthTextCode: ");
			exit(1);	
		}

	}
	else {
		pthread_mutex_lock(&mutex_output);
		pthPrint();
		fprintf(stderr,"please enter valid command(encrypt or decrypt).\n");
		pthread_mutex_unlock(&mutex_output);
		goto loop;
	}
	
	fd_set readfds;
	struct timeval tv = {0,0};

	while(1){
		memset(buff,0,BUFF_LEN);
		FD_ZERO(&readfds);
		FD_SET(main_send_fd,&readfds);
		FD_SET(main_lock_fd,&readfds);

		if(select(FD_SETSIZE,&readfds,NULL,NULL,&tv) <= 0){
			continue;
		}

		if(FD_ISSET(main_send_fd,&readfds)){	
			ret = recvfrom(main_send_fd,buff,BUFF_LEN,0,(struct sockaddr *)&client_addr,&client_addr_len);
			if(ret > 0){
				if(strcmp(buff,"prime numbers created!") == 0){
					pthread_mutex_lock(&mutex_output);
					pthPrint();
					printf("recv msg \"prime numbers created!\"\n");
					pthread_mutex_unlock(&mutex_output);

					char pthTextRW_W[] = "write to .key";
					str_len = strlen(pthTextRW_W);
					if(sendto(main_send_fd,pthTextRW_W,str_len,0,(struct sockaddr*)&textRW_addr,textRW_addr_len) < str_len){
						pthPrint();
						perror("main send write command to pthTextRW failed. ");
					}
					
					char pthLocker_en[] = "encrypt";
					str_len = strlen(pthLocker_en);
					if(sendto(main_send_fd,pthLocker_en,str_len,0,(struct sockaddr*)&locker_addr,locker_addr_len) < str_len){
						pthPrint();
						perror("main send encrypt command to pthLocker failed. ");
					}
				}
				else if(strcmp(buff,"write ok") == 0){
					pthread_mutex_lock(&mutex_output);	
					pthPrint();
					printf("main recv \"write ok\" from pthTextRW.\n");
					pthread_mutex_unlock(&mutex_output);	
					
					write_ok = True;
					if(encrypt_ok == True && write_ok == True){	
						pthread_mutex_lock(&mutex_output);	
						pthPrint();
						printf("encrypt and prime number write work all finished!\n");
						pthread_mutex_unlock(&mutex_output);	
						break;
					}
				}
				else if(strcmp(buff,"read ok") == 0){
					memset(buff,0,BUFF_LEN);
					strcpy(buff,"decrypt");
					ret = sendto(main_send_fd,buff,BUFF_LEN,0,(struct sockaddr *)&locker_addr,locker_addr_len);
					if(ret < strlen(buff)){
						pthPrint();
						perror("main -> locker \"decrypt\": ");
						exit(1);
					}
				}
			}
		}
		else if(FD_ISSET(main_lock_fd,&readfds)){
			ret = recvfrom(main_lock_fd,buff,BUFF_LEN,0,(struct sockaddr *)&locker_addr,&locker_addr_len);
			if(ret > 0){
				if(strcmp(buff,"encrypt ok") == 0){
					pthread_mutex_lock(&mutex_output);	
					pthPrint();
					printf("main recv \"encrypt ok\" from pthLocker.\n");
					pthread_mutex_unlock(&mutex_output);	
					encrypt_ok = True;
					if(encrypt_ok == True && write_ok == True){
						pthread_mutex_lock(&mutex_output);	
						pthPrint();
						printf("encrypt and prime number write work all finished!\n");
						pthread_mutex_unlock(&mutex_output);	
						break;
					}					
				}
				else if(strcmp(buff,"decrypt ok") == 0){
					pthread_mutex_lock(&mutex_output);
					pthPrint();
					printf("main recv \"decrypt ok\" from pthLocker, that means all tasks finished.\n");
					pthread_mutex_unlock(&mutex_output);
					break;
				}
			}
		}
	}
	pthread_join(pthTextCode,&retval);
	pthPrint();
	printf("retval from textCode thread: %d\n",*(int*)(retval));
	pthread_join(pthTextRW,&retval);
	pthPrint();
	printf("retval from textRW thread: %d\n",*(int*)(retval));
	pthread_join(pthLocker,&retval);
	pthPrint();
	printf("retval from Locker thread: %d\n",*(int*)(retval));
	
	pthread_mutex_destroy(&mutex_output);   //free mutex resource.
	free(retval);
	free(buff);
	return 0;
}
