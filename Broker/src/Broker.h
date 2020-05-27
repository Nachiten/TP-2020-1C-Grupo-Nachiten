/*
 * Broker.h
 *
 *  Created on: 30 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_BROKER_H_
#define SRC_BROKER_H_

#include"shared/socket.h"
#include"shared/cargador.h"
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>
#include"shared/estructuras.h"
#include <inttypes.h> // Para tener el uint32_t

#define TEST_ID 1111 //hasta que se me ocurra una forma de crear los id

typedef struct {
	int sub;
	int recibido;
}t_sub;

typedef struct {
	int id; // a borrar
	int id_correlativo;
	void* mensaje;
	t_list* subs; //lista de suscriptores
}t_mensaje;

typedef struct {
	t_list* mensajes; //lista de mensajes
	t_list* subs; //lista de suscriptores
}t_cola;

t_cola colaNew;
t_cola colaAppeared;
t_cola colaGet;
t_cola colaLocalized;
t_cola colaCatch;
t_cola colaCaught;


#endif /* SRC_BROKER_H_ */
