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
#include<commons/collections/list.h>

//estructura para recibir mensajes ----------------------------------------------------------------------------------------
typedef struct {
	void* mensaje;
	int32_t conexion;
	uint32_t size;
} Hilo;

//estructuras que usa socket ------------------------------------------------------------------------------------

typedef enum
{
	NEW = 1,
	APPEARED,
	GET,
	LOCALIZED,
	CATCH,
	CAUGHT,
	SUSCRIPCION,
	DESSUSCRIPCION,
	CONFIRMACION,
	TEST = 404,
	ERROR = -1,
	DESCONEXION = 0
}codigo_operacion;

typedef struct
{
	uint32_t size;
	void* stream;
} t_buffer;

typedef struct
{
	codigo_operacion codigo_op;
	t_buffer* buffer;
} t_paquete;


//estructuras del Broker -----------------------------------------------------------------------------------------------

typedef struct {
	int32_t socket;
	int32_t recibido; // 0 si no se recibio, 1 si ya se recibio
	int32_t suscripto; // 0 si no esta suscripto, 1 si esta suscripto
}t_sub;

typedef struct {
	int32_t id;
	int32_t id_correlativo;
	void* mensaje;
	uint32_t tamanioMensaje;
	t_list* subs; //lista de suscriptores
	uint32_t particion; // de 0 en adelante
}t_mensaje;

typedef struct {
	codigo_operacion tipoCola;
	t_list* mensajes; //lista de mensajes
	t_list* subs; //lista de suscriptores
}t_cola;

// Structs para manejar memoria -----------------------------------------------------------------------------------------

typedef struct {
	uint32_t limiteSuperior;
	uint32_t limiteInferior;
	uint32_t estaLibre; //es un fucking booleano de cuarta, meterle solo 0(ocupada) o 1(libre)
}particion;

/*---->NO TOCAR!!!!<---- el "lista_particiones" al lado del "typedef struct", si se saca, aparece un error extraño donde la estructura
no se reconoce a si misma cuando queremos sus campos anterior y siguiente apunten a otras instancias de la misma estructura */
typedef struct lista_particiones{
	particion laParticion;
	uint32_t numero_de_particion;
	uint32_t numero_de_victima;
	int32_t ID_MENSAJE_GUARDADO;
	struct lista_particiones* anter_particion;
	struct lista_particiones* sig_particion;
}lista_particiones;

typedef struct particionesCandidatas{
	int32_t numero_de_particion;
	uint32_t size_de_particion;
	lista_particiones* puntero_a_particion_candidata;
	struct particionesCandidatas* sig_candidata;
}particionesCandidatas;

// Structs para los tipos de mensaje ------------------------------------------------------------------------------------

typedef struct {
	int32_t id_mensaje;
	int colaMensajes;
}confirmacionMensaje;

// se tiene que mandar al broker para que sepa a que cola suscribir o desuscribir al cliente
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

	int32_t ID;
	int32_t corrID; //si no tienen ninguno pongan -2
}New;

typedef struct LocalizedPokemon // INCOMPLETO
{
	uint32_t largoNombre;
	char* nombrePokemon;

	uint32_t cantPosciciones;
	Coords* posPokemones; //esto tiene que ser una lista/array o algo, no podemos mandar un puntero ToDo

	int32_t ID;
	int32_t corrID;//si no tienen ninguno pongan -2
}Localized;

typedef struct GetPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	int32_t ID;
	int32_t corrID;//si no tienen ninguno pongan -2
}Get;

typedef struct AppearedPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	Coords posPokemon;

	int32_t ID;
	int32_t corrID;//si no tienen ninguno pongan -2
}Appeared;

typedef struct CatchPokemon
{
	uint32_t largoNombre;
	char* nombrePokemon;

	Coords posPokemon;

	int32_t ID;
	int32_t corrID;//si no tienen ninguno pongan -2
}Catch;

typedef struct CaughtPokemon // por hacer
{
	uint32_t largoNombre;
	char* nombrePokemon;

	int32_t pudoAtrapar; // BOOLEANO

	int32_t ID;
	int32_t corrID;//si no tienen ninguno pongan -2
}Caught;

// Firmas de Funciones
t_log* iniciar_logger(void);

#endif
