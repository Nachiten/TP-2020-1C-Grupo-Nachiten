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
void test3(){
	printf("Hola, soy el test 3 ");
}

int main(){

	CU_initialize_registry();

	CU_pSuite prueba = CU_add_suite("Suite de prueba", NULL, NULL);

	CU_add_test(prueba, "Probar Struct New", probarNew);
	CU_add_test(prueba, "Probar Struct Get", probarGet);
	//CU_add_test(prueba, "tres", test3);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}
