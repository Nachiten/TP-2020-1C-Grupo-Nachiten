#include "estructuras.h"

typedef struct Entrenador{

	// Temporal | Num Pokedex
	int* listaPokemones;

	struct Entrenador* siguiente;
	struct Entrenador* anterior;

}Entrenador;




void meterItemEn(Catch cosaAMeter, struct CatchPokemon* laLista, int* ultimoItem){

	laLista[ *ultimoItem ] = cosaAMeter;

	*ultimoItem = *ultimoItem + 1;

}



//sacarItemDe(){ //TODO

//}

void meterItemEnCola( Entrenador** fin, Entrenador* aMeter){

	(*fin)->anterior = aMeter; // entr2 es lo que se quiere meter

	*fin = aMeter;

}

//TODO Hacer que meter un item en la cola (el primero) se haga con esta funion y no con la forma dentro de main
//void meterPrimerItem(Entrenador* aMeter, Entrenador* inicio, Entrenador* fin){
	// Inizializando la cola con el entr1
	//inicio = aMeter;
	//fin = aMeter;
//}


int main(){

	// Cola de entrenadores punteros
	Entrenador* inicio;
	Entrenador* fin;

	int nums1[3] = {1,2,3};
	int nums2[3] = {4,5,6};
	int nums3[3] = {7,8,9};

	Entrenador entr1 = {&nums1, NULL, NULL};
	Entrenador entr2 = {&nums2, NULL, NULL};
	Entrenador entr3 = {&nums3, NULL, NULL};

	meterPrimerItem(&entr1, inicio, fin);

	// METER PRIMER ITEM EN COLA
	inicio = &entr1;
	fin = &entr1;

	meterItemEnCola(&fin, &entr2);
	meterItemEnCola(&fin, &entr3);

	//inicio =

	//fin->anterior = &entr2; // entr2 es lo que se quiere meter
	//fin = &entr2;

	//fin->anterior = &entr3; // entr2 es lo que se quiere meter
	//fin = &entr3;


	printf("Fin numero: %i\n", fin->listaPokemones[0]);

	// EXPLOTO ACA

	printf("Inicio numero: %i\n", inicio->listaPokemones[0]);

	//meterItemEnCola(fin, &entr2);
	//meterItemEnCola(fin, &entr3);

	// 2
	//printf(fin->listaPokemones[0]);




	return 0;
}
