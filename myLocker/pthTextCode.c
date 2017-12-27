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

#include "pthTextCode.h"

#define pthTextCode_buff_len 50
#define textCodePort 2000


void* pthTextCode_run(void *arg){
	int sock_fd;
	int flag;
	int ret;
	int option;		
	int *retval;    // pthread return value 
    int low = 1e5;  // my_random() arguments.
	int high = 1e6; 
	int rand_number;
	char *buff;		// the receive buff
    struct sockaddr_in udpaddr;	//recv addr
	struct sockaddr_in client;	//client addr
	
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
	//fcntl(sock_fd,F_SETFL,flag|O_NONBLOCK);
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
	udpaddr.sin_port = htons(textCodePort);
	if(bind(sock_fd,(struct sockaddr*)&udpaddr,sizeof(struct sockaddr_in)) < 0){
		close(sock_fd);
		pthPrint();
		perror("pthTextCode_run bind failed: ");
		exit(1);
	}	

	buff = (char *)malloc(pthTextCode_buff_len); 
	if(buff == NULL){
		pthPrint();
		fprintf(stderr,"malloc failed, line: %d\n",__LINE__);
		close(sock_fd);
		exit(1);
	}

	while(1){
		int client_len;
		client_len = sizeof(client);
		memset(buff,0,pthTextCode_buff_len);
		ret = recvfrom(sock_fd,buff,pthTextCode_buff_len,0,(struct sockaddr*)&client,&client_len);
		pthread_mutex_lock(&mutex_output);
		pthPrint();
		printf("ret: %d, recv buff: %s\n",ret,buff);
		pthread_mutex_unlock(&mutex_output);
		if(ret > 0){
			if(strcmp(buff,"encrypt") == 0) {
				/* resolve number and create prime numbers. */
				rand_number = my_random(low,high);
				resolve(rand_number);
				/* prime numbers end. */
				char prime_created_msg[] = "prime numbers created!";
				short prime_msg_len = strlen(prime_created_msg);
				if(sendto(sock_fd,prime_created_msg,prime_msg_len,0,(struct sockaddr*)&client,client_len) < prime_msg_len){
					pthPrint();
					perror("\"prime numbers created!\" send to main failed: ");
				}
				else {
					pthread_mutex_lock(&mutex_output);
					pthPrint();
					fprintf(stdout,"pthTextCode -> main \"prime numbers created\" ok.\n");
					pthread_mutex_unlock(&mutex_output);
					break;
				}
			}
			else if(strcmp(buff,"decrypt") == 0){
				break;
			}
			else {
				pthPrint();
				fprintf(stderr,"command neither \"encrypt\" nor \"decrypt\" received.\n"); 
				exit(-1);
			}
			
		}
	}
	close(sock_fd);
	free(buff);
	buff = NULL;
	/* void pthread_exit(void *retval); */
	retval = (int *)malloc(sizeof(int));
	*retval = 1;
	pthread_exit((void *)retval);
	//return NULL;
}

void resolve(int number){
	int i;
	int goal_number;
	
	get_prime();
    prime_stack_cnt = 0;
	goal_number = number;

	for (i=0; i<prime_cnt; i++){
		while(goal_number%prime_number[i] == 0){
			prime_stack[prime_stack_cnt++] = prime_number[i];
			goal_number /= prime_number[i];
		}
	}

#ifdef DEBUG
	pthread_mutex_lock(&mutex_output);
	printf("origin number: %d\n\033[1;33m[textCodeThread]\033[0m its resolve prime numbers:\n",number);	
	for (i=0; i<prime_stack_cnt; i++){
		printf("\033[1;33m%d\033[0m ",prime_stack[i]);
	}
	puts("");
	pthread_mutex_unlock(&mutex_output);
#endif
}

int my_random(int low_boundary,int high_boundary){
	srand(time(NULL));
	/* rand() return value in [0 ~ RAND_MAX] */
	return (int)((double)rand()/RAND_MAX*(high_boundary-low_boundary)+0.5) + low_boundary;
}

static void pthPrint(){
	printf("\033[1;33m[textCodeThread]\033[0m "); 
}
