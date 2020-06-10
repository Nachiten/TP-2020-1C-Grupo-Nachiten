#ifndef SRC_ESTRUCTURAS_H_
#define SRC_ESTRUCTURAS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h> // Para tener el uint32_t
#include "commons/log.h"
#include "commons/string.h"
#include "commons/config.h"
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <readline/readline.h>


// Structs
typedef struct {
	int32_t id_mensaje;
	int colaMensajes;
}confirmacionMensaje;

// se tiene que mandar al broker para que sepa a que cola suscribir al cliente
typedef struct{
	int32_t numeroCola;
}Suscripcion;

typedef struct{
	int32_t numeroCola;
}Dessuscripcion;

typedef struct Coords{
	uint32_t x;
	uint32_t y;
}Coords;

typedef struct NewPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	Coords posPokemon;

	uint32_t cantPokemon;

	uint32_t ID; // ver si hay que sacar esto porque no se usaria
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

	int32_t pudoAtrapar; // BOOLEANO

	uint32_t ID;
	uint32_t corrID;
}Caught;

// Firmas de Funciones
t_log* iniciar_logger(void);

#endif
