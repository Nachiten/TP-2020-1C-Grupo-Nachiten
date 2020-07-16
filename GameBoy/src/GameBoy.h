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

void hilo_recibir_mensajes(Hilo* estructura);

#endif /* SRC_GAMEBOY_H_ */
