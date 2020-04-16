#include "CUnit/Basic.h"
#include "../Broker/src/broker.c"

void probarNew(){

	char* nombrePokemon = "Pikachu";
	Coords coordenadas = {3,2};

	New unNew = {7, nombrePokemon, coordenadas, 5, 222, 3};

	CU_ASSERT_EQUAL(unNew.largoNombre, 7);
	CU_ASSERT_EQUAL(unNew.nombrePokemon, "Pikachu");
	CU_ASSERT_EQUAL(unNew.posPokemon.x, 3);
	CU_ASSERT_EQUAL(unNew.posPokemon.y, 2);
	CU_ASSERT_EQUAL(unNew.cantPokemon, 5);
	CU_ASSERT_EQUAL(unNew.ID, 222);
	CU_ASSERT_EQUAL(unNew.corrID, 3);
}
void probarGet(){
	char* nombrePokemon = "Bulbasour";

	Get unGet = {9, nombrePokemon, 345, 1};

	CU_ASSERT_EQUAL(unGet.largoNombre, 9);
	CU_ASSERT_EQUAL(unGet.nombrePokemon, "Bulbasour");

	CU_ASSERT_EQUAL(unGet.ID, 345);
	CU_ASSERT_EQUAL(unGet.corrID, 1);
}

void probarFuncionmeterItemEn(){
	Catch CATCH_POKEMON[20];

	int ultimoItemCATCH = 0;

	Catch catchPrueba1 = { 7, "Pikachu" , 1 , 5 , 69};
	Catch catchPrueba2 = { 12, "Algooooooooo" , 4 , 7 , 107};
	Catch catchPrueba3 = { 2, "Cj" , 6 , 9 , 911};

	meterItemEn(catchPrueba1 , &CATCH_POKEMON , &ultimoItemCATCH);
	meterItemEn(catchPrueba2 , &CATCH_POKEMON , &ultimoItemCATCH);
	meterItemEn(catchPrueba3 , &CATCH_POKEMON , &ultimoItemCATCH);

	CU_ASSERT_EQUAL(CATCH_POKEMON[0].largoNombre, 7);
	CU_ASSERT_EQUAL(CATCH_POKEMON[0].nombrePokemon, "Pikachu");
	CU_ASSERT_EQUAL(CATCH_POKEMON[0].posPokemon.x, 1);
	CU_ASSERT_EQUAL(CATCH_POKEMON[0].posPokemon.y, 5);
	CU_ASSERT_EQUAL(CATCH_POKEMON[0].ID, 69);

	CU_ASSERT_EQUAL(CATCH_POKEMON[1].largoNombre, 12);
	CU_ASSERT_EQUAL(CATCH_POKEMON[1].nombrePokemon, "Algooooooooo");
	CU_ASSERT_EQUAL(CATCH_POKEMON[1].posPokemon.x, 4);
	CU_ASSERT_EQUAL(CATCH_POKEMON[1].posPokemon.y, 7);
	CU_ASSERT_EQUAL(CATCH_POKEMON[1].ID, 107);

	CU_ASSERT_EQUAL(CATCH_POKEMON[2].largoNombre, 2);
	CU_ASSERT_EQUAL(CATCH_POKEMON[2].nombrePokemon, "Cj");
	CU_ASSERT_EQUAL(CATCH_POKEMON[2].posPokemon.x, 6);
	CU_ASSERT_EQUAL(CATCH_POKEMON[2].posPokemon.y, 9);
	CU_ASSERT_EQUAL(CATCH_POKEMON[2].ID, 911);

}
/*
int main(){

	CU_initialize_registry();

	CU_pSuite prueba = CU_add_suite("Suite de prueba", NULL, NULL);

	CU_add_test(prueba, "Probar Struct New", probarNew);
	CU_add_test(prueba, "Probar Struct Get", probarGet);
	CU_add_test(prueba, "Probar Funcion meterItemEn (lista)", probarFuncionmeterItemEn);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}*/
