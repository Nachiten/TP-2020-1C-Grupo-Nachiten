/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"
	log_info(logger, "Soy un log\n");

	config = leer_config();

	ip = config_get_string_value(config, "IP");

	if(ip == NULL){
		log_error(logger , "NO SE HA PODIDO LEER LA IP\n");
	}

	puerto = config_get_string_value(config, "PUERTO");

	if(puerto == NULL){
		log_error(logger , "NO SE HA PODIDO LEER EL PUERTO\n");
	}

	log_info(logger, "Lei la IP %s y puerto %s\n", ip, puerto);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion
	conexion = crear_conexion(ip, puerto);

	//enviar mensaje
	enviar_mensaje("Hola mundo soy un mensaje tipo string :)", conexion);

	//recibir mensaje
	char* mensajeRecibido = recibir_mensaje(conexion);

	//loguear mensaje recibido
	log_info(logger, "El mensaje recibido: %s\n", mensajeRecibido);

	terminar_programa(conexion, logger, config);

}


t_log* iniciar_logger(void)
{
	t_log *logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO);

	if(logger == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}

	return logger;
}


t_config* leer_config(void)
{
	t_config *config = config_create("./tp0.config");

	if (config == NULL){
		printf("No pude leer la config \n");
		exit(2);
	}

	return config;
}


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if (logger != NULL){
	log_destroy(logger);
	}

	if (config != NULL){
	config_destroy(config);
	}

	liberar_conexion(conexion);

	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
