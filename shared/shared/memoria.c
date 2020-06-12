/*
 * memoria.c
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#include "memoria.h"

void cargar_config_memoria(t_config* config, char* TAMANIO_MEM, char* TAMANIO_MIN_PART, char* ADMIN_MEMORIA, char* ALGOR_REEMPLAZO, char* ALGOR_ASIGN_PARTICION, char* FRECUEN_COMPACT)
{
	TAMANIO_MEM = config_get_string_value(config,"TAMANO_MEMORIA");
	TAMANIO_MIN_PART = config_get_string_value(config,"TAMANO_MINIMO_PARTICION");
	ADMIN_MEMORIA = config_get_string_value(config,"ALGORITMO_MEMORIA");
	ALGOR_REEMPLAZO = config_get_string_value(config,"ALGORITMO_REEMPLAZO");
	ALGOR_ASIGN_PARTICION = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
	FRECUEN_COMPACT = config_get_string_value(config,"FRECUENCIA_COMPACTACION");
}
