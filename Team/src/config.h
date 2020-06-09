/*
 * config.h
 *
 *  Created on: 2 may. 2020
 *      Author: utnso
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include "Team.h"
void liberarConfig();
int inicializar_entrenadores_con_config(t_config*, d_entrenador**, char***, int*, int*, char*);
int calcular_elementos_null(char**);
int validar_tamano_vectores_extraidos(char**, char**, char**);
int niguno_es_cero_y_son_todos_iguales(int, int, int);
int llenar_objetivos_y_actuales_de_entrenador(d_entrenador*, char*, char*, char*);
int calcular_tamano_objetivo_global(d_entrenador*, int, char*);
int pokemon_se_repitio_antes_en_pokemones_entrenadores(char*, int, d_entrenador*, char*);
int esta_en_actuales_config(char*, char**, char*);
int pokemon_se_repitio_antes_en_(char*, char**, int);
int cantidad_de_en_objetivos_entrenadores(char*, d_entrenador*, int);
int cantidad_de_en_pokemones_entrenadores(char*, d_entrenador*, int, char*);
void llenar_objetivo_global(d_entrenador*, int, char***, int, char*);


#endif /* SRC_CONFIG_H_ */
