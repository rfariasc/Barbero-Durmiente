#include <stdio.h>
#include <pthread.h>

#define MAX 9
int sudoku[MAX][MAX];

void toArray( FILE * );
void *fila(void *);
void *columna(void *);
void *miniCuadrado(void * params);

int main(int argc, char **argv)
{
	pthread_t thread1, thread2, thread3;
	int filas=0, columnas=0, miniCuadrados = 0;

	FILE *file;
	
	file = fopen(argv[1],"r");
//	file = fopen("datos1.txt", "r");
	
	if(file==NULL){
        	printf("ERROR ABRIENDO ARCHIVO\n");
        	return 1;
    	}
	toArray(file);
	fclose(file);

	pthread_create( &thread1, NULL, fila, &filas);
	pthread_create( &thread2, NULL, columna, &columnas);
	pthread_create( &thread3, NULL, miniCuadrado, &miniCuadrados);

	pthread_join( thread1,NULL);
	pthread_join( thread2,NULL);
	pthread_join( thread3,NULL);

	return 0;
}

/*
	Verifica las filas
*/
void *fila(void * params){
	pthread_t nextThread;
	int i=0, j=0, temp[MAX];
	int *a = (int *) params;
		
	if(*a> MAX-1)	pthread_exit(NULL);

	for(i=0; i< MAX; i++){
		temp[i]=sudoku[i][*a];

		for (j = i-1; j>=0; --j){   //se verifica el ultimo valor comparando con los numeros que salieron previamente

			if(temp[j] == sudoku[i][*a]){  //si hay 2 numeros iguales, el sudoku no es valido

				printf("Fila %d Hebra %ld, INVALIDA\n", *a+1, pthread_self() );
				pthread_exit(NULL);
			}
		}
	}
	printf("Fila %d Hebra %ld, VALIDA\n", *a+1,  pthread_self() );

	*a = *a+1;
	
	pthread_create( &nextThread, NULL, fila, a );
	pthread_join(nextThread, NULL);

	pthread_exit(NULL);
}

/*
	Verifica las columnas
*/
void *columna(void * params){
	pthread_t nextThread;
	int i=0, j=0, temp[MAX];
	int *a = (int *) params;
		
	if(*a> MAX-1)	pthread_exit(NULL);

	for(i=0; i< MAX; i++){
		temp[i]=sudoku[*a][i];

		for (j = i-1; j>=0; --j){	//se verifica comparando con los numeros que salieron previamente

			if(temp[j] == sudoku[*a][i]){
				printf("Columna %d Hebra %ld, INVALIDA\n", *a+1, pthread_self() );
				
				pthread_exit(NULL);
			}
		}
	}
	printf("Columna %d Hebra %ld, VALIDA\n", *a+1, pthread_self() );

	*a = *a+1;
	
	pthread_create( &nextThread, NULL, columna, a );
	pthread_join(nextThread, NULL);
	pthread_exit(NULL);
}


/*
	Verifica los mini cuadrados
*/
void *miniCuadrado(void * params){
	pthread_t nextThread;
	int i=0, j=0, k=0, count=0, temp[MAX], primero_fil=0, primero_col=0;
	int *a = (int *) params;

	switch(*a){		// indigena mode on
		case 0:
			primero_fil=0;               
			primero_col=0;
			break;
		case 1:
			primero_fil=3;               
			primero_col=0;
			break;
		case 2:
			primero_fil=6;              
			primero_col=0;
			break;
		case 3:
			primero_fil=0;               
			primero_col=3;
			break;
		case 4:
			primero_fil=3;               
			primero_col=3;
			break;
		case 5:
			primero_fil=6;               
			primero_col=3;
			break;
		case 6:
			primero_fil=0;               
			primero_col=6;
			break;
		case 7:
			primero_fil=3;               
			primero_col=6;
			break;
		case 8:
			primero_fil=6;               
			primero_col=6;
			break;

		default:
			break;
	}

	if(*a> MAX-1)	pthread_exit(NULL);

	count = 0;
	for (j = primero_col; j < primero_col + 3; ++j){
		for (i = primero_fil; i < primero_fil + 3; ++i){

			temp[count] = sudoku[i][j];  			//copia los valores del mini cuadrado en un arreglo					
			for(k = count -1 ; k >= 0 ; --k){  		//compara el ultio valor copiado con los anteriores
				if(sudoku[i][j] == temp[k]){ 		//si existe uno igual, el sudoku es invalido
					printf("Mini Cuadrado %d Hebra %ld, INVALIDA\n", *a+1, pthread_self() );					
					pthread_exit(NULL);
				}
			}
			count ++;
		}
	}

	printf("Mini Cuadrado %d Hebra %ld, VALIDA\n", *a+1, pthread_self() );

	*a = *a+1;
	
	pthread_create( &nextThread, NULL, miniCuadrado, a );
	pthread_join(nextThread, NULL);

	pthread_exit(NULL);
}


/*
	Convierte el txt en un arreglo
*/
void toArray( FILE * file){  
	int i=0, j=0, num=0;

	while (!feof (file)){  
		fscanf (file, "%d", &sudoku[i][j]);      
		i++;		

		if(i==MAX){
			j++;
			i=0;
		}
	}
}