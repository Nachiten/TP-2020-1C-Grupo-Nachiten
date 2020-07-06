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
#include <commons/bitarray.h>
#include"shared/socket.h"
#include"shared/cargador.h"
#include"shared/terminarPrograma.h"

// Header files mios
#include "bitarray.h"
#include "iniciarFS.h"

typedef struct PosicionPokemon{
	int posX;
	int posY;
	int cantidad;
}posPokemon;

void leerConfig(int*, int* ,char** ,char** ,char**, t_config*);
void leerMetadataBin(char*, int* , int* , char**, t_config*);
void leerUnPokemon(char*, char*);
void escribirLineaEnBloque(posPokemon);
void crearCarpetaPokemon(char*, char*);
void crearMetadataPokemon(char*, char*);
void crearMetadataCarpeta(char*);
void crearPokemonSiNoExiste(char* , char* );
void escribirPokemon(char* ,int , int , int  );

void escribirLineasEnBloques(t_list*, t_list*, int, char*);
void escribirDatoEnBloque(char*, int, char*);
t_list* separarStringEnBloques(char*, int, int);
void escribirLineaNuevaPokemon(char*, int, int, int, int, int, char*, char*, char*);
char* crearStringArrayBloques(t_list*);

int existeCarpetaPokemon(char* , char* );
int encontrarCoords(int , int );
char* separarCoord(char* );

#endif /* SRC_GAMECARD_H_ */
