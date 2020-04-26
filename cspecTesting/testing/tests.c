#include <cspecs/cspec.h>
#include <string.h>
#include "../../Broker/src/estructuras.h"
#include "../../Broker/src/broker.c"


// Prueba para ver el formato
/*
context (test_de_strings) {
	describe("strlen") {
		it("devuelve la longitud de la cadena") {
			char* unaCadena = "Empanada";
			int longitud = strlen(unaCadena);
			should_int(longitud) be equal to(8);
		} end
	it("devuelve 0 para una cadena vacía") {
			should_int(strlen("")) be equal to(0);
		} end
	} end
}
*/

context (test_de_broker) {

	describe("tests") {

		it("Prueba funcion meterItemEn") {
				Catch CATCH_POKEMON[20];

				int ultimoItemCATCH = 0;

				Catch catchPrueba1 = { 7, "Pikachu" , 1 , 5 , 69};
				Catch catchPrueba2 = { 12, "Algooooooooo" , 4 , 7 , 107};
				Catch catchPrueba3 = { 2, "Cj" , 6 , 9 , 911};

				meterItemEn(catchPrueba1 , &CATCH_POKEMON , &ultimoItemCATCH);
				meterItemEn(catchPrueba2 , &CATCH_POKEMON , &ultimoItemCATCH);
				meterItemEn(catchPrueba3 , &CATCH_POKEMON , &ultimoItemCATCH);

				should_string(CATCH_POKEMON[0].nombrePokemon) be equal to("Pikachu");
				should_int(CATCH_POKEMON[0].largoNombre) be equal to(7);
				should_int(CATCH_POKEMON[0].posPokemon.x) be equal to(1);
				should_int(CATCH_POKEMON[0].posPokemon.y) be equal to(5);
				should_int(CATCH_POKEMON[0].ID) be equal to(69);

				should_string(CATCH_POKEMON[1].nombrePokemon) be equal to("Algooooooooo");
				should_int(CATCH_POKEMON[1].largoNombre) be equal to(12);
				should_int(CATCH_POKEMON[1].posPokemon.x) be equal to(4);
				should_int(CATCH_POKEMON[1].posPokemon.y) be equal to(7);
				should_int(CATCH_POKEMON[1].ID) be equal to(107);

				should_string(CATCH_POKEMON[2].nombrePokemon) be equal to("Cj");
				should_int(CATCH_POKEMON[2].largoNombre) be equal to(2);
				should_int(CATCH_POKEMON[2].posPokemon.x) be equal to(6);
				should_int(CATCH_POKEMON[2].posPokemon.y) be equal to(9);
				should_int(CATCH_POKEMON[2].ID) be equal to(911);
		} end

	} end
}
