/*
 * colaMensajesTeam.c
 *
 *  Created on: 4 may. 2020
 *      Author: utnso
 */

#include "colaMensajesTeam.h"

mensaje_server agarrar_primero();
void actualizar_cola();
int esta_vacia();

cola_mensajes_team cola_indices;

void inicializar_cola_mensajes_team(){
	cola_indices.inicio = NULL;
	cola_indices.fondo = NULL;
}

int primero_de_cola(mensaje_server* mens){
	int respuesta = 1;
	if(esta_vacia() == 1){
		respuesta = -1;
	}
	else{
		*mens = agarrar_primero();
		actualizar_cola();
	}
	return respuesta;

}

void agregar_a_cola(mensaje_server* contenido){
	mensaje_cola_team* nuevoMensaje = malloc(sizeof(mensaje_cola_team));
	int* nuevoVectorInt = malloc(sizeof(int)*2*contenido->cantidad_pos);
	int i;
	for(i=0;i<(2*contenido->cantidad_pos);i++){
		nuevoVectorInt[i] = contenido->posiciones[i];
	}
	nuevoMensaje->mensaje = *contenido;
	nuevoMensaje->mensaje.posiciones = nuevoVectorInt;
	nuevoMensaje->next = NULL;
	if(esta_vacia() == 1){
		cola_indices.inicio = nuevoMensaje;
		cola_indices.fondo = nuevoMensaje;
	}
	else{
		cola_indices.fondo->next = nuevoMensaje;
		cola_indices.fondo = nuevoMensaje;
	}

	free(nuevoMensaje);
	free(nuevoVectorInt);
}

mensaje_server agarrar_primero(){
	return(cola_indices.inicio->mensaje);
}

void actualizar_cola(){
	mensaje_cola_team* temp = cola_indices.inicio;
	cola_indices.inicio = cola_indices.inicio->next;
	free(temp);
}

int esta_vacia(){
	int respuesta = 0;
	if(cola_indices.inicio == NULL){
		respuesta = 1;
	}
	return respuesta;
}

void eliminarColaMensajes(){
	mensaje_cola_team* actual = cola_indices.inicio;
	while(actual != NULL){
		mensaje_cola_team* temp = actual;
		actual = actual->next;
		free(temp->mensaje.posiciones);
		free(temp);
	}
}







