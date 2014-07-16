#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
#include <semaphore.h>
#include <pthread.h>

#define MAX 50	//Cantidad de alumnos
#define T 100  	//Tiempo del retardo T, en milisec (solo hasta 999)
int notas[MAX];
long revisar;
sem_t espera_resultado, desocupado, nota, sala_de_espera;

void crearNotas();
void delay(int);
void *profe(void *);
void *alumn(void *);

int main(int argc, char **argv){
	long i=0;
	pthread_t alumno[MAX], profesor;
	
	crearNotas();

	sem_init(&desocupado, 0, 1);
	sem_init(&espera_resultado, 0, 0);
	sem_init(&nota, 0, 0);
	sem_init(&sala_de_espera, 0, 3);  	//La sala de espera tiene 3 lugares
	
	//Crea el thread del profesor	
	pthread_create(&profesor, NULL, profe, NULL);

	//Crear todos los threads
	for (i = 0; i < MAX; i++) {
		delay(1);						//Crea thread de un alumno cada 1T

		if(((i+1)%10)==0){
			if(i!=0){
				delay(9);				//Agrega 9T si es que es el alumno multiplo de 10  (9T+1T)
			}
		}
		pthread_create(&alumno[i], NULL, alumn, (void *)i);
	}

	//Esperar a todos los threads
	for (i = 0; i < MAX; i++) {
		pthread_join(alumno[i], NULL);
	}

	return 0;
}

void *profe(void * params){
	
	while(1){
		sem_wait(&nota);				//El profe recibe nota

		printf("Soy el profe y le estoy revisando al alumno %ld\n", revisar+1);

		delay(3);

		notas[revisar] = notas[revisar] + ( rand() % 11 );
		if(notas[revisar]>100) notas[revisar]=100;
//		printf("Le cambie la nota a: %d\n", notas[revisar]);

		sem_post(&espera_resultado);	//El profe entrega nota lista
		sem_post(&desocupado);			//El profe se desocupa
	}
	
	
	pthread_exit(NULL);
}

void *alumn(void * params){
	
	long index = (long) params;			//Recupera el indice de la nota
	int fila;

	fila = sem_trywait(&sala_de_espera);

	if( fila != -1 ){

		printf("Espera: Soy %ld y entre a esperar al profe;\n", index+1 );

		sem_wait(&desocupado);				//Alumno espera al profe
		sem_post(&sala_de_espera);			//Alumno desocupa la sala de espera

		printf("Presenta : Soy el alumno %ld y mi nota es %d.\n", index + 1, notas[index]);
		revisar = index;
		sem_post(&nota);					//Alumno entrega nota

		sem_wait(&espera_resultado);		//Alumno espera resultado
		printf("\nRECIBE : Soy el alumno %ld y el profe me cambió la nota a %d\n", index + 1 ,notas[index]);

	}else{									//El profe estaba ocupado!!
//		printf("********Soy el alumno %ld y el profe estaba ocupado asi que  me fui a la xaxu.\n", index + 1);
	}

	pthread_exit(NULL);
}

void crearNotas(){						//LLena el arreglo notas[] (global) con notas aleatorias
	int i = 0;
	srand(time(NULL));

	for(i=0; i<MAX; i++){
		notas[i]= rand() % 101 ; 
	}
}

void delay(int x){						//Aplica un retarde de x milisegundos
	int i=0;

	struct timespec t1,t2;
	t1.tv_sec =1;
	t1.tv_nsec = 1000000L ; 
	
	t1.tv_nsec = t1.tv_nsec * T;
	
	for(i=0;i<x;i++){
		nanosleep(&t1,NULL);	
	}
}
