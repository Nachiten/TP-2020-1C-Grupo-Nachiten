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
#include"shared/socket.h"
#include"shared/cargador.h"
#include"shared/terminarPrograma.h"


char* crearCarpetaEn(char* pathPuntoMontaje, char* nombreCarpeta);
void crearBloquesEn(char* pathBloques, int cantidadBloques);

#endif /* SRC_GAMECARD_H_ */
