/*
 * colaMensajesTeam.h
 *
 *  Created on: 4 may. 2020
 *      Author: utnso
 */

#ifndef SRC_COLAMENSAJESTEAM_H_
#define SRC_COLAMENSAJESTEAM_H_

#include "estructurasTeam.h"

typedef struct nodoColaM{
    mensaje_server mensaje;
    int num_posicion;
    struct nodoColaM* next;
}mensaje_cola_team;

typedef struct{
    mensaje_cola_team* inicio;
    mensaje_cola_team* fondo;
}cola_mensajes_team;

void inicializar_cola_mensajes();
void datos_primero_cola_mensajes(mensaje_server*);
void primero_en_cola_mensajes(Appeared*);
void llenar_con_primero(Appeared*);
void agregar_a_cola_mensajes(mensaje_server*);
void actualizar_cola_mensajes();
int esta_vacia_cola_mensajes();
void eliminar_cola_mensajes();


#endif /* SRC_COLAMENSAJESTEAM_H_ */
