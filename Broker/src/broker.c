#include "estructuras.h"

typedef struct Entrenador{

	// Temporal | Num Pokedex
	int* listaPokemones;

	struct Entrenador* siguiente;
	struct Entrenador* anterior;

}Entrenador;



int main(void) {


	int i;

	/*for (i = 0; i < 10;i++){
		printf("Parte %i\n", i);

		// Pausa el programa (en segundos)
		sleep(1);

	}*/

	return 0;
}
