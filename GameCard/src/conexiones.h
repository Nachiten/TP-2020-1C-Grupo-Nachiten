/*
 * conexiones.h
 *
 *  Created on: 21 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_CONEXIONES_H_
#define SRC_CONEXIONES_H_

#include "gamecard.h"

sem_t* semConexionBroker;

pthread_t hiloNewGameboy, hiloCatchGameboy, hiloGetGameboy;
pthread_t hiloNewBroker, hiloCatchBroker, hiloGetBroker;

typedef struct datosHiloBroker{
	char* IP_BROKER;
	char* PUERTO_BROKER;
	int TIEM_REIN_CONEXION;
	t_log* logger;
}datosHiloBroker;

// Hilos de escucha
void comenzarEscuchaGameBoy();
void comenzarConexionConBroker(datosHiloBroker*);
// Conexiones
void serve_client(int32_t*);
void esperarMensajes(datosHiloColas*);
void escuchoSocket(int32_t); //necesario para recibir cosas desde ESE socket
void esperar_conexiones(int32_t);

int conectarseABroker(t_log*, codigo_operacion);
void process_request(codigo_operacion, int32_t, int32_t);
void suscribirseAUnaCola(int32_t, codigo_operacion);
int escucharGameBoy(char*, char*, t_log*);


#endif /* SRC_CONEXIONES_H_ */
