/*
 * GameBoy.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_GAMEBOY_H_
#define SRC_GAMEBOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include"shared/socket.h"
#include"shared/cargador.h"
#include"shared/terminarPrograma.h"
#include"shared/estructuras.h"
#include"shared/utilidades.h"

char* IP;
char* PUERTO;

typedef struct mensajesRecibidos{
	int32_t ID_MENSAJE_RECIBIDO;
	struct mensajesRecibidos* siguiente;
}mensajesRecibidos;

typedef struct {
	int32_t conexion;
	t_log* log;
	mensajesRecibidos* listaRecibidos;
	uint32_t cola;
	uint32_t pID;
} HiloGameboy;

void hilo_recibir_mensajes(HiloGameboy* estructura);

#endif /* SRC_GAMEBOY_H_ */
