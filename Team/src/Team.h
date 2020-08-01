/*
 * Team.h
 *
 *  Created on: 30 abr. 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"shared/estructuras.h"
#include"estructurasTeam.h" //todo Estructuras que estaban declaradas aca en team.h volaron a estructurasTeam.h
#include"config.h"
#include"colaCaught.h"
#include"colaReady.h"
#include"colaMensajesTeam.h"
#include"deadlock.h"
#include"shared/socket.h"
#include "funciones_team.h"
#include <pthread.h>

//variables globales
char* ip;
char* puerto;
int32_t socketBroker;
uint32_t tiempo_reconexion;


void inicializar_semaforos(int);
void destruir_semaforos(int);
void inicializar_sem_entrenadores(int, int);
void destruir_sem_entrenadores(int);
void inicializar_hilos_entrenadores(d_entrenador*, int, pthread_t*);
void* ciclo_vida_entrenador(parametros_entrenador*);
void administrar_cola_ready(void*);
int comparar_estimaciones(int);
void libero_exec_me_agrego_a_ready_y_espero(d_entrenador* entrenador, int);
void me_agrego_a_ready_y_espero(d_entrenador* entrenador, int);
void administrar_cola_caught(void*);
int recibir_caught(int);
void* ciclo_deadlock(parametros_deadlock*);
void realizar_intercambio_entre(d_entrenador*, char*, d_entrenador*, char*, int);
void tratar_circulos(deadlock_entrenador*, int, elemento_respuesta*, int, parametros_deadlock*, pthread_t*);
void asignar_funcion_moverse(int);
void moverse_fifo(d_entrenador*, int, int, int);
void moverse_rr(d_entrenador*, int, int, int);
void moverse_sjf_sin_d(d_entrenador*, int, int, int);
void moverse_sjf_con_d(d_entrenador*, int, int, int);

// Hicimos las firmas nosotros | TODO
void recepcion_mensajes(void* argumento_de_adorno);
void intento_reconexion(parametros_reconexion* parametros);
void procesar_mensaje(codigo_operacion codigo, int32_t socket);

