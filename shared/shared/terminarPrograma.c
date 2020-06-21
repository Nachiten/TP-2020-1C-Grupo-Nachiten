/*
 * terminarPrograma.c
 *
 *  Created on: 18 abr. 2020
 *      Author: utnso
 */

#include "terminarPrograma.h"

void matarPrograma(t_log* logger, t_config* config, int32_t conexion)
{
	config_destroy(config);
	puts("Puntero a config destruido.");
	cerrar_conexion(conexion);
	puts("Conexion terminada.");
	log_destroy(logger);//por que hace agua???
	puts("Puntero al logger destruido.");
}

