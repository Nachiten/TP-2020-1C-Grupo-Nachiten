#include "gamecard.h"

int main(void) {

	t_log* logger;
	t_config* config;
	char* ip;
	char* puerto;
	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/GameCard.config");

	logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/GameCard/Logs/GameCard.log", "GAMECARD");

	ip = config_get_string_value(config, "IP_BROKER");
	if (ip == NULL){
		log_error(logger, "No se ha podido leer la 'ip' de la config");
		return 3;
	} else {
		log_info(logger, "Se leyo la ip del broker correctamente");
	}
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	if (puerto == NULL){
		log_error(logger, "No se ha podido leer el 'puerto' de la config");
		return 3;
	} else {
		log_info(logger, "Se leyo el puerto del broker correctamente");
	}


	return EXIT_SUCCESS;
}
