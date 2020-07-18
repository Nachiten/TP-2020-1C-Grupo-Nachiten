/*
 * iniciarFS.h
 *
 *  Created on: 28 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_INICIARFS_H_
#define SRC_INICIARFS_H_

#include "gamecard.h"

// INICIALIZACION FILESYSTEM
void crearBloquesEn(char* ,int );
void inicializarFileSystem(char*, char*, char*, int);
int existeFilesystem(char* );
char* crearCarpetaEn(char* ,char* );
t_list* escanearPokemonsExistenes();


#endif /* SRC_INICIARFS_H_ */
