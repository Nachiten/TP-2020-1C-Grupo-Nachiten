/*
 * socket.h
 *
 *  Created on: 17 abr. 2020
 *      Author: utnso
 */

#ifndef SHARED_SOCKET_H_
#define SHARED_SOCKET_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include"estructuras.h"

/* ESTO CREO QUE NO HACE FALTA, SOLO ESTABA POR EL SERVER
#include<pthread.h>
*/

char* saludo(); //volar a la mierda

typedef enum
{
	NEW,
	APPEARED,
	GET,
	LOCALIZED,
	CATCH,
	CAUGHT,
	TEST = 404
}codigo_operacion;

typedef struct
{
	uint32_t size;
	void* stream;
} t_buffer;

typedef struct
{
	codigo_operacion codigo_op;
	t_buffer* buffer;
} t_paquete;

pthread_t thread;

<<<<<<< HEAD
int establecer_conexion(char* ip, char* puerto);//se le da una IP y un PUERTO y establece la conexion
//dice el resultado de intentar conectarse a un modulo
void resultado_de_conexion(int socket, t_log* logger, char* modulo); //enviarle el socket al que se intenta conectar, el logger y nombre del modulo al que intentamos conectar.
void cerrar_conexion(int socket); //se le da el socket y lo libera
void mandar_mensaje(char* mensaje, codigo_operacion tipoMensaje,  int socket_cliente); //se le da el mensaje, el tipo de mensaje que vamos a mandar y el socket que le dice donde mandarlo
=======
uint32_t establecer_conexion(char* ip, char* puerto);//se le da una IP y un PUERTO y establece la conexion
//dice el resultado de intentar conectarse a un modulo
void resultado_de_conexion(uint32_t socket, t_log* logger, char* modulo); //enviarle el socket al que se intenta conectar, el logger y nombre del modulo al que intentamos conectar.
void cerrar_conexion(uint32_t socket); //se le da el socket y lo libera
void mandar_mensaje(void* mensaje, codigo_operacion tipoMensaje,  uint32_t socket_cliente); //se le da el mensaje, el tipo de mensaje que vamos a mandar y el socket que le dice donde mandarlo
>>>>>>> mensajes
//se le da el paquete, el mensaje, que clase de mensaje es, y un int para que "anote" cuanto pesa el paquete despues de serializarlo. y deja listo para mandar
void* serializar_paquete(t_paquete* paquete, void* mensaje, codigo_operacion tipoMensaje, uint32_t *size_serializado);
char* recibir_mensaje(uint32_t socket_cliente, uint32_t* size); //recibe un mensaje
void eliminar_paquete(t_paquete* paquete); //libera la memoria utilizada para manejar los paquetes

<<<<<<< HEAD
#endif /* SHARED_SOCKET_H_ */
=======
//serializar en prueba
uint32_t serializar_paquete_appeared(t_paquete* paquete, Appeared* pokemon);
uint32_t serializar_paquete_prueba(t_paquete* paquete, char* mensaje);

#endif /* SHARED_SOCKET_H_ */
>>>>>>> mensajes
