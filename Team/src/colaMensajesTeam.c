/*
 * colaMensajesTeam.c
 *
 *  Created on: 4 may. 2020
 *      Author: utnso
 */

#include "colaMensajesTeam.h"

cola_mensajes_team cola_indices;

void inicializar_cola_mensajes(){
    cola_indices.inicio = NULL;
    cola_indices.fondo = NULL;
}

void datos_primero_cola_mensajes(mensaje_server* msj){
    *msj = cola_indices.inicio->mensaje;
}

void primero_en_cola_mensajes(Appeared* msj){
    llenar_con_primero(msj);
    cola_indices.inicio->num_posicion+=1;
}

void llenar_con_primero(Appeared* msj){
    int index = cola_indices.inicio->num_posicion;
    msj->posPokemon.x = cola_indices.inicio->mensaje.posiciones[2*index];
    msj->posPokemon.y = cola_indices.inicio->mensaje.posiciones[(2*index)+1];
    msj->nombrePokemon = cola_indices.inicio->mensaje.pokemon;
}

void agregar_a_cola_mensajes(mensaje_server* contenido){
    int i;
    int* nuevoVectorInt;
    mensaje_cola_team* nuevoMensaje;

    nuevoVectorInt = malloc(2 * contenido->cantidad_pos * sizeof(int));
    for(i=0;i<(2*contenido->cantidad_pos);i++){
	nuevoVectorInt[i] = contenido->posiciones[i];
    }

    nuevoMensaje = malloc(sizeof(mensaje_cola_team));
    nuevoMensaje->mensaje = *contenido;
    nuevoMensaje->mensaje.posiciones = nuevoVectorInt;
    nuevoMensaje->num_posicion = 0;//
    nuevoMensaje->next = NULL;

    if(esta_vacia_cola_mensajes() == 1){
	cola_indices.inicio = nuevoMensaje;
	cola_indices.fondo = nuevoMensaje;
    }
    else{
	cola_indices.fondo->next = nuevoMensaje;
	cola_indices.fondo = nuevoMensaje;
    }
}

void actualizar_cola_mensajes(){
    mensaje_cola_team* temp = cola_indices.inicio;
    cola_indices.inicio = cola_indices.inicio->next;
    free(temp);
}

int esta_vacia_cola_mensajes(){
    int respuesta = 0;
    if(cola_indices.inicio == NULL){
	respuesta = 1;
    }
    return respuesta;
}

void eliminar_cola_mensajes(){
    mensaje_cola_team* actual = cola_indices.inicio;
    while(actual != NULL){
        mensaje_cola_team* temp = actual;
	actual = actual->next;
	free(temp->mensaje.posiciones);
	free(temp);
    }
}






