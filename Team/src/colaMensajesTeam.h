/*
 * colaMensajesTeam.h
 *
 *  Created on: 4 may. 2020
 *      Author: utnso
 */

#ifndef SRC_COLAMENSAJESTEAM_H_
#define SRC_COLAMENSAJESTEAM_H_

#include "estructurasTeam.h"

cola_mensajes_team* cola_indices;

void inicializar_cola_mensajes();
void datos_primero_cola_mensajes(mensaje_server**);
void primero_en_cola_mensajes(Appeared*);
void llenar_con_primero(Appeared*);
void agregar_a_cola_mensajes(mensaje_server*);
void actualizar_cola_mensajes();
int esta_vacia_cola_mensajes();
void eliminar_cola_mensajes();


#endif /* SRC_COLAMENSAJESTEAM_H_ */
