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

// Procesar mensajes
void mensajeNew(char*, int, int, int, int);
void mensajeCatch(char*, int, int, int);
void mensajeGet(char*, int);

// Enviar rtas mensajes
void enviarMensajeAppeared(char*, int, int, int);
void enviarMensajeCaught(char*, int, int);
void enviarMensajeLocalized(char*, Localized*, int);

Localized* generarStructLocalized(char*, t_list*, int);




#endif /* SRC_MENSAJES_H_ */
