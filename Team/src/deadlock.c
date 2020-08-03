/*
 * deadlock.c
 *
 *  Created on: 9 may. 2020
 *      Author: Andres
 */

#include "deadlock.h"

char* VACIO = "celda_vacia";

///////////////////-VECTOR DEADLOCK ENTRENADORES-/////////////////////
void sacar_en_espera(deadlock_entrenador* temporal, d_entrenador* entrenadores, int cant_entrenadores){
    int i, j, necesarios;
    j = 0;
    for(i=0;i<cant_entrenadores;i++){
	if(cumple_condicion_deadlock(entrenadores[i]) == 1){
            temporal[j].ubicacion = i;
            temporal[j].en_deadlock = 0;
            necesarios = calcular_necesarios(entrenadores[i]);
            printf("entrenador %i: necesarios y innecesarios %i\n", i, necesarios);
            temporal[j].necesito = malloc((necesarios+1) * sizeof(char*));
            temporal[j].no_necesito = malloc((necesarios+1) * sizeof(char*));
            llenar_estructura_deadlock(entrenadores[i], &(temporal[j]), necesarios);
            j++;
	}
    }
}

int calcular_necesarios(d_entrenador entrenador){
    int objetivos, i, cont, diferencia;
    cont = 0;
    //en este punto la cantidad de objetivos es la misma que pokemones actuales
    objetivos = cant_objetivos(entrenador);
    for(i=0;i<objetivos;i++){
        if(pokemon_se_repitio_antes_en_objetivo(entrenador.objetivo[i], entrenador.objetivo, i) == 0){
            diferencia = diferencia_de_repeticiones_de_en_objetivos_y_actuales(entrenador.objetivo[i], entrenador.objetivo, entrenador.pokemones_actuales, objetivos);
            if(diferencia > 0){
                cont = cont + diferencia;
            }
        }
    }
    return cont;
}

void llenar_estructura_deadlock(d_entrenador entrenador, deadlock_entrenador* temporal, int necesarios){
    int objetivos, i, in_cont, nec_cont, diferencia;
    objetivos = cant_objetivos(entrenador);
    in_cont = 0; nec_cont = 0;
    for(i=0;i<objetivos;i++){
        if(pokemon_se_repitio_antes_en_objetivo(entrenador.objetivo[i], entrenador.objetivo, i) == 0){
            diferencia = diferencia_de_repeticiones_de_en_objetivos_y_actuales(entrenador.objetivo[i], entrenador.objetivo, entrenador.pokemones_actuales, objetivos);
            if(diferencia != 0){
                if(diferencia > 0){
                    llenar_con(nec_cont, nec_cont + diferencia, temporal->necesito, entrenador.objetivo[i]);
                    nec_cont = nec_cont + diferencia;
                }
                else{
                    if(necesarios >= in_cont - diferencia){
                        llenar_con(in_cont, in_cont - diferencia, temporal->no_necesito, entrenador.objetivo[i]);
                        in_cont = in_cont - diferencia;
                    }
                    else{printf("error, mayor cantidad de inncesarios detectados que necesarios");}
                }
            }
        }
    }
    temporal->necesito[necesarios] = NULL;
    for(i=0;i<objetivos;i++){
        if(esta_en_objetivo_individual(entrenador.pokemones_actuales[i], entrenador.objetivo) == 0){
            if(necesarios > in_cont){
                temporal->no_necesito[in_cont] = entrenador.pokemones_actuales[i];
                in_cont++;
            }
            else{printf("error, mayor cantidad de inncesarios detectados que necesarios");}
	}
    }
    temporal->no_necesito[necesarios] = NULL;
}

void borrar_de_entrenador_deadlock(char* pokemon, deadlock_entrenador* entrenador_no_necesita, deadlock_entrenador* entrenador_necesita){
    borrar_de(pokemon, entrenador_no_necesita->no_necesito);
    borrar_de(pokemon, entrenador_necesita->necesito);
}

void borrar_de(char* pokemon, char** vector){
    int i=0;
    while(vector[i] != NULL && strcmp(vector[i], pokemon) != 0){i++;}
    if(vector[i] != NULL){
        vector[i] = VACIO;
    }
    else{printf("Error en borrado de pokemon %s\n", pokemon);}
}

void reset_en_deadlock(deadlock_entrenador* entrenadores, int cant_entrenadores){
    int i = 0;
    while(i<cant_entrenadores){
        entrenadores[i].en_deadlock = 0;
        i++;
    }
}

void liberar_temporal_comp(deadlock_entrenador* temporal, int cantidad){
    int i;
    for(i=0;i<cantidad;i++){
	free(temporal[i].necesito);
	free(temporal[i].no_necesito);
    }
}

///////////////////-RESPUESTA-/////////////////////
void inicializar_respuesta(elemento_respuesta* respuesta, int tamano){
    int i;
    for(i=0;i<tamano;i++){
        respuesta[i].posicion = -1;
        respuesta[i].pokemon = NULL;
    }

}

void mostrar_respuesta(elemento_respuesta* respuesta, int tamano){
    int i=0;
    while(i<tamano){
        printf(" %i  %s\n", respuesta[i].posicion, respuesta[i].pokemon);
        i++;
    }
}

void actualizar_respuesta(elemento_respuesta* respuesta, int tamano){
    inicializar_respuesta(respuesta, tamano);
}

int esta_en_respuesta(int num, elemento_respuesta* vector, int tamano){
    int i, respuesta;
    i=0;
    respuesta=0;
    while(i<tamano && vector[i].posicion != num){i++;}
    if(i<tamano){respuesta = 1;}
    return respuesta;
}

void copiar_respuesta_en(elemento_respuesta* original, elemento_respuesta* copia, int tamano){
    int i;
    for(i=0;i<tamano;i++){
        copia[i] = original[i];
    }
}

///////////////////-MENSAJE DEADLOCK-/////////////////////
void llenar_mensaje_deadlock(parametros_deadlock* mensaje_deadlock, d_entrenador* entrenadores, deadlock_entrenador* temporal){
    mensaje_deadlock->entrenadores = entrenadores;
    mensaje_deadlock->temp_entrenadores = temporal;
}

void agregar_respuesta_tamano(parametros_deadlock* mensaje_deadlock, elemento_respuesta* respuesta, int tamano){
    mensaje_deadlock->tamano_respuesta = tamano;
    mensaje_deadlock->respuesta = respuesta;
}

///////////////////-DETECCION DEADLOCK-/////////////////////
int detectar_deadlock(deadlock_entrenador* entrenadores, int cantidad, elemento_respuesta* respuesta){
    int i, j, corte;
    corte = 0;
    i = 0;
    while(corte == 0 && i<cantidad){
        if(entrenadores[i].en_deadlock == 0)
        {
            j = i+1;
            respuesta[0].posicion = i; // El primer entrenador que va a participar en el intercambio y va a dar un pokemon
            while(corte == 0 && j<cantidad){
                if(entrenadores[j].en_deadlock == 0){
                	// Si alguno de los pokemones que no necesita el primero, lo necesita el segundo
                    if(alguno_de_se_encuentra_en(entrenadores[i].no_necesito, entrenadores[j].necesito, &(respuesta[0].pokemon)) == 1){
                        respuesta[1].posicion = j; // El segundo entrenador que va a participar y recibir el pokemon
                        desde_pivote(entrenadores, cantidad, 1, respuesta, &corte);
                    }
                }
                j++;
            }
            if(corte == 0){
                respuesta[1].posicion = -1;
                respuesta[0].pokemon = NULL;
            }
            else{entrenadores[respuesta[1].posicion].en_deadlock = 1;}
        }
        i++;
    }
    if(corte == 0){respuesta[0].posicion = -1;}
    else{entrenadores[respuesta[0].posicion].en_deadlock = 1;}

    return corte;
}

// un_vector = no necesita el primer entrenador, otro_vector = los que necesita el segundo entrenador
// Pega en interseccion el pokemon que encontro y necesita el segundo entrenador. Y devuelve 1 si encontro, devuelve 0 si no
int alguno_de_se_encuentra_en(char** un_vector, char** otro_vector, char** interseccion){
    int i, respuesta;
    i = 0;
    respuesta = 0;
    //los vectores no_necesito y necesito son de tipo terminan en null y no pueden tener null en alguno de sus valores intermedios
    while(un_vector[i] != NULL && deadlock_esta_en_null(un_vector[i], otro_vector) == 0){i++;}
    if(un_vector[i] != NULL){
        *interseccion = un_vector[i];
        respuesta = 1;
    }
    return respuesta;
}

// pokemon = un pokemon que no necesita el primer entrenador, vector = los que necesita el segundo entrenador
int deadlock_esta_en_null(char* pokemon, char** vector){
    int i, corte;
    i = 0;
    corte = 0;
    while(vector[i] != NULL && deadlock_es_pokemon(vector[i], pokemon) == 0){i++;}
    if(vector[i] != NULL){corte = 1;}
    // Devuelve un 1 si encontro un pokemon que le sirve, 0 sino
    return corte;
}

// Si devuelve 1 es un pokemon que necesita el segundo entrenador
int deadlock_es_pokemon(char* unString, char* pokemon){
    int respuesta = 0;
    if(son_iguales_char(unString, VACIO) == 0 && son_iguales_char(unString, pokemon) == 1){
        respuesta = 1;
    }
    return respuesta;
}

void desde_pivote(deadlock_entrenador* entrenadores, int cantidad, int last_pos, elemento_respuesta* respuesta, int* corte){
    int i=1;
    if(alguno_de_se_encuentra_en(entrenadores[respuesta[last_pos].posicion].no_necesito, entrenadores[respuesta[0].posicion].necesito, &(respuesta[last_pos].pokemon)) == 1){
        *corte = last_pos + 1;
    }
    else{
        if(last_pos+1 < cantidad){
            last_pos++;
            while(*corte == 0 && i<cantidad){
                if(entrenadores[i].en_deadlock == 0 && esta_en_respuesta(i, respuesta, cantidad) == 0 && alguno_de_se_encuentra_en(entrenadores[respuesta[last_pos-1].posicion].no_necesito, entrenadores[i].necesito, &(respuesta[last_pos-1].pokemon)) == 1){
                    respuesta[last_pos].posicion = i;//ver si se puede mejorar esta_en_int y pasarle last-pos en vez del tamano total(cantidad)
                    desde_pivote(entrenadores, cantidad, last_pos, respuesta, corte);
                }
                i++;
            }
            if(*corte == 0){
                respuesta[last_pos].posicion = -1;
                respuesta[last_pos-1].pokemon = NULL;
            }
            else{entrenadores[respuesta[last_pos].posicion].en_deadlock = 1;}
        }
    }
}

///////////////////-RECUPERACION DEADLOCK-/////////////////////
void intercambiar(d_entrenador* un_entrenador, char* un_pokemon, d_entrenador* otro_entrenador, char* otro_pokemon){
    if(buscar_y_reemplazar_por_en_actuales_de(un_pokemon, otro_pokemon, un_entrenador) == -1){
        printf("Error en intercambio, no se encontro pokemon %s\n", un_pokemon);
    }
    if(buscar_y_reemplazar_por_en_actuales_de(otro_pokemon, un_pokemon, otro_entrenador) == -1){
        printf("Error en intercambio, no se encontro pokemon %s\n", otro_pokemon);
    }

    sleep(retardo * 5);
    sumarUnCicloCPU();
    sumarUnCicloCPU();
    sumarUnCicloCPU();
    sumarUnCicloCPU();
    sumarUnCicloCPU();

    log_info(logger, "INTERCAMBIO | Entrenador [%i] entrega pokemon %s, entrenador [%i] entrega pokemon %s", un_entrenador->numeroEntrenador,  un_pokemon, otro_entrenador->numeroEntrenador, otro_pokemon);
}

void actualizar_estado_deadlock(d_entrenador* entrenadores, deadlock_entrenador* temp_entrenadores, int cantidad){
    int i;
    for(i=0;i<cantidad;i++){
        actualizar_estado_entrenador(&entrenadores[temp_entrenadores[i].ubicacion], 0);
    }
}

void deteccion_y_recuperacion(d_entrenador* entrenadores, int cant_entrenadores, int cant_deadlock_entrenadores, pthread_t* hilos){
    parametros_deadlock mensaje_deadlock;
    elemento_respuesta* respuesta = malloc(cant_deadlock_entrenadores * sizeof(elemento_respuesta));
    deadlock_entrenador* deadlock_entrenadores = malloc(cant_deadlock_entrenadores * sizeof(deadlock_entrenador));
    int tamano_respuesta, cont;
    cont = 1;

    sacar_en_espera(deadlock_entrenadores, entrenadores, cant_entrenadores);
    /*
    int index;for(index=0;index<4;index++){
        printf("entrenador %i", index);
        printf("   necesarios %s", deadlock_entrenadores[index].necesito[0]);
        printf("   innecesarios %s\n", deadlock_entrenadores[index].no_necesito[0]);
    }
    */
    inicializar_respuesta(respuesta, cant_deadlock_entrenadores);

    tamano_respuesta = detectar_deadlock(deadlock_entrenadores, cant_deadlock_entrenadores, respuesta);
    if(tamano_respuesta > 0){
        //ver si esta bien llenar mensaje_deadlock una sola vez ya que en teoria le estamos pasando la direccion de memoria de los vectores asi que no deberia haber problema
        llenar_mensaje_deadlock(&mensaje_deadlock, entrenadores, deadlock_entrenadores);
        do{
            printf("Iteracion %i\n", cont);
            tratar_circulos(deadlock_entrenadores, cant_deadlock_entrenadores, respuesta, tamano_respuesta, &mensaje_deadlock, hilos);
            reset_en_deadlock(deadlock_entrenadores, cant_deadlock_entrenadores);
            cont++;
            tamano_respuesta = detectar_deadlock(deadlock_entrenadores, cant_deadlock_entrenadores, respuesta);
        }while(tamano_respuesta > 0);
    }
    //printf("No se detectaron mas circulos deadlock (todo el deadlock esta resuelto)\n");
    log_info(logger, "DEADLOCK | Todo el deadlock fue resuelto correctamente");
    actualizar_estado_deadlock(entrenadores, deadlock_entrenadores, cant_deadlock_entrenadores);
    //informar_estado_actual(entrenadores, cant_entrenadores);

    free(respuesta);
    liberar_temporal_comp(deadlock_entrenadores, cant_deadlock_entrenadores);
    free(deadlock_entrenadores);
}


