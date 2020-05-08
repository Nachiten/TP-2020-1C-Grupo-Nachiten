/*
 * server.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SHARED_SERVER_H_
#define SHARED_SERVER_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>
#include"socket.h"

pthread_t thread;



//Esta función arranca el servidor, solo hay que pasarle la IP y PUERTO en los que queremos que escuche.
void iniciar_server(char* ip, char* puerto); //sacar IP y PUERTO de los archivos de config.
//Esta funcion recibe el socket que preparó iniciar_server y se queda escuchando a la espera de que lleguen mensajes
void esperar_cliente(int);
void serve_client(int *socket);//Está atento a si llega un mensaje o el socket falla?
void process_request(int cod_op, int cliente_fd);//esta cosa administra segun el tipo de mensaje que llegó, a quien llama para que lo abra.
void devolver_mensaje(void* mensaje_recibido, int size, int socket_cliente, codigo_operacion codigo_op);


#endif /* SHARED_SERVER_H_ */
