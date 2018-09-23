#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define FALSE 0
#define TRUE 1

#define N 20

struct timespec t;

sem_t mutex, empty, full;
// empty: semforo que representa as posicoes vazias do buffer
// full: indica as posicoes cheias do buffer
// mutex: garante a exclusao mutua na regiao critica

int buffer[N], lo, hi, count;
// lo: proxima prosicao cheia
// hi: proxima posicao vazia
// count: conta a quantidade de dados presentes no buffer

void *produtor(void *);
void *consumidor(void *);

int main(void){
	//Seta a semente de geracao de numeros aleatorios
	srand(time(NULL));

	count = 0;
	lo = 0;
	hi = 0;

	//Inicializa os semaforos
	sem_init (&mutex, 0, 1);
	sem_init (&empty, 0, N);
	sem_init (&full, 0, 1);
	
	pthread_t thd0, thd1, thd2, thd3, thd4;
	
	//Inicializa as threads
	pthread_create(&thd0, 0, produtor, NULL);
	pthread_create(&thd1, 0, produtor, NULL);
	pthread_create(&thd2, 0, produtor, NULL);
	pthread_create(&thd3, 0, consumidor, NULL);
	pthread_create(&thd4, 0, consumidor, NULL);
	
	//Finalizacao das threads
	pthread_join(thd0, 0);
	pthread_join(thd1, 0);
	pthread_join(thd2, 0);
	pthread_join(thd3, 0);
	pthread_join(thd4, 0);

	exit(0);
}

// Produz os itens do buffer (numeros aleatorios)

int produz_item(){
	int val;
	val = rand() % 1000;
	printf("\nProduzindo item: %d", val);
	
	return val;	
}

// Exibe o valor q esta sendo consumido
void consome_item(int item){
	printf("\nConsumindo item: %d", item);
}

// Insere os itens produzidos no buffer
void insere_item(int val){
	if(count < N){
		buffer[hi] = val;
		//A proxima posicao vazia e o contador de elementos sao incrementados
		hi = (hi + 1) % N;
		count = count + 1;
		if(count == N)
			printf("\n############## Buffer cheio ##############");
	}
	sleep(1);
}

// Retira item do buffer

int remove_item(){
	int val;
	if(count > 0){
		//O item eh removido
		val = buffer[lo];
		
		//Em seguida a proxima posicao cheia eh incrementada para a posicao seguinte
		lo = (lo + 1) % N;
		// o contador de elementos eh decrementado
		count = count - 1;
		return val;
	}
}

void *produtor(void *p_arg){
	int item;
	
	while(TRUE){
		item = produz_item();
		
		// sem_wait fecha o semaforo e o sem_post abre o semaforo
		sem_wait(&empty);
		sem_wait(&mutex);
		
		insere_item(item);
		
		sem_post(&mutex);
		sem_post(&full);
		
		sleep(1);
	}

}

void *consumidor(void *p_arg)
{
	int item;

	while(TRUE)
	{
		
		sem_wait(&full);
		sem_wait(&mutex);
		
		item = remove_item();
		
		sem_post(&mutex);
		sem_post(&empty);
		
		consome_item(item);
  
		sleep(1);
	}
}
