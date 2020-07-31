/*
 * Team.h
 *
 *  Created on: 30 abr. 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
//#include <windows.h>
#include <semaphore.h>
#include"shared/estructuras.h"

typedef struct{
	int posicion[2];
    int estado;
    int estado_block;
    char** objetivo;
    char** pokemones_actuales;
} d_entrenador;

//cambiar las funciones que usan este tipo de dato y reemplazarlo por el Caught que se tiene en estructuras.h
typedef struct{
	int resultado;
	int num_envio;//OJO el valor -1 esta reservado para mensaje de comportamiento default, en ese caso resultado es la posicion del entrenador
}mensaje_caught;

typedef struct{
    int posicion;//este valor es un dato_extra que eventualmente podria ser un void*
    void* mensaje;
}mensaje_broker;

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
    void* mensaje_a_enviar;
    int codigo;
} parametros_emisor;

typedef struct{
    char* pokemon;
    int repeticiones;
    int estado_en_memoria;
    int id_mensaje;
} elemento_objetivo;

enum{ESTADO_NEW, READY, EXEC, BLOCKED, EXIT};
//enum{APPEARED, GET, LOCALIZED, CATCH, CAUGHT};
enum{ESPERA_CAUGHT, EN_ESPERA, ACTIVO, EN_DEADLOCK};
enum{FIFO, RR, SJF_S, SJF_C};

void inicializar_semaforos(int);
void destruir_semaforos(int);
void inicializar_sem_entrenadores(int, int);
void destruir_sem_entrenadores(int);
void inicializar_hilos_entrenadores(d_entrenador*, int, pthread_t*);
void* ciclo_vida_entrenador(parametros_entrenador*);
void* administrar_cola_ready(void*);
void procesar_mensaje_server(void*);
void procesar_mensaje_caught(Caught*);
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
