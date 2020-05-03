/*
 * config.c
 *
 *  Created on: 2 may. 2020
 *      Author: Andres
 */

#include "config.h"

char** temp1;
d_entrenador* temp2;

void inicializar_config(t_config* config, d_entrenador** entrenadores, char*** objetivo_global, int* cant_entrenadores, int* objetivos){
	int i, j, cont, cant_objetivos;
	cant_objetivos = 0;
	char** vector;
	char** posicion_entrenador = config_get_array_value(config, "POSICION_ENTRENADORES");
	char** objetivo = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	char** pokemones_actuales = config_get_array_value(config, "POKEMON_ENTRENADORES");
	if((*posicion_entrenador) == NULL){printf("Soy un NULL\n");}
	int cant_posiciones = calcular_elementos(posicion_entrenador);
	printf("La cantidad de entrenadores es: %i\n", cant_posiciones);
	temp2 = malloc(cant_posiciones * sizeof(d_entrenador));
	for(i=0;i<cant_posiciones;i++){
		temp2[i].estado = 1;

		temp2[i].posicion[0] = posicion_entrenador[i][0]-'0';
		temp2[i].posicion[1] = posicion_entrenador[i][2]-'0';

		vector = string_split(pokemones_actuales[i], "|");
		cont = 0;
		while(vector[cont] != NULL){cont++;}
		temp2[i].pokemones_actuales = malloc(cont * sizeof(char*));
		for(j=0;j<cont;j++){
			temp2[i].pokemones_actuales[j] = vector[j];
		}

		vector = string_split(objetivo[i], "|");
		cont = 0;
		while(vector[cont] != NULL){cont++;}
		cant_objetivos+=cont;
		temp2[i].objetivo = malloc((cont+1) * sizeof(char*));
		for(j=0;j<(cont+1);j++){
			temp2[i].objetivo[j] = vector[j];
		}
	}
	printf("La cantidad de objetivos es: %i\n", cant_objetivos);
	temp1 = malloc((cant_objetivos+1) * sizeof(char*));
	cont = 0;
	for(i=0;i<cant_posiciones;i++){
		j=0;
		//Sacar el 10 despues
		while(temp2[i].objetivo[j] != NULL && j<10){
			temp1[cont] = temp2[i].objetivo[j];
			j++;
			cont++;
		}
	}
	temp1[cont] = NULL;

	*entrenadores = temp2;
	*cant_entrenadores = cant_posiciones;
	//Ver si es realmente necesario conocer la cantidad de entrenadores
	//si se usa pocas veces, hacer una funcion en vez de devolver el valor
	*objetivo_global = temp1;
	*objetivos = cant_objetivos;

}

int calcular_elementos(char** puntero){
	int i = 0;
	char** auxiliar = puntero;
	//sacar el 50 despues
	while(auxiliar[i] != NULL && i<50){i++;}
	return i;
}

void liberarConfig(){
	free(temp1);
	free(temp2);
}



