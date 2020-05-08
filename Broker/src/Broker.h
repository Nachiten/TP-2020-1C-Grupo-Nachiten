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
#include"shared/server.h"

#define TEST_ID 1111 //hasta que se me ocurra una forma de crear los id

typedef struct {
	int sub;
	int recibido;
}t_sub;

typedef struct {
	int id;
	char* mensaje; //es un string, no olvidar el \0 al final
	t_list* subs; //lista de suscriptores
}t_mensaje;

typedef struct {
	char* nombre; //a cambiar si encuentro algo mejor
	t_list* mensajes; //lista de mensajes
	t_list* subs; //lista de suscriptores
}t_cola;

t_sub crear_sub(int socket);

#endif /* SRC_BROKER_H_ */
