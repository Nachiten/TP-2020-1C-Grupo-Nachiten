#include "estructuras.h"

void meterItemEn(Catch cosaAMeter, struct CatchPokemon* laLista, int* ultimoItem){

	laLista[ *ultimoItem ] = cosaAMeter;

	*ultimoItem = *ultimoItem + 1;

}

// sacarItemDe(){

//}

/*int main(){

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
		printf("PosX: %i\n"        , CATCH_POKEMON[i].posPokemon.x);
		printf("PosY: %i\n"        , CATCH_POKEMON[i].posPokemon.y);
		printf("ID: %i\n"          , CATCH_POKEMON[i].ID);

	}

	return 0;
}*/
