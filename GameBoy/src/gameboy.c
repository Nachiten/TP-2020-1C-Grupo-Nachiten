#include "GameBoy.h"

int main(void) {

	/*
	int i = 1;
	printf("Probando GameBoy %i", i);
	*/

	t_log* logger;
	t_config* config;
	int socket;
	char* IP_BROKER;
	char* PUERTO_BROKER;
	char* mensaje_recibido;

	//Dejo cargado un logger para loguear los eventos.
	logger = cargarUnLog("Logs/Gameboy.log", "Gameboy");

	//Cargo las configuraciones del .config
	config = leerConfiguracion("../Configs/GameBoy.config");

	IP_BROKER = config_get_string_value(config,"IP_BROKER");
	PUERTO_BROKER = config_get_string_value(config,"PUERTO_BROKER");

	//creo conexión con el Broker.
	socket = establecer_conexion(IP_BROKER,PUERTO_BROKER);
	if(socket == -1)
	{
		log_warning(logger, "Conexión fallida con socket de BROKER");
	}
	else
	{
		log_info(logger, "Conexión establecida con socket de BROKER");
	}

	//Enviamos mensaje de prueba
	mandar_mensaje("mensaje de prueba de mandar mensaje\n", TEST, socket);

	uint32_t size;
	//Para recibir mensajes (coming soon)
	mensaje_recibido = recibir_mensaje(socket, size);
	//se supone que recibimos un mensaje diciendo que la conexion fue exitosa o algo asi
	log_info(logger, mensaje_recibido);


	//enviarle los recursos a liberar
	matarPrograma(logger, config, socket);

	return EXIT_SUCCESS;
}
