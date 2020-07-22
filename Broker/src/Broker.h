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
#include"shared/memoria.h" //las funciones para manejar memoria
#include"shared/estructuras.h"
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<string.h>
#include<pthread.h>
#include <inttypes.h> // Para tener el uint32_t
#include <semaphore.h> // para los semaforos
//#include<commons/collections/list.h>

t_log* logger;

sem_t* semNew;
sem_t* semAppeared;
sem_t* semGet;
sem_t* semLocalized;
sem_t* semCatch;
sem_t* semCaught;
sem_t* semId;

sem_t* semCache;
sem_t* semParticiones;

t_cola* colaDescartesNew;
t_cola* colaDescartesAppeared;
t_cola* colaDescartesGet;
t_cola* colaDescartesLocalized;
t_cola* colaDescartesCatch;
t_cola* colaDescartesCaught;

t_cola* colaNew;
t_cola* colaAppeared;
t_cola* colaGet;
t_cola* colaLocalized;
t_cola* colaCatch;
t_cola* colaCaught;

int32_t id_inicial;

//Memoria del Broker
void* CACHE;
lista_particiones* hoja_de_particiones;
uint32_t TAMANIO_MEM;
char* ALGOR_REEMPLAZO;
uint32_t FRECUEN_COMPACT;
uint32_t PARTICIONES_ELIMINADAS;
uint32_t NUMERO_VICTIMA;
uint32_t TAMANIO_MIN_PART;
char* ADMIN_MEMORIA;
char* ALGOR_ASIGN_PARTICION;

// ***********servidor***********
//Esta función arranca el servidor, solo hay que pasarle la IP y PUERTO en los que queremos que escuche.
void iniciar_server(char* ip, char* puerto); //sacar IP y PUERTO de los archivos de config.
//Esta funcion recibe el socket que preparó iniciar_server y se queda escuchando a la espera de que lleguen mensajes
void esperar_cliente(int32_t);
void serve_client(int32_t *socket);//Está atento a si llega un mensaje o el socket falla?
void process_request(codigo_operacion cod_op, int32_t socket_cliente, uint32_t sizeAAllocar);//esta cosa administra segun el tipo de mensaje que llegó, a quien llama para que lo abra.

// ***********funciones de broker***********
void inicializar_colas();
void inicializar_semaforos();
void llenar_listaColas();
t_sub crear_sub(int32_t socket);
t_mensaje* crear_mensaje(int32_t id, int32_t id_correlativo, void* mensaje, uint32_t sizeDeMensaje);
void suscribir(t_sub* sub,t_cola* cola);
void agregar_sub(int32_t socket, t_cola* cola);
void mandar_mensajes_broker(t_cola* cola);
void modificar_cola(t_cola* cola, int id_mensaje, int32_t socket);
void confirmar_mensaje(int32_t socket, confirmacionMensaje* mensaje);
int32_t a_suscribir(Suscripcion* mensaje);
int32_t a_desuscribir(Dessuscripcion* mensaje);
void desuscribir(int32_t socket, t_cola* cola);

// ***********agregar a colas***********
void agregar_mensaje_new(New* mensaje, uint32_t sizeMensaje);
void agregar_mensaje_appeared(Appeared* mensaje, uint32_t sizeMensaje);
void agregar_mensaje_get(Get* mensaje, uint32_t sizeMensaje);
void agregar_mensaje_localized(Localized* mensaje, uint32_t sizeMensaje);
void agregar_mensaje_catch(Catch* mensaje, uint32_t sizeMensaje);
void agregar_mensaje_caught(Caught* mensaje, uint32_t sizeMensaje);

void agregar_descarte (t_cola* cola, t_mensaje* descarte);
void agregar_mensajes_viejos(int32_t socket, t_cola* cola);
void buscar_mensajes_descarte(int32_t socket, t_cola* cola, t_cola* descarte);
int sub_presente(int32_t socketCliente, t_mensaje* mensaje);
// ***********borrar***********

void borrar_datos(t_cola* cola, t_mensaje* mensaje);
void borrar_datos_new(New* mensaje);
void borrar_datos_appeared(Appeared* mensaje);
void borrar_datos_get(Get* mensaje);
void borrar_datos_localized(Localized* mensaje);
void borrar_datos_catch(Catch* mensaje);
void borrar_datos_caught(Caught* mensaje);

void borrar_mensajes(t_cola* cola);

#endif /* SRC_BROKER_H_ */
