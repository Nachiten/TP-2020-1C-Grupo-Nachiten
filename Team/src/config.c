/*
 * config.c
 *
 *  Created on: 2 may. 2020
 *      Author: Andres
 */

#include "config.h"

char** temp1;
d_entrenador* temp2;

/* Lee los datos de la config en los parametros pasados por referencia:
PARAMS:
@config : Archivo de config
@entrenadores: lista de entrenadores
@objetivo_global: Todos los pokemones objetivo
@cant_entrenadores: Cantidad de entrenadores existentes
@objetivos: Cantidad de objetivos
*/
void inicializar_config(t_config* config, d_entrenador** entrenadores, char*** objetivo_global, int* cant_entrenadores, int* objetivos, char* vacio){
	int i, j, cont, aux_cont, cant_objetivos, cant_posiciones;
	cant_objetivos = 0;

	// Implicitamente se esta haciendo 'malloc' de estas variables. Debe hacerse free al final luego de usarlas.
	char** posicion_entrenador = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	char** objetivo = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	char** pokemones_actuales = config_get_array_value(config, "POKEMON_ENTRENADORES");

	if((*posicion_entrenador) == NULL){printf("La posicion de los entrenadores es NULL\n");}

	cant_posiciones = calcular_elementos_null(posicion_entrenador);
	printf("La cantidad de entrenadores es: %i\n", cant_posiciones);

	temp2 = malloc(cant_posiciones * sizeof(d_entrenador));

	for(i=0;i<cant_posiciones;i++){
		temp2[i].estado = 0;
		temp2[i].estado_block = 1;
		temp2[i].target = NULL;

		temp2[i].posicion[0] = posicion_entrenador[i][0]-'0';
		temp2[i].posicion[1] = posicion_entrenador[i][2]-'0';

		llenar_objetivos_y_actuales_de_entrenador(&(temp2[i]), objetivo[i], pokemones_actuales[i], vacio);

	}

	cant_objetivos = calcular_tamano_objetivo_global(temp2, cant_posiciones, vacio);
	printf("La cantidad de objetivos es: %i\n", cant_objetivos);
	temp1 = malloc((cant_objetivos+1) * sizeof(char*));
	llenar_objetivo_global(temp2, cant_posiciones, &temp1, cant_objetivos, vacio);

	*entrenadores = temp2;
	*cant_entrenadores = cant_posiciones;
	*objetivo_global = temp1;
	*objetivos = cant_objetivos;

	// Liberando memoria
	free(posicion_entrenador);
	free(objetivo);
	free(pokemones_actuales);
}

int calcular_elementos_null(char** vector){
	int i = 0;
	while(vector[i] != NULL){i++;}
	return i;
}

void llenar_objetivos_y_actuales_de_entrenador(d_entrenador* entrenador, char* vector_objetivos, char* vector_pokemones, char* vacio){
	int i, cont, aux_cont;
	char** vector;

	vector = string_split(vector_objetivos, "|");
	cont = calcular_elementos_null(vector);
	entrenador->objetivo = malloc((cont+1) * sizeof(char*));
	for(i=0;i<(cont+1);i++){
		entrenador->objetivo[i] = vector[i];
	}

	entrenador->pokemones_actuales = malloc((cont+1) * sizeof(char*));
	vector = string_split(vector_pokemones, "|");
	aux_cont = calcular_elementos_null(vector);
	for(i=0;i<aux_cont;i++){
		entrenador->pokemones_actuales[i] = vector[i];
	}
	for(i=aux_cont;i<cont;i++){
		entrenador->pokemones_actuales[i] = vacio;
	}
	entrenador->pokemones_actuales[i] = NULL;
}

int calcular_tamano_objetivo_global(d_entrenador* entrenadores, int cant_entrenadores, char* vacio){
	int i, j, cont, diferencia;
	cont = 0;
	for(i=0;i<cant_entrenadores;i++){
		j=0;
		while(entrenadores[i].pokemones_actuales[j] != NULL && string_equals_ignore_case(entrenadores[i].pokemones_actuales[j], vacio) == 0){
			if(pokemon_se_repitio_antes_en(entrenadores[i].pokemones_actuales[j], entrenadores[i].pokemones_actuales, j) == 0 && pokemon_se_repitio_antes_en_pokemones_entrenadores(entrenadores[i].pokemones_actuales[j], i, entrenadores, vacio) == 0){
				diferencia = cantidad_de_en_objetivos_entrenadores(entrenadores[i].pokemones_actuales[j], entrenadores, cant_entrenadores) - cantidad_de_en_pokemones_entrenadores(entrenadores[i].pokemones_actuales[j], entrenadores, cant_entrenadores, vacio);
				if(diferencia < 0){
					printf("error pokemon %s encontrado en actuales mas veces que en objetivos de entrenador %i\n", entrenadores[i].pokemones_actuales[j], i);
					diferencia = 0;
				}
				cont = cont + diferencia;
			}
			j++;
		}
	}

	return cont;
}

int pokemon_se_repitio_antes_en_pokemones_entrenadores(char* pokemon, int pos_entrenador, d_entrenador* entrenadores, char* vacio){
	int i, corte;
	i=0;
	corte=0;

	while(i<pos_entrenador && corte == 0){
		if(esta_en_actuales_config(pokemon, entrenadores[i].pokemones_actuales, vacio) == 1){
			corte = 1;
		}
		i++;
	}

	return corte;
}

int esta_en_actuales_config(char* pokemon, char** actuales, char* vacio){
	int i, corte;
	i=0;
	corte=0;

	while(actuales[i] != NULL && string_equals_ignore_case(actuales[i], vacio) == 0 && corte == 0){
		if(string_equals_ignore_case(actuales[i], pokemon) == 1){
			corte = 1;
		}
		i++;
	}

	return corte;
}

int pokemon_se_repitio_antes_en(char* pokemon, char** actuales, int pos_pokemon){
	int i, corte;
	i=0;
	corte = 0;

	while(i<pos_pokemon && corte == 0){
		if(string_equals_ignore_case(actuales[i], pokemon) == 1){
			corte = 1;
		}
		i++;
	}

	return corte;
}

int cantidad_de_en_objetivos_entrenadores(char* pokemon, d_entrenador* entrenadores, int cant_entrenadores){
	int i, j, cont;
	cont = 0;

	for(i=0;i<cant_entrenadores;i++){
		j=0;
		while(entrenadores[i].objetivo[j] != NULL){
			if(string_equals_ignore_case(entrenadores[i].objetivo[j], pokemon) == 1){
				cont++;
			}
			j++;
		}
	}

	return cont;
}

int cantidad_de_en_pokemones_entrenadores(char* pokemon, d_entrenador* entrenadores, int cant_entrenadores, char* vacio){
	int i, j, cont;
	cont = 0;

	for(i=0;i<cant_entrenadores;i++){
		j=0;
		while(entrenadores[i].pokemones_actuales[j] != NULL && string_equals_ignore_case(entrenadores[i].pokemones_actuales[j], vacio) == 0){
			if(string_equals_ignore_case(entrenadores[i].pokemones_actuales[j], pokemon) == 1){
				cont++;
			}
			j++;
		}
	}

	return cont;
}

void llenar_objetivo_global(d_entrenador* entrenadores, int cant_entrenadores, char*** vector_objetivo_global, int cant_objetivos, char* vacio){
	int i, j, k, cont, diferencia;
	cont = 0;

	for(i=0;i<cant_entrenadores;i++){
		j=0;
		while(entrenadores[i].pokemones_actuales[j] != NULL && string_equals_ignore_case(entrenadores[i].pokemones_actuales[j], vacio) == 0){
			if(pokemon_se_repitio_antes_en(entrenadores[i].pokemones_actuales[j], entrenadores[i].pokemones_actuales, j) == 0 && pokemon_se_repitio_antes_en_pokemones_entrenadores(entrenadores[i].pokemones_actuales[j], i, entrenadores, vacio) == 0){
				diferencia = cantidad_de_en_objetivos_entrenadores(entrenadores[i].pokemones_actuales[j], entrenadores, cant_entrenadores) - cantidad_de_en_pokemones_entrenadores(entrenadores[i].pokemones_actuales[j], entrenadores, cant_entrenadores, vacio);
				if(diferencia < 0){
					printf("error pokemon %s encontrado en actuales mas veces que en objetivos de entrenador %i\n", entrenadores[i].pokemones_actuales[j], i);
					diferencia = 0;
				}
				if(diferencia > 0){
					for(k=0;k<diferencia;k++){
						if((k+cont) < cant_objetivos){
							(*vector_objetivo_global)[k+cont] = entrenadores[i].pokemones_actuales[j];
						}
						else{printf("Error en llenado de objetivo_global, mas pokemones que lo calculado anteriormente\n");}
					}
					cont = cont + diferencia;
				}
			}
			j++;
		}
	}
	(*vector_objetivo_global)[cant_objetivos] = NULL;

}

void liberarConfig(){
	free(temp1);
	free(temp2);
}



