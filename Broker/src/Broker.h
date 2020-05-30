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
	int socket;
	int recibido; // 0 si no se recibio, 1 si ya se recibio
}t_sub;

typedef struct {
	int id;
	int id_correlativo;
	void* mensaje;
	t_list* subs; //lista de suscriptores
}t_mensaje;

typedef struct {
	codigo_operacion tipoCola;
	t_list* mensajes; //lista de mensajes
	t_list* subs; //lista de suscriptores
}t_cola;

t_cola colaNew;
t_cola colaAppeared;
t_cola colaGet;
t_cola colaLocalized;
t_cola colaCatch;
t_cola colaCaught;

// ***********servidor***********
//Esta función arranca el servidor, solo hay que pasarle la IP y PUERTO en los que queremos que escuche.
void iniciar_server(char* ip, char* puerto); //sacar IP y PUERTO de los archivos de config.
//Esta funcion recibe el socket que preparó iniciar_server y se queda escuchando a la espera de que lleguen mensajes
void esperar_cliente(int);
void serve_client(int *socket);//Está atento a si llega un mensaje o el socket falla?
void process_request(int cod_op, int cliente_fd);//esta cosa administra segun el tipo de mensaje que llegó, a quien llama para que lo abra.
void devolver_mensaje(void* mensaje_recibido, int size, int socket_cliente, codigo_operacion codigo_op);

// ***********funciones de broker***********
t_sub crear_sub(int socket);
t_mensaje crear_mensaje(int id,void* mensaje);
t_cola crear_cola(void);
void suscribir(t_sub* sub,t_cola cola);
void agregar_mensaje(void* mensaje, codigo_operacion tipo_mensaje, t_cola cola);
void agregar_sub(int socket, t_cola cola);

#endif /* SRC_BROKER_H_ */
