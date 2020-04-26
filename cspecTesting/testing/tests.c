#include <cspecs/cspec.h>
#include <string.h>
#include "../../Broker/src/estructuras.h"
#include "../../Broker/src/broker.c"


// Prueba para ver el formato
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
