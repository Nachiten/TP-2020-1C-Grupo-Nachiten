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

typedef struct{
	int ubicacion;
	char** no_necesito;
	char** necesito;
} deadlock_entrenador;

//enum{APPEARED, LOCALIZED, CAUGHT};
enum{ESPERA_CAUGHT, EN_ESPERA, ACTIVO, EN_DEADLOCK};

t_log* iniciar_logger(void);
t_config* leer_config(void);
//void terminar_programa(int, t_log*, t_config*);
void terminar_programa(t_log*, t_config*);
int calcular_elementos(char**);
void copiar_contenido(char**, char**, int);
int calcular_mas_cerca_de(int, int, d_entrenador**, int);
int distancia_a(int, int, int, int);
int valor_absoluto(int);
void asignar_target(d_entrenador*, char*);
void cambiar_estado_a(d_entrenador*, int);
int soy_primero_en_ready();
void moverse_a(d_entrenador*, int, int);
void bloquear(d_entrenador*, int);
int recibir_respuesta_caught();
void agregar_captura(d_entrenador*);
int esta_en_el_limite(d_entrenador*);
int cant_objetivos(d_entrenador*);
int cant_pokemones_actuales(d_entrenador*, int);
int esta_terminado(d_entrenador*);
int se_encuentra_en_objetivo(char*, char**);
void se_cumplio_objetivo(int*, d_entrenador**, int);
int se_encuentra_en_char(char*, char**, int);
void eliminar_de_objetivo_actual(char*, char***, int);
int cant_en_espera(d_entrenador*, int);
int calcular_objetivos_reales(d_entrenador*, int);
void actualizar_actuales(char**, char**, int, d_entrenador*, int);
int calcular_objetivo_individual(d_entrenador*);


#endif /* SRC_TEAM_H_ */
