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
	int cant_entrenadores, cant_objetivos, i, myId, objetivo, flag_finalizacion, cant_objetivos_por_config;
	objetivo = 0;
	myId = 20;
	i=0;
	flag_finalizacion = 0;
	char** objetivo_global;
	char** objetivo_actual;
	mensaje_server* mensaje_rec;

	inicializar_config(config, &entrenadores, &objetivo_global, &cant_entrenadores, &cant_objetivos_por_config);
	cant_objetivos = calcular_objetivos_reales(entrenadores, cant_entrenadores);
	printf("La cantidad de objetivos reales es: %i\n", cant_objetivos);
	objetivo_actual = malloc((cant_objetivos+1) * sizeof(char*));
	actualizar_actuales(objetivo_actual, objetivo_global, cant_objetivos_por_config, entrenadores, cant_entrenadores);//saco los que ya tengo

	inicializar_cola_mensajes_team();//cola que contiene los mensajes que llegan de otros modulos Broker/GameBoy
	// En este bucle lleno la cola con los mensajes recibidos si es que me sirven. PD: por ahora mensajes hardcodeados
	// Este bucle se va a borrar cuando se implemente la verdadera conexion con el Broker y la recepcion de mensajes
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


	mensaje_server mensaje;
	d_entrenador* elegido;//entrenador elegido para capturar el pokemon del mensaje
	int pos_elegido, temp_cant;

	while(objetivo == 1){
		if(primero_de_cola(&mensaje) != -1){ //se extrae el primer mensaje de la colaMensajesTeam
			printf("Pokemon del mensaje: %s\n", mensaje.pokemon);
			i = 0;
			flag_finalizacion = se_encuentra_en_char(mensaje.pokemon, objetivo_actual, cant_objetivos);
			//el flag_finalizacion decrementa cualquiera sea el mensaje caught
			//itera hasta al maximo de cantidad de posiciones que pueda siempre y cuando necesites mas instancias del mismo pokemon
			while(flag_finalizacion != 0 && i<mensaje.cantidad_pos){
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
						cambiar_estado_a(elegido, 2);//EXEC
						moverse_a(elegido, mensaje.posiciones[2*i], mensaje.posiciones[(2*i)+1]);
						bloquear(elegido, ESPERA_CAUGHT);
						//enviar mensaje catch
						//recibir mensaje caught
						if(recibir_respuesta_caught() == 1){
							eliminar_de_objetivo_actual(elegido->target, &objetivo_actual, cant_objetivos);
							agregar_captura(elegido);
							if(esta_en_el_limite(elegido) == 1){
								printf("esta en el limite\n");
								if(esta_terminado(elegido) == 1){printf("Termino\n");
									cambiar_estado_a(elegido, 4);//EXIT
									se_cumplio_objetivo(&objetivo, &entrenadores, cant_entrenadores);
								}
								else{printf("Pero no termino\n");
									bloquear(elegido, EN_ESPERA);
								}
							}
							else{bloquear(elegido, ACTIVO);}//ACTIVO
						}
						else{
							bloquear(elegido, ACTIVO);
						}
						flag_finalizacion -= 1;
						i++;
					}
				}
			}
			temp_cant = cant_en_espera(entrenadores, cant_entrenadores);
			printf("En espera: %i\n", temp_cant);
			if( temp_cant > 1 && temp_cant >= cant_entrenadores/2){
				deadlock_entrenador* temp_entrenadores = malloc(temp_cant * sizeof(deadlock_entrenador));
				sacar_en_espera(&temp_entrenadores, entrenadores, cant_entrenadores);
				printf("\n");
				printf("Inicio proceso de deteccion de deadlock\n");
				tratar_deadlock(temp_entrenadores, temp_cant, entrenadores);
				actualizar_estado(entrenadores, cant_entrenadores, temp_entrenadores, temp_cant, &objetivo);
				liberar_temporal_comp(temp_entrenadores, temp_cant);
				free(temp_entrenadores);
			}
		}
		else{objetivo = 0;
			printf("Terminacion forzada\n");
		}
	}




	liberarConfig();
//	eliminarColaMensajes();
	free(objetivo_actual);
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
	distancia_minimo = -1;
	for(i=0;i<cantidad;i++){
		if((*entrenadores)[i].estado == 0 || ((*entrenadores)[i].estado == 3 && (*entrenadores)[i].estado_block == ACTIVO)){
			distancia = distancia_a(pos_x, pos_y, (*entrenadores)[i].posicion[0], (*entrenadores)[i].posicion[1]);
			if(distancia < distancia_minimo || distancia_minimo == -1){
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
	if(calcular_objetivo_individual(entrenador) == 0){retorno = 1;}
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
	int actuales = cant_pokemones_actuales(entrenador, cantidad);
	//while(i<cantidad && se_encuentra_en_objetivo(entrenador->pokemones_actuales[i], entrenador->objetivo) == 1){i++;}
	while(i<cantidad && se_encuentra_en_char(entrenador->objetivo[i], entrenador->objetivo, cantidad) == se_encuentra_en_char(entrenador->objetivo[i], entrenador->pokemones_actuales, actuales)){i++;}
	if(i == cantidad){resultado = 1;}
	return resultado;
}

//alternativa se_encuentra_en_char implementada, mantener un tiempo por si llega a ser util pero despues eliminar esta funcion
int se_encuentra_en_objetivo(char* pokemon, char** objetivo){
	int i = 0;
	int resultado = 0;
	while(objetivo[i] != NULL && strcmp(objetivo[i], pokemon) != 0){i++;}
	if(objetivo[i] != NULL){
		resultado = 1;
	}
	return resultado;
}

void se_cumplio_objetivo(int* objetivo, d_entrenador** entrenadores, int tamano){
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

int se_encuentra_en_char(char* pokemon, char** vector, int tamano){
	int i = 0;
	int resultado = 0;
	while(i<tamano){
		if(vector[i] != NULL && strcmp(vector[i], pokemon) == 0){
				resultado++;
		}
		i++;
	}
	return resultado;
}

void eliminar_de_objetivo_actual(char* pokemon, char*** objetivo, int tamano){
	int i = 0;
	int corte = 0;
	while(i<tamano && corte == 0){
		if((*objetivo)[i] != NULL && strcmp((*objetivo)[i], pokemon) == 0){
			(*objetivo)[i] = NULL;
			corte = 1;
		}
		i++;
	}
	if(corte == 0){printf("Error pokemon no se ecuentra en objetivo_actual\n");}//cambiar por el logger despues
}

int cant_en_espera(d_entrenador* entrenadores, int cantidad){
	int cont, i;
	cont = 0;
	for(i=0;i<cantidad;i++){
		if(entrenadores[i].estado == 3 && (entrenadores[i].estado_block == EN_ESPERA || entrenadores[i].estado_block == ACTIVO)){
			cont++;
		}
	}
	return cont;
}

int calcular_objetivos_reales(d_entrenador* entrenadores, int cantidad){
	int i, cont;
	cont = 0;
	for(i=0;i<cantidad;i++){
		cont = cont + calcular_objetivo_individual(&entrenadores[i]);
	}
	return cont;
}

int calcular_objetivo_individual(d_entrenador* entrenador){
	int respuesta;
	int objetivo = cant_objetivos(entrenador);
	int actuales = cant_pokemones_actuales(entrenador, objetivo);
	respuesta = objetivo - actuales;
	return respuesta;
}

void actualizar_actuales(char** actual, char** global, int cantidad_objetivos, d_entrenador* entrenadores, int cantidad){
	int i, j, k, p, temp1, temp2, objetivos, diferencia;
	k = 0;
	for(i=0;i<cantidad_objetivos;i++){
		temp1 = 0;
		for(j=0;j<cantidad;j++){
			objetivos = cant_objetivos(&entrenadores[j]);
			temp1 = temp1 + se_encuentra_en_char(global[i], entrenadores[j].pokemones_actuales, objetivos);
		}
		if(temp1 == 0){
			actual[k] = global[i];
			k++;
		}
		else{
			p = 0;
			while(p != k && strcmp(actual[p], global[i]) != 0){
				p++;
			}
			if(p == k){
				temp2 = se_encuentra_en_char(global[i], global, cantidad_objetivos);
				if(temp2 > temp1){
					diferencia = temp2 - temp1;
					while(diferencia != 0){
						actual[k] = global[i];
						k++;
						diferencia-=1;
					}
				}
				else{if(temp2 < temp1){printf("Inconsistencia en config sobre los pokemones");}}
			}
		}
	}
	actual[k] = NULL;
	//printf("k es %i\n", k);
}
