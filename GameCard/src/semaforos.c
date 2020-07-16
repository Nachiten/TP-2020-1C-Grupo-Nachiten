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

			printf("Se encontro el pokemon: %s en la posicion: %i\n", pokemon, i);

			sem_wait(semaforoActual->semaforo);

			return;
		}
	}
	printf("No se encontro el semaforo deseado");
}

void signalSemaforoPokemon(char* pokemon){
	int i;

	for ( i = 0; i< list_size(listaSemPokemon); i++){
		semPokemon* semaforoActual = list_get(listaSemPokemon, i);

		if (strcmp(pokemon, semaforoActual->nombrePokemon) == 0){

			printf("Se encontro el pokemon: %s en la posicion: %i\n", pokemon, i);

			sem_post(semaforoActual->semaforo);

			return;
		}
	}
	printf("No se encontro el semaforo deseado");
}
