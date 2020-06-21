/*
 * deadlock.h
 *
 *  Created on: 9 may. 2020
 *      Author: utnso
 */

#ifndef SRC_DEADLOCK_H_
#define SRC_DEADLOCK_H_

#include "Team.h"

void sacar_en_espera(deadlock_entrenador*, d_entrenador*, int);
int calcular_necesarios(d_entrenador);
void llenar_estructura_deadlock(d_entrenador, deadlock_entrenador*, int);
void borrar_de_entrenador_deadlock(char*, deadlock_entrenador*, deadlock_entrenador*);
void borrar_de(char*, char**);
void reset_en_deadlock(deadlock_entrenador*, int);
void liberar_temporal_comp(deadlock_entrenador*, int);
void inicializar_respuesta(elemento_respuesta*, int);
void mostrar_respuesta(elemento_respuesta*, int);
void actualizar_respuesta(elemento_respuesta*, int);
int esta_en_respuesta(int, elemento_respuesta*, int);
void copiar_respuesta_en(elemento_respuesta*, elemento_respuesta*, int);
void llenar_mensaje_deadlock(parametros_deadlock*, d_entrenador*, deadlock_entrenador*);
void agregar_respuesta_tamano(parametros_deadlock*, elemento_respuesta*, int);
int detectar_deadlock(deadlock_entrenador*, int, elemento_respuesta*);
int alguno_de_se_encuentra_en(char**, char**, char**);
int deadlock_esta_en_null(char*, char**);
int deadlock_es_pokemon(char*, char*);
void desde_pivote(deadlock_entrenador*, int, int, elemento_respuesta*, int*);
void intercambiar(d_entrenador*, char*, d_entrenador*, char*);
void actualizar_estado_deadlock(d_entrenador*, deadlock_entrenador*, int);
void deteccion_y_recuperacion(d_entrenador*, int, int, pthread_t*);


#endif /* SRC_DEADLOCK_H_ */
