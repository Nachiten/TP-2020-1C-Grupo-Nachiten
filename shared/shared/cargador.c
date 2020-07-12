/*
 * cargador.c
 *
 *  Created on: 18 abr. 2020
 *      Author: utnso
 */

#include "cargador.h"

//Deja un log listo para usar.
t_log* cargarUnLog(char* ruta, char* nombreLog)
{
	return log_create(ruta, nombreLog, 1, LOG_LEVEL_TRACE);
}

//carga las configuraciones del archivo config que le pasamos.
t_config* leerConfiguracion(char* ruta)
{
	return config_create(ruta);
}
