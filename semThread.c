#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

sem_t *sem[10];

void *run(void *param){
	int *tmp = (int *)malloc(sizeof(int));
	*tmp = *((int *)param);
	sem_wait(sem[*tmp]);	
	pthread_exit(tmp);
	return NULL;
}
int main(){
	pthread_t tid[10];
	int tag[10] = {0,1,2,3,4,5,6,7,8,9};
	int i;
	for(i=0;i<10;i++){
		char str[3] = {0};
		str[0] = i+'0';
		sem[i] = sem_open(str, O_CREAT, 0777);
	}
	for(i=0;i<10;i++){
		pthread_create(&tid[i],NULL,run, &tag[i]);
		printf("create tid %d.\n",i);
	}
	for(i=0;i<10;i++){
		sem_post(sem[i]);
	}
	for(i=0;i<10;i++){
		int *ret;
		pthread_join(tid[i],&ret);
		sem_close(sem[i]);
		printf("tid %d finished.\n",*ret);
		free(ret);
	}
	return 0;
}
