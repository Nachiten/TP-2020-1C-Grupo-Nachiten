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
		log_info(logger, "Se leyo la ip del broker correctamente");
	}
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	if (puerto == NULL){
		log_error(logger, "No se ha podido leer el 'puerto' de la config");
		return 3;
	} else {
		log_info(logger, "Se leyo el puerto del broker correctamente");
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
	objetivo_actual = malloc(cant_objetivos * sizeof(char*));
	copiar_contenido(objetivo_actual, objetivo_global, cant_objetivos);
	inicializar_cola_mensajes_team();//cola que contiene los mensajes que llegan de otros modulos Broker/GameBoy

	//en este bucle lleno la cola con los mensajes recibidos si es que me sirven. PD: por ahora mensajes harcodeados
	while(objetivo == 0){
		if(i<5){
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
	//este bucle se va a borrar cuando se implemente la verdadera conexion con el Broker y la recepcion de mensajes

	mensaje_server mensaje;
	d_entrenador* elegido;//entrenador elegido para capturar el pokemon del mensaje
	int pos_elegido;

	while(objetivo == 1){
		if(primero_de_cola(&mensaje) != -1){ //se extrae el primer mensaje de la colaMensajesTeam
			printf("Pokemon del mensaje: %s\n", mensaje.pokemon);
			i = 0;
			flag_finalizacion = 0;
			//el flag_finalizacion cambia a 1 cuando el mensaje caught otorga un resultado positivo a la captura del pokemon, en caso contrario
			//itera hasta que se terminen las posiciones que informa el mensaje sobre dicho pokemon
			//por ahora esta asi, deberia iterar hasta al maximo que pueda siempre y cuando necesites mas instancias del mismo pokemon
			while(flag_finalizacion == 0 && i<mensaje.cantidad_pos){
				//el vector posiciones contiene todas los datos de las posiciones en un unico vector
				//osea la pos 0 y 1 del vector corresponden a la variable X e Y de la primera posicion informada
				pos_elegido = calcular_mas_cerca_de(mensaje.posiciones[2*i], mensaje.posiciones[(2*i)+1], &entrenadores, cant_entrenadores);
				if(pos_elegido != -1){	//en caso de que se encuentre algun entrenador disponible
					printf("Entrenador disponible en posicion %i del vector entrenadores\n", pos_elegido);
					elegido = &entrenadores[pos_elegido];
					asignar_target(elegido, mensaje.pokemon);
					cambiar_estado_a(elegido, 1);//ready
					//agregar_a_Ready(elegido);
					//activar hilo entrenador;
					if(soy_primero_en_ready() == 1){
						cambiar_estado_a(elegido, 2);//exec
						moverse_a(elegido, mensaje.posiciones[2*i], mensaje.posiciones[(2*i)+1]);
						bloquear(elegido, EN_ESPERA);//se bloquea a la espera de una respuesta o un deadlock en el futuro
						//enviar mensaje catch
						//recibir mensaje caught
						if(recibir_respuesta_caught() == 1){
							agregar_captura(elegido);
							if(esta_en_el_limite(elegido) == 1){
								if(esta_terminado(elegido) == 1){
									cambiar_estado_a(elegido, 4);//EXIT
									se_cumplio_objetivo(&objetivo, objetivo_global, &entrenadores, cant_entrenadores);
								}
							}
							else{bloquear(elegido, 1);}//ACTIVO
						}
						else{bloquear(elegido, 1);}
						flag_finalizacion = 1;
					}
					i++;
				}
			}
		}
		else{objetivo = 0;
			printf("Terminacion forzada\n");
		}
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
	t_log *logger = log_create("Team.log", "Team", 0, LOG_LEVEL_INFO);
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
	if( distancia < 0 ) distancia *= -1;
	return distancia;
}

void asignar_target(d_entrenador* entrenador, char* pokemon){
	entrenador->target = pokemon;
}

void cambiar_estado_a(d_entrenador* entrenador, int nuevo_estado){
	entrenador->estado = nuevo_estado;
}

int soy_primero_en_ready(){return 1;}

void moverse_a(d_entrenador* entrenador, int pos_x, int pos_y){
	//Me muevo en x
	while(entrenador->posicion[0] != pos_x){
		if((entrenador->posicion[0] - pos_x) > 0){
			entrenador->posicion[0]-=1; //si estoy a la derecha (en x), decremento
		}
		else{entrenador->posicion[0]+=1;}//si estoy a la izquierda (en x), aumento
		//delay X segundos por configuracion
	}
	//Me muevo en y
	while(entrenador->posicion[1] != pos_y){
		if((entrenador->posicion[1] - pos_y) > 0){
			entrenador->posicion[1]-=1;//si estoy arriba (en y), decremento
		}
		else{entrenador->posicion[1]+=1;}//si estoy abajo (en y), aumento
		//delay X segundos por configuracion
	}

}

void bloquear(d_entrenador* entrenador, int estado_bloqueado){
	if(entrenador->estado != 3){ //distinto de BLOCKED
		cambiar_estado_a(entrenador, 3);
	}
	entrenador->estado_block = estado_bloqueado;
}

int recibir_respuesta_caught(){return 1;}

void agregar_captura(d_entrenador* entrenador){
	int i = 0;
	while(entrenador->pokemones_actuales[i] != NULL){i++;}//itero hasta encontrar un espacio vacio, se supone que cualquie entrenador en esta funcion NO debe estar completo
	entrenador->pokemones_actuales[i] = entrenador->target;
	entrenador->target = NULL;
}

int esta_en_el_limite(d_entrenador* entrenador){
	int retorno = 0;
	int cantidad_objetivos = cant_objetivos(entrenador);
	if(cantidad_objetivos == cant_pokemones_actuales(entrenador, cantidad_objetivos)){
		retorno = 1;
	}
	return retorno;
}

int cant_objetivos(d_entrenador* entrenador){
	int i = 0;
	while(entrenador->objetivo[i] != NULL){i++;}
	return i;
}

int cant_pokemones_actuales(d_entrenador* entrenador, int cantidad_objetivos){
	int i = 0;
	while(i<cantidad_objetivos && entrenador->pokemones_actuales[i] != NULL){i++;}
	return i;
}

int esta_terminado(d_entrenador* entrenador){
	int i = 0;
	int resultado = 0;
	int cantidad = cant_objetivos(entrenador);
	while(i<cantidad && se_encuentra_en_objetivo(entrenador->pokemones_actuales[i], entrenador->objetivo) == 1){i++;}
	if(i == cantidad){resultado = 1;}
	return resultado;
}

//podria probarse una alternativa que devuelva la cantidad de veces que se repite, podria ser util para la utilizacion de flag_finalizacion
int se_encuentra_en_objetivo(char* pokemon, char** objetivo){
	int i = 0;
	int resultado = 0;
	while(objetivo[i] != NULL && strcmp(objetivo[i], pokemon) != 0){i++;}
	if(strcmp(objetivo[i], pokemon) == 0){
		resultado = 1;
	}
	return resultado;
}

void se_cumplio_objetivo(int* objetivo, char** objetivo_global, d_entrenador** entrenadores, int tamano){
	int i = 0;
	//int respuesta = -1;
	while(i<tamano && (*entrenadores)[i].estado == 4){i++;}//4 = exit
	if(i == tamano){
		*objetivo = 0;//se cambia el objetivo
		//respuesta = 1;
		printf("Terminacion correcta\n");
	}
	//return respuesta;
}



















