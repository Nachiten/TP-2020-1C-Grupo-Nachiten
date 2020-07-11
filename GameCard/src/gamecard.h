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

typedef struct PosicionPokemon{
	int posX;
	int posY;
	int cantidad;
}posPokemon;

typedef struct datosHiloBroker{
	char* IP_BROKER;
	char* PUERTO_BROKER;
	int TIEM_REIN_CONEXION;
	t_log* logger;
}datosHiloBroker;

void escuchoSocket(int32_t miSocket); //necesario para recibir cosas desde ESE socket
void esperar_conexiones(int32_t socket_servidor);


void crearCarpetaPokemon(char*, char*);
void crearMetadataCarpeta(char*);
void crearMetadataPokemon(char*, char*);
void crearPokemonSiNoExiste(char* , char* );
void escribirDatoEnBloque(char*, int, char*);
void escribirLineaNuevaPokemon(char*, int, int, int, int, int, char*, char*, char*);
void escribirLineasEnBloques(t_list*, t_list*, int, char*);
void fijarBloquesYPesoA(char*, char*, t_list*, int);
void leerConfig(int*, int* ,char** ,char** ,char**, t_config*);
void leerMetadataBin(char*, int* , int* , char**, t_config*);
void leerUnPokemon(char*, char*);

int cantidadDeBloquesQueOcupa(int, int);
int encontrarCoords(int , int );
int existeCarpetaPokemon(char* , char* );
int hayAlgunBloque(char* , char*);

char* crearStringArrayBloques(t_list*);
char* generarLineaCoordsPokemon(int, int, int);
char* separarCoord(char* );

char** leerBloques(char* , char*);

t_list* separarStringEnBloques(char*, int, int);

#endif /* SRC_GAMECARD_H_ */
