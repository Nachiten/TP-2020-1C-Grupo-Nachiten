/*
 * estructurasTeam.h
 *
 *  Created on: 31 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_ESTRUCTURASTEAM_H_
#define SRC_ESTRUCTURASTEAM_H_

#include"shared/estructuras.h"


typedef struct{
    char* pokemon;
    int cantidad_pos;
    int* posiciones;
} mensaje_server;

typedef struct nodoColaM{
    mensaje_server* mensaje;
    int num_posicion;
    struct nodoColaM* next;
}mensaje_cola_team;

typedef struct{
    mensaje_cola_team* inicio;
    mensaje_cola_team* fondo;
}cola_mensajes_team;

typedef struct{
	int numeroEntrenador;
	int cantCiclosCPU;
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
    int flag_conexion_broker;
    int tiempo_reconexion;
} parametros_reconexion;



enum{ESTADO_NEW, READY, EXEC, BLOCKED, EXIT};
enum{ESPERA_CAUGHT, EN_ESPERA, ACTIVO, EN_DEADLOCK};
enum{FIFO, RR, SJF_S, SJF_C};



#endif /* SRC_ESTRUCTURASTEAM_H_ */
