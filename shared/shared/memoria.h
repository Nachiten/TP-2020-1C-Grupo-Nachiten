/*
 * memoria.h
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#ifndef SHARED_MEMORIA_H_
#define SHARED_MEMORIA_H_

#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include"estructuras.h"

//para tomar los valores del archivo config que dicen como manejar la memoria
void cargar_config_memoria(t_config* config, char* TAMANIO_MEM, char* TAMANIO_MIN_PART, char* ADMIN_MEMORIA, char* ALGOR_REEMPLAZO, char* ALGOR_ASIGN_PARTICION, char* FRECUEN_COMPACT);

//void agregar_mensaje_a_Cache(void* CACHE, t_mensaje borrado, t_cola cola);



#endif /* SHARED_MEMORIA_H_ */
