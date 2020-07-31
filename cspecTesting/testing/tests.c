#include <cspecs/cspec.h>
#include <string.h>

// Prueba para ver el formato
context (test_de_strings) {
	describe("Pruebo el Strlen") {
		it("devuelve longitud de la cadena") {
			char* unaCadena = "Empanada";
			int longitud = strlen(unaCadena);
			should_int(longitud) be equal to(8);
		} end
	it("devuelve 0 para cadena vacía") {
			should_int(strlen("")) be equal to(0);
		} end
	} end
}
