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

// Header files mios
#include "bitarray.h"
#include "iniciarFS.h"
#include "semaforos.h"

char* pathBloques;
char* pathMetadata;
char* pathFiles;
int BLOCKS;
int BLOCK_SIZE;
int TIEM_REIN_OPERACION;

typedef struct semaforoPokemon{
	sem_t* semaforo;
	char* nombrePokemon;
}semPokemon;

t_list* listaSemPokemon;

typedef struct datosHiloBroker{
	char* IP_BROKER;
	char* PUERTO_BROKER;
	int TIEM_REIN_CONEXION;
	t_log* logger;
}datosHiloBroker;

void escuchoSocket(int32_t miSocket); //necesario para recibir cosas desde ESE socket
void esperar_conexiones(int32_t socket_servidor);


void crearCarpetaPokemon(char*);
void crearMetadataCarpeta(char*);
void crearMetadataPokemon(char*);
void crearPokemonSiNoExiste(char* );
void escribirDatoEnBloque(char*, int);
void escribirLineaNuevaPokemon(char*, int, int, int);
void escribirLineasEnBloques(t_list*, t_list*);
void fijarBloquesA(char*, t_list*);
void leerConfig(int*, int* ,char** ,char** ,char**, t_config*);
void leerMetadataBin(char*, int* , int* , char**, t_config*);
void leerUnPokemon(char*, char*);

int cantidadDeBloquesQueOcupa(int);
int encontrarCoords(int , int, char* );
int existeCarpetaPokemon(char* );
int hayAlgunBloque(char* , char*);

char* crearStringArrayBloques(t_list*);
char* generarLineaCoordsPokemon(int, int, int);
char* separarCoord(char* );

char** leerBloques(char*);

t_list* separarStringEnBloques(char*, int);

#endif /* SRC_GAMECARD_H_ */
