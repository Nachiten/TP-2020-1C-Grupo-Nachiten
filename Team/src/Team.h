/*
 * Team.h
 *
 *  Created on: 30 abr. 2020
 *      Author: Andres
 */

#ifndef SRC_TEAM_H_
#define SRC_TEAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <shared/socket.h>
#include <commons/collections/queue.h>
/*
typedef enum{
	NEW,
	READY,
	EXEC,
	BLOCKED,
	EXIT;
}d_estado;
*/
//enum{NEW, READY, EXEC, BLOCKED, EXIT};

typedef struct{
	int posicion[2];
	int estado;
	char** objetivo;
	char** pokemones_actuales;
} d_entrenador;

typedef struct{
	char* pokemon;
	int cantidad_pos;
	int* posiciones;
} mensaje_server;

//enum{APPEARED, LOCALIZED, CAUGHT};
enum{EN_ESPERA, ACTIVO, EN_DEADLOCK};

t_log* iniciar_logger(void);
t_config* leer_config(void);
//void terminar_programa(int, t_log*, t_config*);
void terminar_programa(t_log*, t_config*);
int calcular_elementos(char**);
void copiar_contenido(char**, char**, int);
int calcular_mas_cerca_de(int, int, d_entrenador**, int);
int distancia_a(int, int, int, int);
int valor_absoluto(int);

#endif /* SRC_TEAM_H_ */
