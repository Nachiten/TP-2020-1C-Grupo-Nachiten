/*
 * GameCard.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMECARD_H_
#define SRC_GAMECARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <commons/bitarray.h>
#include"shared/socket.h"
#include"shared/cargador.h"
#include"shared/terminarPrograma.h"
#include <semaphore.h> // para los semaforos
#include <dirent.h> // Para poder escanear carpetas

// Header files mios
#include "bitarray.h"
#include "iniciarFS.h"
#include "semaforos.h"
#include "mensajes.h"
#include "conexiones.h"

char* pathBloques;
char* pathMetadata;
char* pathFiles;
int BLOCKS;
int BLOCK_SIZE;
int TIEM_REIN_OPERACION;
char* IP_BROKER;
char* PUERTO_BROKER;

int socketNew;
int socketGet;
int socketCatch;

typedef struct semaforoPokemon{
	sem_t* semaforo;
	char* nombrePokemon;
}semPokemon;

t_list* listaSemPokemon;
t_log* logger;

// Leer bloques pokemon
char* leerContenidoBloquesPokemon(char**, int);
char* leerContenidoDeUnBloque(char*, int);

// Funciones usadas por los mensajes
char* sumarALineaPokemon(char*, int, int, int);
char* restarALineaPokemon(char*, int, int);
void sumarBloquesExtraALista(t_list*, int);
void liberarNBloques(t_list*, int);
t_list* convertirAListaDeCoords(char*);
t_list* convertirAListaDeEnterosDesdeChars(char** listaDeChars);

void crearCarpetaPokemon(char*);
void crearMetadataCarpeta(char*);
void crearMetadataPokemon(char*);
void crearPokemonSiNoExiste(char* );
void escribirDatoEnBloque(char*, int);
void escribirLineaNuevaPokemon(char*, int, int, int);
void escribirLineasEnBloques(t_list*, t_list*);
void leerConfig(int*, int* ,char** ,char** ,char**, t_config*);
void leerMetadataBin(char*, int* , int* , char**, t_config*);
void leerUnPokemon(char*, char*);

int cantidadDeBloquesQueOcupa(int);
int cantidadDeElementosEnArray(char** array);
int encontrarCoords(int , int, char* );
int existeCarpetaPokemon(char* );
int hayAlgunBloque(char* , char*);

char* agregarNuevoPokemonALineas(int, int, int, char*);
char* crearStringArrayBloques(t_list*);
char* generarLineaCoordsPokemon(int, int, int);
char* separarCoord(char* );

// Leer y escribir datos del matadata.bin de un pokemon
char** leerBloquesPokemon(char*);
int leerSizePokemon(char*);

void fijarSizeA(char*, int);
void fijarBloquesA(char*, t_list*);

t_list* separarStringEnBloques(char*, int);

// Apertura archivos
void abrirArchivoPokemon(char*);
void cerrarArchivoPokemon(char*);

#endif /* SRC_GAMECARD_H_ */
