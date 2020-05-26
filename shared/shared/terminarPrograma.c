/*
 * terminarPrograma.c
 *
 *  Created on: 18 abr. 2020
 *      Author: utnso
 */

#include "terminarPrograma.h"

void matarPrograma(t_log* logger, t_config* config, int conexion)
{
	log_destroy(logger);
	config_destroy(config);
	cerrar_conexion(conexion);
}

