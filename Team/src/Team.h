/*
 * Team.h
 *
 *  Created on: 30 abr. 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <shared/socket.h>
#include <commons/collections/queue.h>
/*
typedef enum{
	NEW,
	READY,
*/
//enum{NEW, READY, EXEC, BLOCKED, EXIT};

typedef struct{
	int posicion[2];
	int estado;
	char* target;
	int estado_block;
	char** objetivo;
	char** pokemones_actuales;
} d_entrenador;

typedef struct{
	char* pokemon;
	int cantidad_pos;
	int* posiciones;
} mensaje_server;

//enum{APPEARED, LOCALIZED, CAUGHT};
t_config* leer_config(void);int, t_log*, t_config*);
void terminar_programa(t_log*, t_config*);
int calcular_elementos(char**);
void copiar_contenido(char**, char**, int);
int calcular_mas_cerca_de(int, int, d_entrenador**, int);
int distancia_a(int, int, int, int);
int valor_absoluto(int);
void asignar_target(d_entrenador*, char*);
void cambiar_estado_a(d_entrenador*, int);
void moverse_a(d_entrenador*, int, int);
vd bloquear(d_entrenador*, int);
i recibir_respuesta_caught();
vr en;nn