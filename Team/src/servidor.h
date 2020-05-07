/*
 * servidor.h
 *
 *  Created on: 3 may. 2020
 *      Author: utnso
 */

#ifndef SRC_SERVIDOR_H_
#define SRC_SERVIDOR_H_

#include "Team.h"
void recibir_mensajes(int, int, mensaje_server**, int*, int*);
int filtrar_mensaje(int, int, int, void*, char** , int);
void liberar_servidor(int);
void liberarFull();

#endif /* SRC_SERVIDOR_H_ */
