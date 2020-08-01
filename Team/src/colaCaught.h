/*
 * colaCaught.h
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_COLACAUGHT_H_
#define SRC_COLACAUGHT_H_

#include"estructurasTeam.h"
#include <semaphore.h>

typedef struct nodoColaC{
    mensaje_caught mensaje;
    struct nodoColaC* next;
}elemento_cola_caught;

typedef struct{
    elemento_cola_caught* inicio;
    elemento_cola_caught* fondo;
}cola_Caught;

void inicializar_cola_caught(int);
void eliminar_cola_caught();
int esta_vacia_cola_caught();
void actualizar_cola_caught();
void agregar_a_cola_caught(mensaje_caught*);
int resultado_primero_cola_caught();
int posicion_primero_cola_caught();
void inicializar_vector_envios(int);
void liberar_vector_envios();
void armar_enviar_catch(char*, int, int, int, int);

#endif /* SRC_COLACAUGHT_H_ */
