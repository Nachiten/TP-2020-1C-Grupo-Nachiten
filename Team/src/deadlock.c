/*
 * deadlock.c
 *
 *  Created on: 9 may. 2020
 *      Author: Andres
 */

#include "deadlock.h"

char* utilizado = "Vacio";

void sacar_en_espera(deadlock_entrenador** temporal, d_entrenador* entrenadores, int cantidad){
	int i, j, innecesarios, necesarios;
	j = 0;
	for(i=0;i<cantidad;i++){
		if(entrenadores[i].estado == 3 && (entrenadores[i].estado_block == EN_ESPERA || entrenadores[i].estado_block == ACTIVO)){
			(*temporal)[j].ubicacion = i;
			calcular_innecesarios_y_necesarios(entrenadores[i], &innecesarios, &necesarios);
			printf("entrenador %i: necesarios %i, innecesarios %i\n", i, necesarios, innecesarios);
			(*temporal)[j].necesito = malloc((necesarios+1) * sizeof(char*));
			(*temporal)[j].no_necesito = malloc((innecesarios+1) * sizeof(char*));
			llenar_estructura_deadlock(entrenadores[i], (*temporal)[j], innecesarios, necesarios);
			j++;
		}
	}
}

void calcular_innecesarios_y_necesarios(d_entrenador entrenador, int* innecesarios, int* necesarios){
    int objetivos, actuales, i, j, in_cont, nec_cont, aux;
    char* pokemon;
    objetivos = cant_objetivos(&entrenador);
    actuales = cant_pokemones_actuales(&entrenador, objetivos);
    in_cont = 0; nec_cont = 0;
    for(i=0;i<actuales;i++){
    	if(se_encuentra_en_char(entrenador.pokemones_actuales[i], entrenador.objetivo, objetivos) == 0){
            in_cont++;
    	}
        else{
            aux = 0;
            pokemon = entrenador.pokemones_actuales[i];
            while(strcmp(entrenador.pokemones_actuales[aux], pokemon) != 0){aux++;}
            if(aux == i){
                int diferencia = se_encuentra_en_char(pokemon, entrenador.pokemones_actuales, actuales) - se_encuentra_en_char(pokemon, entrenador.objetivo, objetivos);
                if(diferencia != 0){
                    if(diferencia>0){
                        in_cont = in_cont + diferencia;
                    }
                    else{
                        nec_cont = nec_cont - diferencia;
                    }
                }
            }
        }
    }
    *innecesarios = in_cont;
    for(i=0;i<objetivos;i++){
    	if(se_encuentra_en_char(entrenador.objetivo[i], entrenador.pokemones_actuales, actuales) == 0){
            nec_cont++;
    	}
    }
    *necesarios = nec_cont;
}

void llenar_estructura_deadlock(d_entrenador entrenador, deadlock_entrenador temporal, int innecesarios, int necesarios){
    int objetivos, actuales, i, j, k, in_cont, nec_cont, aux;
    char* pokemon;
    objetivos = cant_objetivos(&entrenador);
    actuales = cant_pokemones_actuales(&entrenador, objetivos);
    in_cont = 0; nec_cont = 0;
    for(i=0;i<actuales;i++){
    	if(se_encuentra_en_char(entrenador.pokemones_actuales[i], entrenador.objetivo, objetivos) == 0){
    		temporal.no_necesito[in_cont] = entrenador.pokemones_actuales[i];
            in_cont++;
    	}
        else{
            aux = 0;
            pokemon = entrenador.pokemones_actuales[i];
            while(strcmp(entrenador.pokemones_actuales[aux], pokemon) != 0){aux++;}
            if(aux == i){
                int diferencia = se_encuentra_en_char(pokemon, entrenador.pokemones_actuales, actuales) - se_encuentra_en_char(pokemon, entrenador.objetivo, objetivos);
                if(diferencia != 0){
                    if(diferencia>0){
                        for(k=in_cont;k<(in_cont+diferencia);k++){
                            temporal.no_necesito[in_cont] = pokemon;
                        }
                        in_cont = in_cont + diferencia;
                    }
                    else{
                        for(k=nec_cont;k<(nec_cont-diferencia);k++){
                            temporal.necesito[nec_cont] = pokemon;
                        }
                        nec_cont = nec_cont - diferencia;
                    }
                }
            }
        }
    }
    temporal.no_necesito[in_cont] = NULL;

    for(i=0;i<objetivos;i++){
    	if(se_encuentra_en_char(entrenador.objetivo[i], entrenador.pokemones_actuales, actuales) == 0){
            temporal.necesito[nec_cont] = entrenador.objetivo[i];
            nec_cont++;
    	}
    }
    temporal.necesito[nec_cont] = NULL;
}

void tratar_deadlock(deadlock_entrenador* temp_entrenadores, int cantidad, d_entrenador* entrenadores){
    int deteccion, i, cont;
    int* respuesta = malloc(cantidad * sizeof(int));
    cont = 1;
    for(i=0;i<cantidad;i++){respuesta[i] = -1;}

    printf("Iteracion 1\n");
    deteccion = detectar_deadlock(temp_entrenadores, cantidad, respuesta);
    while(deteccion != -1){
        int t=0;
        printf("Se detecto deadlock\n");
        printf("Respuesta: ");
        while(t<cantidad && respuesta[t] != -1){
            printf("%i ", respuesta[t]);
            t++;
        }
        printf("\n");
        recuperar_deadlock(temp_entrenadores, cantidad, respuesta, entrenadores);
        for(i=0;i<cantidad;i++){respuesta[i] = -1;}
        cont++;
        printf("Iteracion %i\n", cont);
        deteccion = detectar_deadlock(temp_entrenadores, cantidad, respuesta);
    }
    free(respuesta);
    printf("Fin de deadlock\n");
}

int detectar_deadlock(deadlock_entrenador* entrenadores, int cantidad, int* respuesta){
    int i, j, corte;
    corte = -1;
    i = 0;
    while(corte == -1 && i<cantidad){
        j = i+1;
        respuesta[0] = i;
        while(corte == -1 && j<cantidad){
            if(alguno_de_se_encuentra_en(entrenadores[i].no_necesito, entrenadores[j].necesito) == 1){
                respuesta[1] = j;
                desde_pivote(entrenadores, cantidad, 1, respuesta, &corte);
            }
            j++;
        }
        i++;
    }
    return corte;
}

int alguno_de_se_encuentra_en(char** un_vector, char** otro_vector){
    int i, respuesta;
    i = 0;
    respuesta = 0;
    while(un_vector[i] != NULL && se_encuentra_en_null(un_vector[i], otro_vector) == 0){i++;}
    if(un_vector[i] != NULL){respuesta = 1;}
    return respuesta;
}

int se_encuentra_en_null(char* pokemon, char** vector){
    int i, respuesta;
    i=0;
    respuesta = 0;
    while(vector[i] != NULL && respuesta == 0){
        if(strcmp(utilizado, vector[i]) != 0 && strcmp(pokemon, vector[i]) == 0){respuesta = 1;}
        i++;
    }
    return respuesta;
}

void desde_pivote(deadlock_entrenador* entrenadores, int cantidad, int last_pos, int* respuesta, int* corte){
    int i, j;
    i=1;
    if(alguno_de_se_encuentra_en(entrenadores[last_pos].no_necesito, entrenadores[0].necesito) == 1){
        *corte = last_pos;
    }
    else{
        if(last_pos+1 < cantidad){
            while(corte = -1 && i<cantidad){
                if(esta_en_int(i, respuesta, cantidad) == 0 && alguno_de_se_encuentra_en(entrenadores[last_pos].no_necesito, entrenadores[i].necesito) == 1){
                    last_pos++;
                    respuesta[last_pos] = i;
                    desde_pivote(entrenadores, cantidad, last_pos, respuesta, corte);
                }
                i++;
            }
        }
    }
}

int esta_en_int(int num, int* vector, int cantidad){
    int i, respuesta;
    i=0;
    respuesta=0;
    while(i<cantidad && vector[i] != num){i++;}
    if(i != cantidad){respuesta = 1;}
    return respuesta;
}

void recuperar_deadlock(deadlock_entrenador* temp_entrenadores, int cantidad, int* respuesta, d_entrenador* entrenadores){
    int i, j;
    char* un_pokemon;
    char* otro_pokemon;
    i=1;
    while(i<cantidad){
        j=0;
        while(temp_entrenadores[respuesta[i-1]].no_necesito[j] != NULL && se_encuentra_en_null(temp_entrenadores[respuesta[i-1]].no_necesito[j], temp_entrenadores[respuesta[i]].necesito) == 0){j++;}
        if(temp_entrenadores[respuesta[i-1]].no_necesito[j] != NULL){
            un_pokemon = temp_entrenadores[respuesta[i-1]].no_necesito[j];
            temp_entrenadores[respuesta[i-1]].no_necesito[j] = utilizado;
            borrar_de(un_pokemon, temp_entrenadores[respuesta[i]].necesito);
            j=0;
            while(temp_entrenadores[respuesta[i]].no_necesito[j] != NULL && se_encuentra_en_null(temp_entrenadores[respuesta[i]].no_necesito[j], temp_entrenadores[respuesta[i-1]].necesito) == 0){j++;}
            if(temp_entrenadores[respuesta[i]].no_necesito[j] != NULL){
                otro_pokemon = temp_entrenadores[respuesta[i]].no_necesito[j];
                temp_entrenadores[respuesta[i]].no_necesito[j] = utilizado;
                borrar_de(otro_pokemon, temp_entrenadores[respuesta[i-1]].necesito);
                realizar_intercambio(&entrenadores[temp_entrenadores[respuesta[i-1]].ubicacion], un_pokemon, &entrenadores[temp_entrenadores[respuesta[i]].ubicacion], otro_pokemon);
            }
            else{printf("Error en recuperacion deadlock");}
        }
        else{printf("Error en recuperacion deadlock");}
        i++;
    }
}

void borrar_de(char* pokemon, char** vector){
    int i=0;
    while(vector[i] != NULL && strcmp(vector[i], pokemon) != 0){i++;}
    if(vector[i] != NULL){
        vector[i] = utilizado;
    }
    else{printf("Error en borrado de pokemon");}
}

void realizar_intercambio(d_entrenador* un_entrenador, char* un_pokemon, d_entrenador* otro_entrenador, char* otro_pokemon){
    cambiar_estado_a(un_entrenador, 1);
    if(soy_primero_en_ready() == 1){
       cambiar_estado_a(un_entrenador, 2);
       moverse_a(un_entrenador, otro_entrenador->posicion[0], otro_entrenador->posicion[1]);
       intercambiar(un_entrenador, un_pokemon, otro_entrenador, otro_pokemon);
    }
}

void intercambiar(d_entrenador* un_entrenador, char* un_pokemon, d_entrenador* otro_entrenador, char* otro_pokemon){
    int i;
    i=0;
    while(un_entrenador->pokemones_actuales[i] != un_pokemon){i++;}
    un_entrenador->pokemones_actuales[i] = otro_pokemon;
    i=0;
    while(otro_entrenador->pokemones_actuales[i] != otro_pokemon){i++;}
    otro_entrenador->pokemones_actuales[i] = un_pokemon;
}

void actualizar_estado(d_entrenador* entrenadores, int cant_entrenadores, deadlock_entrenador* temp_entrenadores, int cantidad, int* objetivo){
    int i;
    d_entrenador* entrenador;
    for(i=0;i<cantidad;i++){
        entrenador = &entrenadores[temp_entrenadores[i].ubicacion];
        if(esta_en_el_limite(entrenador) == 1){
            printf("entrenador pos %i esta en el limite\n", temp_entrenadores[i].ubicacion);
            if(esta_terminado(entrenador) == 1){
                printf("Termino\n");
                cambiar_estado_a(entrenador, 4);//EXIT
                se_cumplio_objetivo(objetivo, &entrenadores, cant_entrenadores);
            }
            else{
                printf("Pero no termino\n");
            }
        }
    }
    printf("Fin de actualizacion de deadlock\n");
}

void liberar_temporal_comp(deadlock_entrenador* temporal, int cantidad){
	int i;
	for(i=0;i<cantidad;i++){
		free(temporal[i].necesito);
		free(temporal[i].no_necesito);
	}
}

