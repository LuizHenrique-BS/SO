#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>

sem_t semid; //Variavel para semaforo
sem_t semid2;
void * Thread1(){
	int i=0;
	sem_wait(&semid);//Inicio Regiao Critica
	for(i=1;i<10;i++){
		printf("T1 - %d\n",i);
		sleep(2);
	}
	sem_post(&semid);//Fim Regiao Critica
}
void * Thread2(){
	int i=0;
	sem_wait(&semid);
	for(i=20;i<30;i++){
		printf("T2 - %d\n",i);
		sleep(1);
	}
	sem_post(&semid);
}

void * Thread3(){
	int i=0;
	sem_wait(&semid2);//Inicio Regiao Critica
	for(i=10;i<20;i++){
		printf("T3 - %d\n",i);
		sleep(2);
	}
	sem_post(&semid2);//Fim Regiao Critica
}
void * Thread4(){
	int i=0;
	sem_wait(&semid2);
	for(i=30;i<40;i++){
		printf("T4 - %d\n",i);
		sleep(1);
	}
	sem_post(&semid2);
}
int main(){
	pthread_t t1,t2,t3,t4;
	int ret1,ret2,ret3,ret4;
	//Cria semaforo iniciado com 1
	sem_init(&semid, 0, 1);
	sem_init(&semid2, 0, 1);
	ret1 = pthread_create(&t1,NULL, Thread1, NULL);
	ret2 = pthread_create(&t2,NULL, Thread2, NULL);
	ret3 = pthread_create(&t3,NULL, Thread3, NULL);
	ret4 = pthread_create(&t4,NULL, Thread4, NULL);	
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	printf("Main acabou\n");
}
