#include "semaforos.h"

void crearSemaforoPokemon(char* pokemon){
	semPokemon* semaforoPokemon = malloc(sizeof(semPokemon));

	sem_t* semaforoNuevo = malloc(sizeof(sem_t));

	sem_init(semaforoNuevo, 0, 1);

	semaforoPokemon->nombrePokemon = pokemon;
	semaforoPokemon->semaforo = semaforoNuevo;

	list_add(listaSemPokemon, semaforoPokemon);
}

void waitSemaforoPokemon(char* pokemon){
	int i;

	for ( i = 0; i< list_size(listaSemPokemon); i++){
		semPokemon* semaforoActual = list_get(listaSemPokemon, i);

		if (strcmp(pokemon, semaforoActual->nombrePokemon) == 0){

			sem_wait(semaforoActual->semaforo);

			//printf("WAIT | %s\n", pokemon);

			return;
		}
	}
	//printf("No se encontro el semaforo deseado\n");
}

void signalSemaforoPokemon(char* pokemon){
	int i;

	for ( i = 0; i< list_size(listaSemPokemon); i++){
		semPokemon* semaforoActual = list_get(listaSemPokemon, i);

		if (strcmp(pokemon, semaforoActual->nombrePokemon) == 0){

			sem_post(semaforoActual->semaforo);

			//printf("SIGNAL | %s\n", pokemon);

			return;
		}
	}
	//printf("No se encontro el semaforo deseado\n");
}

void leerSemaforosLista(){

	int i;

	for (i=0 ; i<list_size(listaSemPokemon) ; i++){
		semPokemon* structSemaforoActual = list_get(listaSemPokemon, i);

		int* valorSemaforo = malloc(sizeof(int));

		sem_getvalue(structSemaforoActual->semaforo, valorSemaforo);

		printf("Valor semaforo: %i es: %i\n", i, *valorSemaforo);
	}

}

void printearSemaforosExistentes(){

	int i;

	for (i=0 ; i<list_size(listaSemPokemon) ; i++){
		semPokemon* structSemaforoActual = list_get(listaSemPokemon, i);

		char* nombreActual = structSemaforoActual->nombrePokemon;

		printf("Semaforo: %s\n", nombreActual);

	}

}

void crearSemaforosDePokemonesExistentes()
{
	t_list* listaPokemons = escanearPokemonsExistenes();

	int i;

	int cantElementos = list_size(listaPokemons);

	for(i = cantElementos - 1; i >= 0; i--)
	{
		char* unPokemon = list_remove(listaPokemons, i);

		crearSemaforoPokemon(unPokemon);
	}

	list_destroy(listaPokemons);
}



