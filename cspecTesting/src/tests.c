#include <cspecs/cspec.h>
#include <string.h>
#include "../../GameCard/src/gamecard.h"

// Test de prueba para entender el formato
context (tests_varios) {
	describe("Pruebo el Strlen") {

		it("devuelve la longitud de la cadena") {
			char* unaCadena = "Empanada";
			int longitud = strlen(unaCadena);
			should_int(longitud) be equal to(8);
		} end

		it("devuelve 0 para una cadena vacía") {
				should_int(strlen("")) be equal to(0);
		} end

		it("Pruebo el separarCoords"){
			char* misCoords = separarCoord("1-2=20");
			should_string(misCoords) be equal to("1-2");
		} end



	} end


}


