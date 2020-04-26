#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h> // Para tener el uint32_t

typedef struct Coords{
	uint32_t x;
	uint32_t y;
}Coords;

typedef enum {
		FALSE,
		TRUE
}bool;

typedef struct NewPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	Coords posPokemon;

	uint32_t cantPokemon;

	uint32_t ID;
	uint32_t corrID;
}New;

typedef struct LocalizedPokemon // INCOMPLETO
{
	uint32_t largoNombre;
	char* nombrePokemon;

	uint32_t cantPosciciones;
	Coords* posPokemones;

	uint32_t ID;
	uint32_t corrID;
}Localized;

typedef struct GetPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	uint32_t ID;
	uint32_t corrID;
}Get;

typedef struct AppearedPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	Coords posPokemon;

	uint32_t ID;
	uint32_t corrID;
}Appeared;

typedef struct CatchPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	Coords posPokemon;

	uint32_t ID;
	uint32_t corrID;
}Catch;

typedef struct CaughtPokemon // por hacer
{
	uint32_t largoNombre;
	char* nombrePokemon;

	uint32_t pudoAtrapar; // BOOLEANO

	uint32_t ID;
	uint32_t corrID;
}Caught;

#endif
