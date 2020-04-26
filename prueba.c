#include <cspecs/cspec.h>
#include <string.h>
#include "Broker/src/estructuras.h"

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

				should_int(strlen("")) be equal to(0);
		} end


		it("devuelve 0 para una cadena vacía") {
				should_int(CATCH_POKEMON[0].largoNombre) be equal to(7);
		} end
	} end
}
