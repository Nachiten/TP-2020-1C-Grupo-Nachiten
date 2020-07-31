/*
 * config.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include "Team.h"

int extraer_valores_config(t_config*, int*, int*, int*, int*, int*);
int inicializar_entrenadores_con_config(t_config*, d_entrenador**, char***, int*, int*);
int calcular_elementos_null(char**);
int validar_tamano_vectores_extraidos(char**, char**, char**);
int niguno_es_cero_y_son_todos_iguales(int, int, int);
int convertir_a_int(char*);
int convertir_algoritmo(char*);
int validar_datos(int, int, int, int);
int llenar_objetivos_y_actuales_de_entrenador(d_entrenador*, char*, char*);
int calcular_tamano_objetivo_global(d_entrenador*, int);
int pokemon_se_repitio_antes_en_objetivos_entrenadores(char*, int, d_entrenador*);
int diferencia_cantidad_de_en_objetivos_y_pokemones_entrenadores(char*, d_entrenador*, int);
int cantidad_de_en_objetivos_entrenadores(char*, d_entrenador*, int);
int cantidad_de_en_pokemones_entrenadores(char*, d_entrenador*, int);
void llenar_objetivo_global(d_entrenador* entrenadores, int cant_entrenadores, char** vector_objetivo_global, int cant_objetivos);
void liberarConfig();



#endif /* SRC_CONFIG_H_ */
