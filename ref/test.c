#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
#include <semaphore.h>
#include <pthread.h>

#define MAX 50
#define T 200  // tiempo de T en milisec (solo hasta 999)
int notas[MAX];
long revisar;
sem_t espera_resultado, desocupado, nota, sala_de_espera;

void crearNotas();
void delay(int);
void *profe(void *);
void *alumn(void *);

//TEST TEST
void imprimir_notas();

int main(int argc, char **argv){
	
	long i=0;
	pthread_t alumno[MAX], profesor;
	
	crearNotas();

	sem_init(&desocupado, 0, 1);
	sem_init(&espera_resultado, 0, 0);
	sem_init(&nota, 0, 0);
	sem_init(&sala_de_espera, 0, 3);

	imprimir_notas();
	
	pthread_create(&profesor, NULL, profe, NULL);

	/// create all threads
	for (i = 0; i < MAX; i++) {
		delay(1);

		if(((i+1)%10)==0){
			if(i!=0){
				delay(9);
				printf("\nAqui ENTRA AL DELAY!!!===============\n");
			}
		}
		

		pthread_create(&alumno[i], NULL, alumn, (void *)i);
	}

	/// wait all threads
	for (i = 0; i < MAX; i++) {
		pthread_join(alumno[i], NULL);
	}

	imprimir_notas();

	return 0;
}

void *profe(void * params){
	
	while(1){
	
		sem_wait(&nota);			//el profe recibe nota

		printf("Soy el profe y le estoy revisando al alumno %ld\n", revisar+1);

		delay(3);

		notas[revisar] = notas[revisar] + ( rand() % 11 );
		if(notas[revisar]>100) notas[revisar]=100;

//		printf("Le cambie la nota a: %d\n", notas[revisar]);

		sem_post(&espera_resultado);		//el profe entrega nota lista
		sem_post(&desocupado);			//el profe se desocupa
	}
	
	
	pthread_exit(NULL);
}

void *alumn(void * params){
	
	long index = (long) params;				//recupera el indice de la nota

	int fila;

	fila = sem_trywait(&sala_de_espera);

	if( fila != -1 ){

		printf("Soy %ld y entre a esperar al profe;\n", index+1 );

		sem_wait(&desocupado);				//alumno espera al profe
		sem_post(&sala_de_espera);			//alumno desocupa la sala de espera

		printf("Presenta :   Soy el alumno %ld y mi nota es %d.\n", index + 1, notas[index]);
		revisar = index;
		sem_post(&nota);					//alumno entrega nota

		sem_wait(&espera_resultado);		//alumno espera resultado
		printf("\nRECIBE : Soy el alumno %ld y el profe me cambió la nota a %d\n", index + 1 ,notas[index]);

	}else{	//el profe estaba ocupado =(

		printf("********Soy el alumno %ld y el profe estaba ocupado asi que  me fui a la xaxu.\n", index + 1);

	}

	pthread_exit(NULL);
}

void crearNotas(){
	int i = 0;
	srand(time(NULL));

	for(i=0; i<MAX; i++){
		notas[i]= rand() % 101 ; 
	}
}

void delay(int x){
	int i=0;

	struct timespec t1,t2;
	t1.tv_sec =0;
	t1.tv_nsec = 1000000L ; 
	
	t1.tv_nsec = t1.tv_nsec * T;
	
	for(i=0;i<x;i++){
		nanosleep(&t1,NULL);	
	}
	
}

void imprimir_notas(){
	int i = 0;

	printf("\n");

	for(i=0; i<MAX; i++){
		printf("%d\t", notas[i]);
		
		if(((i+1)%10)==0) printf("\n");
	}
}