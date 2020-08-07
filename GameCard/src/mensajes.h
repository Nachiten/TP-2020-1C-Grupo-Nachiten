/*
 * mensajes.h
 *
 *  Created on: 21 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_MENSAJES_H_
#define SRC_MENSAJES_H_

#include "gamecard.h"

// New -> Appeared
// Catch -> Caught
// Get -> Localized

typedef struct structNew{
	char* nombrePokemon;
	int posX;
	int posY;
	int cantidad;
	int ID;
}structNew;

// Procesar mensajes
void mensajeNew(structNew*);
void mensajeCatch(char*, int, int, int);
void mensajeGet(char*, int);

// Enviar rtas mensajes
void enviarMensajeAppeared(char*, int, int, int);
void enviarMensajeCaught(char*, int, int);
void enviarMensajeLocalized(char*, Localized*, int);

Localized* generarStructLocalized(char*, t_list*, int);




#endif /* SRC_MENSAJES_H_ */
