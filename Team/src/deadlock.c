/*
 * deadlock.c
 *
 *  Created on: 9 may. 2020
 *      Author: Andres
 */

#include "deadlock.h"

void sacar_en_espera(deadlock_entrenador** temporal, d_entrenador* entrenadores, int cantidad){
	int i, j, innecesarios, necesarios;
	j = 0;
	for(i=0;i<cantidad;i++){
		if(entrenadores[i].estado == 3 && (entrenadores[i].estado_block == EN_ESPERA || entrenadores[i].estado_block == ACTIVO)){
			(*temporal)[j].ubicacion = i;
			calcular_innecesarios_y_necesarios(entrenadores[i], &innecesarios, &necesarios);
			printf("entrenador %i: necesarios %i, innecesarios %i\n", i, necesarios, innecesarios);
			j++;
		}
	}
}

calcular_innecesarios_y_necesarios(d_entrenador entrenador, int* innecesarios, int* necesarios){
	int objetivos, actuales, i, cont;
	objetivos = cant_objetivos(&entrenador);
	actuales = cant_pokemones_actuales(&entrenador, objetivos);
	cont = 0;
	for(i=0;i<actuales;i++){
		if(se_encuentra_en_char(entrenador.pokemones_actuales[i], entrenador.objetivo, objetivos) == 0){
			cont++;
		}
	}
	*innecesarios = cont;
	cont=0;
	for(i=0;i<objetivos;i++){
		if(se_encuentra_en_char(entrenador.objetivo[i], entrenador.pokemones_actuales, actuales) == 0){
			cont++;
		}
	}
	*necesarios = cont;
}

