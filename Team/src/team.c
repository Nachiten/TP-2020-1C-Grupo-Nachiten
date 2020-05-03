#include "Team.h"

int main(void) {

	printf("Inicio de Team\n");

	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;
	logger = iniciar_logger();
	config = leer_config();

	//log_info(logger, "Soy un log\n");
	ip = config_get_string_value(config, "IP_BROKER");
	//printf("Puerto: %s\n", ip);
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	//printf("Puerto: %s\n", puerto);
	//conexion = crear_conexion(ip, puerto);
	conexion = 0;

	d_entrenador* entrenadores;
	int cant_entrenadores, cant_objetivos, i, myId, objetivo;
	objetivo = 0;
	myId = 20;
	i=0;
	char** objetivo_global;
	char** objetivo_actual;
	mensaje_server* mensaje_rec;

	inicializar_config(config, &entrenadores, &objetivo_global, &cant_entrenadores, &cant_objetivos);
/*
	printf("posicion entrenador0: [%i, %i]\n", entrenadores[1].posicion[0], entrenadores[1].posicion[1]);
	printf("pokemones_actuales[0] entrenador2: %s\n", entrenadores[2].pokemones_actuales[0]);
	printf("pokemones_actuales[1] entrenador1: %s\n", entrenadores[1].pokemones_actuales[1]);
	printf("pokemones_actuales[2] entrenador0: %s\n", entrenadores[0].pokemones_actuales[2]);
	printf("objetivo[0] entrenador2: %s\n", entrenadores[2].objetivo[0]);
	printf("objetivo[1] entrenador1: %s\n", entrenadores[1].objetivo[1]);
	printf("objetivo[2] entrenador0: %s\n", entrenadores[0].objetivo[2]);
*/
	t_queue* colaMensajesTeam = queue_create();
	objetivo_actual = malloc(cant_objetivos * sizeof(char*));
	copiar_contenido(objetivo_actual, objetivo_global, cant_objetivos);

	while(objetivo == 0){
		if(i<2){
			int id, idReal;
			recibir_mensajes(conexion, i, &mensaje_rec, &id, &idReal);
			if(filtrar_mensaje(idReal, myId, id, mensaje_rec, objetivo_actual, cant_objetivos) == 1){
				queue_push(colaMensajesTeam, mensaje_rec);
			}
			liberarServidor(i);
			i++;
		}
		else{objetivo = 1;}
	}

	int size = queue_is_empty(colaMensajesTeam);
	printf("size: %i\n", size);
	liberarConfig();

	printf("Fin de Team\n");

	return EXIT_SUCCESS;
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
	t_config* config = config_create("./team.config");
	if(config == NULL){
		printf("No se pudo leer config\n");
		exit(1);
	}
	return config;
}

void copiar_contenido(char** destino, char** origen, int tamano){
	int i;
	for(i=0;i<tamano;i++){
		destino[i] = origen[i];
	}
}



