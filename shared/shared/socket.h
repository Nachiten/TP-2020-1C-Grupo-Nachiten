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
#include"utilidades.h"

typedef enum
{
	NEW = 1,
	APPEARED,
	GET,
	LOCALIZED,
	CATCH,
	CAUGHT,
	TEST = 404,
	ERROR = -1,
	DESCONEXION = 0
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

//los socket tienen que ser INT

uint32_t establecer_conexion(char* ip, char* puerto);//se le da una IP y un PUERTO y establece la conexion
//dice el resultado de intentar conectarse a un modulo
void resultado_de_conexion(uint32_t socket, t_log* logger, char* modulo); //enviarle el socket al que se intenta conectar, el logger y nombre del modulo al que intentamos conectar.
void cerrar_conexion(uint32_t socket); //se le da el socket y lo libera
void mandar_mensaje(void* mensaje, codigo_operacion tipoMensaje,  uint32_t socket_cliente); //se le da el mensaje, el tipo de mensaje que vamos a mandar y el socket que le dice donde mandarlo
//void* recibir_mensaje(int socket_cliente, uint32_t* size); //recibe un mensaje
void eliminar_paquete(t_paquete* paquete); //libera la memoria utilizada para manejar los paquetes


//FUNCIONES SERIALIZAR

//se le da el paquete, el "algo" que vamos a mandar, tipo de mensaje y un size para que "anote" cuanto pesa el paquete despues de serializarlo.
void* serializar_paquete(t_paquete* paquete, void* mensaje, codigo_operacion tipoMensaje, uint32_t *size_serializado);//prepara paquete y deja listo para mandar

//estas funciones las necesita serializar_paquete para saber que empaquetar

uint32_t serializar_paquete_new(t_paquete* paquete, New* pokemon); //serializa un mensaje New
uint32_t serializar_paquete_appeared(t_paquete* paquete, Appeared* pokemon); //serializa un mensaje Appeared
uint32_t serializar_paquete_get(t_paquete* paquete, Get* pokemon); //serializar un mensaje Get
uint32_t serializar_paquete_catch(t_paquete* paquete, Catch* pokemon); //serializar un mensaje Catch
uint32_t serializar_paquete_caught(t_paquete* paquete, Caught* pokemon); //serializar un mensaje Caught
uint32_t serializar_paquete_prueba(t_paquete* paquete, char* mensaje); //serializa mensaje de prueba

//FUNCIONES DESSERIALIZAR

//esto convierte lo que queda del mensaje en una estructura con los datos acomodados y bien piola
//void* desserializar_mensaje (void* restoDelMensaje, codigo_operacion tipoMensaje, uint32_t size);

//estas funciones las necesita desserializar_paquete para saber como manejar la info que le llega

//void desserializar_appeared(void* restoDelMensaje, Appeared *estructura, uint32_t* size);


//version 2
void recibir_mensaje(void* estructura, int socket_cliente, uint32_t* size);
void desserializar_mensaje (void* estructura, codigo_operacion tipoMensaje, uint32_t* size, int socket_cliente);
void desserializar_new(New* estructura, uint32_t* size, int socket_cliente);
void desserializar_appeared(Appeared* estructura, uint32_t* size, int socket_cliente);
void desserializar_get(Get* estructura, uint32_t* size, int socket_cliente);


#endif /* SHARED_SOCKET_H_ */
