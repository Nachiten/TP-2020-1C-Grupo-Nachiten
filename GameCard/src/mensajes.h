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

typedef struct structGet{
	char* nombrePokemon;
	int ID;
}structGet;

typedef struct structCatch{
	char* nombrePokemon;
	int posX;
	int posY;
	int ID;
}structCatch;


// Procesar mensajes
void mensajeNew(structNew*);
void mensajeCatch(structCatch*);
void mensajeGet(structGet*);

// Enviar rtas mensajes
void enviarMensajeAppeared(char*, int, int, int);
void enviarMensajeCaught(char*, int, int);
void enviarMensajeLocalized(char*, Localized*, int);

Localized* generarStructLocalized(char*, t_list*, int);




#endif /* SRC_MENSAJES_H_ */
