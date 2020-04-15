#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h> // Para tener el uint32_t

typedef struct Coords{
	int x;
	int y;
}Coords;

typedef struct CatchPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	Coords coords;

	uint32_t ID;
	uint32_t corrID;
}Catch;

typedef struct NewPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	Coords coords;

	uint32_t cantPokemon;

	uint32_t ID;
	uint32_t corrID;
}New;

typedef struct LocalizedPokemon // INCOMPLETO
{
	uint32_t largoNombre;
	char* nombrePokemon;

	Coords coords;

	uint32_t cantPosciciones;

	uint32_t ID;
	uint32_t corrID;
}Localized;


void meterItemEn(Catch cosaAMeter, struct CatchPokemon* laLista, int* ultimoItem){

	laLista[ *ultimoItem ] = cosaAMeter;

	*ultimoItem = *ultimoItem + 1;

}

// sacarItemDe(){

//}

int main(){

	Catch CATCH_POKEMON[20];
	int ultimoItemCATCH = 0;

	Catch catchPrueba1 = { 7, "Pikachu" , 1 , 5 , 69};
	Catch catchPrueba2 = { 12, "Algooooooooo" , 4 , 7 , 107};
	Catch catchPrueba3 = { 2, "Cj" , 6 , 9 , 911};

	meterItemEn(catchPrueba1 , &CATCH_POKEMON , &ultimoItemCATCH);
	meterItemEn(catchPrueba2 , &CATCH_POKEMON , &ultimoItemCATCH);
	meterItemEn(catchPrueba3 , &CATCH_POKEMON , &ultimoItemCATCH);

	int i;
	for (i = 0; i<3; i++){

		printf("Largo nombre: %i\n", CATCH_POKEMON[i].largoNombre);
		printf("Nombre: %s\n"      , CATCH_POKEMON[i].nombrePokemon);
		printf("PosX: %i\n"        , CATCH_POKEMON[i].coords.x);
		printf("PosY: %i\n"        , CATCH_POKEMON[i].coords.y);
		printf("ID: %i\n"          , CATCH_POKEMON[i].ID);

	}

	return 0;
}
