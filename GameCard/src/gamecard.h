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
#include <sys/stat.h>
#include <sys/types.h>
#include <commons/bitarray.h>
#include"shared/socket.h"
#include"shared/cargador.h"
#include"shared/terminarPrograma.h"

char* crearCarpetaEn(char* ,char* );
void crearBloquesEn(char* ,int );
void leerConfig(int*, int* ,char** ,char** ,char** );
void leerMetadataBin(int* , int* , char** );
t_bitarray* crearBitArray(char*, int);


#endif /* SRC_GAMECARD_H_ */
