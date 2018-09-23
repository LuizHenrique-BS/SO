#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1

sem_t mutex, escritex;
// escritex: controla o acesso ao banco de dados
// mutex: garante a exclusao mutua na regiao critica

int leitores;
// leitores: conta a quantidade de leitores acessando o banco de dados simultaneamente

void *escritor(int );
void *leitor(int );

int main(void){
	
	leitores = 0;

	//Inicializa os semaforos
	sem_init (&mutex, 0, 1);
	sem_init (&escritex, 0, 1);
	
	pthread_t thd0, thd1, thd2, thd3, thd4;
	
	//Inicializa as threads
	pthread_create(&thd0, 0, (void *) leitor, 1);
	pthread_create(&thd1, 0, (void *) leitor, 2);
	pthread_create(&thd2, 0, (void *) leitor, 3);
	pthread_create(&thd3, 0, (void *) escritor, 0);
	pthread_create(&thd4, 0, (void *) escritor, 1);
	
	//Finalizacao das threads
	pthread_join(thd0, 0);
	pthread_join(thd1, 0);
	pthread_join(thd2, 0);
	pthread_join(thd3, 0);
	pthread_join(thd4, 0);

	exit(0);
}

// Escreve itens no buffer

void escrever_item(int i){
	printf("\nO escritor NUMERO %d está escrevendo no buffer", i);
	
}

// Le itens do buffer
void ler_item(int i){
	printf("\nO leitor NUMERO %d está lendo item do buffer", i);
}


// Simula a escrita de um item

void *escritor(int id){

	while(TRUE){
		// escritor aguarda até que ninguém esteja acessando o banco de dados para acessa-lo
		sem_wait(&escritex);
		
			escrever_item(id);
		
		sem_post(&escritex);
  
		sleep(4);
	}	

}

// Simula a leitura de um item
void *leitor(int id){

	while(TRUE){
		
		// O acesso a variavel leitores eh sincronizado pelo mutex
		sem_wait(&mutex);
			++leitores;
			
			//O acesso ao banco de dados eh fechado aos escritores pela passagem do primeiro leitor
			if(leitores == 1)
				sem_wait(&escritex);
		// O acesso a variavel leitores eh liberado para os demais leitores
		sem_post(&mutex);
		
		// A leitura do item eh efetuada
			ler_item(id);
		
		// Novamente eh feita a sincronizacao da variavel leitores	
		sem_wait(&mutex);
			--leitores;
			// O acesso ao banco de dados eh liberado aos escritores caso nao haja nenhum leitor
			if(leitores == 0)
				sem_post(&escritex);
		sem_post(&mutex);
		
		sleep(4);
	}

}
