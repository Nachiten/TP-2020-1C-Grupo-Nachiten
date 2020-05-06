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

	ip = config_get_string_value(config, "IP_BROKER");
	if (ip == NULL){
		log_error(logger, "No se ha podido leer la 'ip' de la config");
		return 3;
	} else {
		log_info(logger, "Se leyo la ip del broker correctamente\n");
	}
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	if (puerto == NULL){
		log_error(logger, "No se ha podido leer el 'puerto' de la config");
		return 3;
	} else {
		log_info(logger, "Se leyo el puerto del broker correctamente\n");
	}

	//conexion = crear_conexion(ip, puerto);
	conexion = 0;

	d_entrenador* entrenadores;
	int cant_entrenadores, cant_objetivos, i, myId, objetivo, flag_finalizacion;
	objetivo = 0;
	myId = 20;
	i=0;
	flag_finalizacion = 0;
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

	objetivo_actual = malloc(cant_objetivos * sizeof(char*));
	copiar_contenido(objetivo_actual, objetivo_global, cant_objetivos);
	inicializar_cola_mensajes_team();


	while(objetivo == 0){
		if(i<2){
			int id, idReal;
			recibir_mensajes(conexion, i, &mensaje_rec, &id, &idReal);
			if(filtrar_mensaje(idReal, myId, id, mensaje_rec, objetivo_actual, cant_objetivos) == 1){
				agregar_a_cola(mensaje_rec);
			}
			liberarServidor(i);
			i++;
		}
		else{objetivo = 1;}
	}

	mensaje_server mensaje;
	d_entrenador* elegido;
	int pos_elegido;
	while(objetivo == 1){
		if(primero_de_cola(&mensaje) != -1){
			printf("Pokemon del mensaje: %s\n", mensaje.pokemon);
			i = 0;
			flag_finalizacion = 0;
			while(flag_finalizacion == 0 && i<mensaje.cantidad_pos){
				pos_elegido = calcular_mas_cerca_de(mensaje.posiciones[2*i], mensaje.posiciones[(2*i)+1], &entrenadores, cant_entrenadores);
				if(pos_elegido != -1){
					printf("Entrenador disponible en posicion %i del vector entrenadores\n", pos_elegido);
					flag_finalizacion = 1;
				}
			}
		}
		objetivo = 0;
	}




	liberarConfig();
	eliminarColaMensajes();
	//free(objetivo_actual);
	terminar_programa(logger, config);

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

// Se usa funcion de las shared, no esta
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

void terminar_programa(t_log* logger, t_config* config){
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	if(config != NULL){config_destroy(config);}
	if(logger != NULL){log_destroy(logger);}
	//liberar_conexion(conexion);
}

int calcular_mas_cerca_de(int pos_x, int pos_y, d_entrenador** entrenadores, int cantidad){
	int i, distancia, distancia_minimo, pos_a_enviar;
	pos_a_enviar = -1;
	distancia_minimo = 50; //maximo imposible, despues cambiar para que sea la distancia al primer entrenador del vector
	for(i=0;i<cantidad;i++){
		if((*entrenadores)[i].estado == 0 || (*entrenadores)[i].estado == 3){
			distancia = distancia_a(pos_x, pos_y, (*entrenadores)[i].posicion[0], (*entrenadores)[i].posicion[0]);
			if(distancia < distancia_minimo){
				distancia_minimo = distancia;
				pos_a_enviar = i;
			}
		}
	}
	return pos_a_enviar;
}

int distancia_a(int pos_x, int pos_y, int actual_x, int actual_y){
	int distancia = valor_absoluto(actual_x - pos_x) + valor_absoluto(actual_y - pos_y);
	return distancia;
}

int valor_absoluto(int distancia){
	if(distancia<1){distancia = distancia * (-1);}
	return distancia;
}


