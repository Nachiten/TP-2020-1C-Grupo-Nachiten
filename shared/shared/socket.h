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
#include<commons/memory.h>//este
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include"estructuras.h"
#include"utilidades.h"

pthread_t thread;

//los socket tienen que ser INT

int32_t establecer_conexion(char* ip, char* puerto);//se le da una IP y un PUERTO y establece la conexion
//dice el resultado de intentar conectarse a un modulo
void resultado_de_conexion(int32_t socket, t_log* logger, char* modulo); //enviarle el socket al que se intenta conectar, el logger y nombre del modulo al que intentamos conectar.
void cerrar_conexion(int32_t socket); //se le da el socket y lo libera
void mandar_mensaje(void* mensaje, codigo_operacion tipoMensaje,  int32_t socket_cliente); //se le da el mensaje, el tipo de mensaje que vamos a mandar y el socket que le dice donde mandarlo
void recibir_mensaje(void* estructura, codigo_operacion tipoMensaje, int32_t socket_cliente); //recibe un mensaje
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
uint32_t serializar_paquete_suscripcion(t_paquete* paquete, Suscripcion* cola); //serializar un mensaje suscribirse
uint32_t serializar_paquete_dessuscripcion(t_paquete* paquete, Dessuscripcion* cola); //serializar un mensaje dessuscribirse
uint32_t serializar_paquete_prueba(t_paquete* paquete, char* mensaje); //serializa mensaje de prueba

//FUNCIONES DESSERIALIZAR
//estas funciones las necesita desserializar_paquete para saber como manejar la info que le llega
void desserializar_mensaje (void* estructura, codigo_operacion tipoMensaje, int32_t socket_cliente);
void desserializar_new(New* estructura, int32_t socket_cliente);
void desserializar_appeared(Appeared* estructura, int32_t socket_cliente);
void desserializar_get(Get* estructura, int32_t socket_cliente);
void desserializar_catch(Catch* estructura, int32_t socket_cliente);
void desserializar_caught(Caught* estructura, int32_t socket_cliente);
void desserializar_suscripcion(Suscripcion* estructura, int32_t socket_cliente);
void desserializar_dessuscripcion(Dessuscripcion* estructura, int32_t socket_cliente);

void recibir_mensaje_cliente(Hilo* estructura);

#endif /* SHARED_SOCKET_H_ */
