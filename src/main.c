#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct CatchPokemon{
	unsigned int largoNombre;
	char* nombrePokemon;

	unsigned int posX;
	unsigned int posY;

	unsigned int ID;
}

void meterItemEn(char cosaAMeter, char* laLista, int* ultimoItem){

	laLista[ *ultimoItem ] = cosaAMeter;

	*ultimoItem = *ultimoItem + 1;

}

int main(){

	char CATCH_POKEMON[20];

	int ultimoItemCATCH = 0;

	int numeroEntero = 0;

	meterItemEn('o', &CATCH_POKEMON, &ultimoItemCATCH);
	meterItemEn('z', &CATCH_POKEMON, &ultimoItemCATCH);
	meterItemEn('j', &CATCH_POKEMON, &ultimoItemCATCH);


	printf("%c ", CATCH_POKEMON[0]);
	printf("%c ", CATCH_POKEMON[1]);
	printf("%c ", CATCH_POKEMON[2]);

	printf("Numero: %i ", numeroEntero);
	printf("Tamaño de esto: %i ", sizeof(numeroEntero) );

	//printf("%i ", ultimoItemCATCH);


	// La cola guarda mensajes

	return 0;
}
