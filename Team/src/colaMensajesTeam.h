/*
 * colaMensajesTeam.h
 *
 *  Created on: 4 may. 2020
 *      Author: utnso
 */

#ifndef SRC_COLAMENSAJESTEAM_H_
#define SRC_COLAMENSAJESTEAM_H_

#include "Team.h"

typedef struct nodoColaM{
	mensaje_server mensaje;
	struct nodoColaM* next;
}mensaje_cola_team;

typedef struct{
	mensaje_cola_team* inicio;
	mensaje_cola_team* fondo;
}cola_mensajes_team;

void inicializar_cola_mensajes_team();
int primero_de_cola(mensaje_server*);
void agregar_a_cola(mensaje_server*);
void eliminarColaMensajes();

#endif /* SRC_COLAMENSAJESTEAM_H_ */
