//https://computing.llnl.gov/tutorials/pthreads/

#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
#include <semaphore.h>
#include <pthread.h>

#define MAX 5
int revisar;
sem_t mutex;

void crearNotas();
void *alumn(void *);

int main(int argc, char **argv){
	
	long i=0, j=0;
	pthread_t alumno[MAX];
	
	sem_init(&mutex, 0, 1);

	for (i = 0 ; i < MAX; i+=1) {

		printf("creando : %ld\n", i);
		sleep(2);

		if(pthread_create(&alumno[i], NULL, alumn, (void *)i)){
			printf("ERROR creating thread:  %ld\n", i);
		}

	}

	for (i = 0; i < MAX; i++) {
		
		pthread_join(alumno[i], NULL);
	}
	
	return 0;

}


void *alumn(void * params){
	

/*
	IMPORTANTE!!!
	*Este problema te tuvo pegado varias horas...
	*Esta parte es la importante, a la hora de usar threads los parámetros se taldean por que estás pasando
	*como direcciones de memoria y pasas referencias y blablabla, la cosa es que si lo haces de la manera 
	*aquí propuesta, los numeros no se taldean y todo anda ok.
*/

	long index = (long) params;
	printf("Empieza %ld\n",index);

//	int r = sem_trywait(&mutex);			

	int r = sem_wait(&mutex);			
	printf("Soy el alumno  %ld;  RETURN = %d\n", index, r );

	sleep(3);
	sem_post(&mutex);
	
	pthread_exit(NULL);
}