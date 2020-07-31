/*
 * Team.h
 *
 *  Created on: 30 abr. 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include"shared/estructuras.h"

typedef struct{
    int posicion[2];
    int estado;
    int estado_block;
    char** objetivo;
    char** pokemones_actuales;
} d_entrenador;

typedef struct{
	int resultado;
        int num_envio;
}mensaje_caught;

typedef struct{
    char* pokemon;
    int cantidad_pos;
    int* posiciones;
} mensaje_server;

typedef struct{
    int ubicacion;
    char** no_necesito;
    char** necesito;
    int en_deadlock;
} deadlock_entrenador;

typedef struct{
    int posicion;
    char* pokemon;
}elemento_respuesta;

typedef struct{
    d_entrenador* entrenador;
    int pos;
    Appeared* mensaje;//
} parametros_entrenador;

typedef struct{
    d_entrenador* entrenadores;
    deadlock_entrenador* temp_entrenadores;
    int tamano_respuesta;
    elemento_respuesta* respuesta;
} parametros_deadlock;

typedef struct{
    int socket;
    int tiempo_reconexion;
} parametros_recepcion;

typedef struct{
    int* flag_conexion_broker;
    int tiempo_reconexion;
} parametros_reconexion;

enum{ESTADO_NEW, READY, EXEC, BLOCKED, EXIT};
enum{ESPERA_CAUGHT, EN_ESPERA, ACTIVO, EN_DEADLOCK};
enum{FIFO, RR, SJF_S, SJF_C};

// Hicimos nosotros | TODO
void recepcion_mensajes(parametros_recepcion* parametros);
void intento_reconexion(parametros_reconexion* parametros);


void inicializar_semaforos(int);
void destruir_semaforos(int);
void inicializar_sem_entrenadores(int, int);
void destruir_sem_entrenadores(int);
void inicializar_hilos_entrenadores(d_entrenador*, int, pthread_t*);
void* ciclo_vida_entrenador(parametros_entrenador*);
void* administrar_cola_ready(void*);
int comparar_estimaciones(int);
void libero_exec_me_agrego_a_ready_y_espero(d_entrenador* entrenador, int);
void me_agrego_a_ready_y_espero(d_entrenador* entrenador, int);
void* administrar_cola_caught(void*);
int recibir_caught(int);
void* ciclo_deadlock(parametros_deadlock*);
void realizar_intercambio_entre(d_entrenador*, char*, d_entrenador*, char*, int);
void tratar_circulos(deadlock_entrenador*, int, elemento_respuesta*, int, parametros_deadlock*, pthread_t*);
void asignar_funcion_moverse(int);
void moverse_fifo(d_entrenador*, int, int, int);
void moverse_rr(d_entrenador*, int, int, int);
void moverse_sjf_sin_d(d_entrenador*, int, int, int);
void moverse_sjf_con_d(d_entrenador*, int, int, int);

