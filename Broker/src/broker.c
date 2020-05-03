#include "Broker.h"


typedef struct Entrenador{

	// Temporal | Num Pokedex
	int* listaPokemones;

	struct Entrenador* siguiente;
	struct Entrenador* anterior;

}Entrenador;


int main(void) {

	t_log* logger;
	t_config* config;
	int socket;
	char* IP_BROKER;
	char* PUERTO_BROKER;

	//Dejo cargado un logger para loguear los eventos.
	logger = cargarUnLog("Logs/broker.log", "broker");

	//Cargo las configuraciones del .config
	config = leerConfiguracion("../Configs/Broker.config");

	IP_BROKER = config_get_string_value(config,"IP_BROKER");
	PUERTO_BROKER = config_get_string_value(config,"PUERTO_BROKER");

	//Arranco el Broker como servidor.
	iniciar_server(IP_BROKER, PUERTO_BROKER);



	/*
	//Loggear "soy un log"
	log_info(logger, "Soy un log");

	int i;

	for (i = 0; i < 10;i++){
		printf("Parte %i\n", i);

		// Pausa el programa (en segundos)
		sleep(1);

	}
	*/

	return EXIT_SUCCESS;
}

/*
t_log* iniciar_logger(void)
{
	t_log *logger = log_create("Logs/broker.log", "Broker", 1, LOG_LEVEL_INFO);

	if(logger == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}

	return logger;
}
*/


