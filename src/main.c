#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h> // Para tener el uint32_t

struct CatchPokemon // Mensaje
{
	uint32_t largoNombre;
	char* nombrePokemon;

	uint32_t posX;
	uint32_t posY;

	uint32_t ID;
	// unsigned int corrID;

	// (id && corrID) (id,corrid) ( && && && && & & & & ) (struct1 == struct2)

	// AquienMeEnvie
	// De quien recibi ACK
};

void meterItemEn(struct CatchPokemon cosaAMeter, struct CatchPokemon* laLista, int* ultimoItem){

	laLista[ *ultimoItem ] = cosaAMeter;

	*ultimoItem = *ultimoItem + 1;

}

// sacarItemDe(){}

int main(){

	struct CatchPokemon CATCH_POKEMON[20];
	int ultimoItemCATCH = 0;

	struct CatchPokemon catchPrueba1 = { 7, "Pikachu" , 1 , 5 , 69};
	struct CatchPokemon catchPrueba2 = { 12, "Algooooooooo" , 4 , 7 , 107};
	struct CatchPokemon catchPrueba3 = { 2, "Cj" , 6 , 9 , 911};

	meterItemEn(catchPrueba1 , &CATCH_POKEMON , &ultimoItemCATCH);
	meterItemEn(catchPrueba2 , &CATCH_POKEMON , &ultimoItemCATCH);
	meterItemEn(catchPrueba3 , &CATCH_POKEMON , &ultimoItemCATCH);

	int i;
	for (i = 0; i<3; i++){

		printf("Largo nombre: %i\n", CATCH_POKEMON[i].largoNombre);
		printf("Nombre: %s\n"      , CATCH_POKEMON[i].nombrePokemon);
		printf("PosX: %i\n"        , CATCH_POKEMON[i].posX);
		printf("PosY: %i\n"        , CATCH_POKEMON[i].posY);
		printf("ID: %i\n"          , CATCH_POKEMON[i].ID);

	}

	return 0;
}
