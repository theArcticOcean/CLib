#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>

sem_t *sem;

void *run(void *arg){
	int *tmp = (int *)arg;
	while(1){
		sem_wait(sem);			//the value of semaphore is reduced, the thread keep run.
		*tmp = *tmp + 1;	
	}
}

int main(int argc, char **argv){
	int cnt = 0;
	int i;
	pthread_t tid;
	sem = sem_open("test_sem",O_CREAT,0777,0);
	if(pthread_create(&tid,NULL,run,&cnt)){
		perror("pthread create: ");	
		return -1;
	}
	for(i=0; i<10; i++){
		if(i&1) 
			if(sem_post(sem)){  //the value of semaphore is incremented, and all threads waiting for it (save cpu resource) are awakened.
				perror("sem_post ");
			}
	}
	pthread_cancel(tid);
	sem_close(sem);
	printf("%d\n",cnt);
	return 0;
}
