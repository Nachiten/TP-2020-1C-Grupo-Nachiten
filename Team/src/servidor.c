/*
 * servidor.c
 *
 *  Created on: 3 may. 2020
 *      Author: Andres
 */

#include "servidor.h"

mensaje_server* mensajesAppeared;
mensaje_server* mensajesLocalized;
int* mensajesAppeared_sub1;
int* mensajesLocalized_sub1;

void recibir_mensajes(int socket, int i, mensaje_server** destino, int* id, int* idReal){
	int aux_id;
	switch(i){
		case 0:
			aux_id = 20;
			mensajesLocalized = malloc(sizeof(mensaje_server));
			mensajesLocalized->pokemon = "Pikachu";
			mensajesLocalized->cantidad_pos = 2;
			mensajesLocalized_sub1 = malloc(2 * mensajesLocalized->cantidad_pos * sizeof(int));
			mensajesLocalized->posiciones = mensajesLocalized_sub1;
			mensajesLocalized->posiciones[0] = 3; mensajesLocalized->posiciones[1] = 2;
			mensajesLocalized->posiciones[2] = 1; mensajesLocalized->posiciones[3] = 0;
			*destino = mensajesLocalized;
			//*id = LOCALIZED;
			*id = 1;
			break;
		case 1:
			aux_id = 20;
			mensajesLocalized = malloc(sizeof(mensaje_server));
			mensajesLocalized->pokemon = "Bulbasaur";
			mensajesLocalized->cantidad_pos = 3;
			mensajesLocalized_sub1 = malloc(2 * mensajesLocalized->cantidad_pos * sizeof(int));
			mensajesLocalized->posiciones = mensajesLocalized_sub1;
			mensajesLocalized->posiciones[0] = 2; mensajesLocalized->posiciones[1] = 3;
			mensajesLocalized->posiciones[2] = 2; mensajesLocalized->posiciones[3] = 2;
			mensajesLocalized->posiciones[4] = 2; mensajesLocalized->posiciones[5] = 1;
			*destino = mensajesLocalized;
			//*id = LOCALIZED;
			*id = 1;
			break;
		case 2:
			aux_id = 20;
			mensajesAppeared = malloc(sizeof(mensaje_server));
			mensajesAppeared->pokemon = "Charmander";
			mensajesAppeared->cantidad_pos = 1;
			mensajesAppeared_sub1 = malloc(2 * mensajesAppeared->cantidad_pos * sizeof(int));
			mensajesAppeared->posiciones = mensajesAppeared_sub1;
			mensajesAppeared->posiciones[0] = 2; mensajesAppeared->posiciones[1] = 3;
			mensajesAppeared->posiciones[2] = 3; mensajesAppeared->posiciones[3] = 4;
			*destino = mensajesAppeared;
			//*id = APPEARED;
			*id = 0;
			break;
		case 3:
			aux_id = 20;
			mensajesAppeared = malloc(sizeof(mensaje_server));
			mensajesAppeared->pokemon = "Inferneit";
			mensajesAppeared->cantidad_pos = 1;
			mensajesAppeared_sub1 = malloc(2 * mensajesAppeared->cantidad_pos * sizeof(int));
			mensajesAppeared->posiciones = mensajesAppeared_sub1;
			mensajesAppeared->posiciones[0] = 1; mensajesAppeared->posiciones[1] = 0;
			*destino = mensajesAppeared;
			//*id = APPEARED;
			*id = 0;
			break;
		case 4:
			aux_id = 20;
			mensajesAppeared = malloc(sizeof(mensaje_server));
			mensajesAppeared->pokemon = "Squirtle";
			mensajesAppeared->cantidad_pos = 1;
			mensajesAppeared_sub1 = malloc(2 * mensajesAppeared->cantidad_pos * sizeof(int));
			mensajesAppeared->posiciones = mensajesAppeared_sub1;
			mensajesAppeared->posiciones[0] = 0; mensajesAppeared->posiciones[1] = 1;
			*destino = mensajesAppeared;
			//*id = APPEARED;
			*id = 0;
			break;
		default:printf("Error");

	}
	*idReal = aux_id;
}

int filtrar_mensaje(int idReal, int myId, int id, void* contenido, char**objetivo_actual, int tamano){
	int respuesta = 1;
	if(idReal != myId){respuesta = 0;}
	else{
		mensaje_server* mensaje = contenido;
		int i = 0;
		//funcion se_encuentra_en
		while(i<tamano && (strcmp(mensaje->pokemon, objetivo_actual[i]) != 0)){i++;}
		if(i==tamano){respuesta = 0;}
	}
	return respuesta;
}

void liberarServidor(int index){
	switch(index){
		case 0: case 1:
			free(mensajesLocalized);
			free(mensajesLocalized_sub1);
			break;
		case 2: case 3: case 4:
			free(mensajesAppeared);
			free(mensajesAppeared_sub1);
			break;
	}
}



