/*
 * funciones_team.c
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#include "funciones_team.h"

///////////////////-SEMAFOROS-/////////////////////
void init_sem(sem_t* semaforo, int valor){
    sem_init(semaforo, 0, valor);
}

void init_mutex(pthread_mutex_t* mutex){
    pthread_mutex_init(mutex, NULL);
}

void destroy_mutex(pthread_mutex_t* mutex){
    pthread_mutex_destroy(mutex);
}

///////////////////-HILOS-/////////////////////
void activar_hilo_administrador_cola_ready(pthread_t* hilo_cola_ready){
    pthread_create(hilo_cola_ready, NULL, administrar_cola_ready, NULL);
}

void activar_hilo_administrador_cola_caught(pthread_t* hilo_cola_caught){
    pthread_create(hilo_cola_caught, NULL, administrar_cola_caught, NULL);
}

void activar_hilo_circulo_deadlock(parametros_deadlock* parametros, pthread_t* hilo){
    pthread_create(hilo, NULL, ciclo_deadlock, parametros);
}

void activar_hilo(pthread_t* hilo, void* (*funcion)(void*), void* parametros){
    pthread_create(hilo, NULL, funcion, parametros);
}

void join_hilo(pthread_t* hilo){
    pthread_join(*hilo, NULL);
}

void join_hilos(pthread_t* hilos, int cantidad){
    int i;
    printf("join hilos ");
    for(i=0;i<cantidad;i++){
        pthread_join(hilos[i], NULL);
        printf("%i ", i);
    }
    printf("\n");
}

///////////////////-EMISION DE MENSAJES-/////////////////////
void preparar_mensajes_get(elemento_objetivo* objetivo, int tamano){
    int i;
    for(i=0;i<tamano;i++){
       armar_preparar_mensaje_get(objetivo[i].pokemon, i); 
    }
}

void armar_preparar_mensaje_get(char* pokemon, int pos_en_objetivo_global){
    Get mensaje = armar_mensaje_get(pokemon);
    agregar_mensaje_get_a_cola_mensajes_broker(mensaje, pos_en_objetivo_global);
}

Get armar_mensaje_get(char* unPokemon){
    Get mensaje;
    mensaje.pokemon = unPokemon;
    return mensaje;
}

void armar_enviar_registrar_mensaje_catch(char* pokemon, int pos_x, int pos_y, int posicion){
    Catch mensaje = armar_mensaje_catch(pokemon, pos_x, pos_y);
    int idMensaje = enviar_mensaje_catch(&mensaje);
    registrar_id_mensaje_catch(idMensaje, posicion);
}

Catch armar_mensaje_catch(char* pokemon, int pos_x, int pos_y){
    Catch mensaje;
    mensaje.pokemon = pokemon;
    mensaje.pos_x = pos_x;
    mensaje.pos_y = pos_y;
    return mensaje;
}
/*
int enviar_mensaje_catch(Catch* mensaje){
    int respuesta = emitir_mensaje_a_broker(mensaje, CATCH);
    return respuesta;
}
*/
//////////////////-OBJETIVO ACTUAL-/////////////////////
int cantidad_de_veces_en_objetivo_actual(char* pokemon, elemento_objetivo* objetivo_global, int tamano_objetivo, int* pos_pokemon_en_objetivo){
    int i, cantidad, pos;
    cantidad = 0;
    pos = buscar_pos_en_objetivo(pokemon, objetivo_global, tamano_objetivo);
    if(pos != -1){cantidad = cantidad_repeticiones_de(objetivo_global[i]);}
    *pos_pokemon_en_objetivo = pos;
    return cantidad;
}

int cantidad_repeticiones_de(elemento_objetivo pokemon_objetivo){
    return objetivo[i].repeticiones;
}

int eliminar_de_objetivo_global(char* pokemon, elemento_objetivo* objetivo_global, int tamano_objetivo){
    int pos, corte;
    corte = 0;
    pos = buscar_pos_en_objetivo(pokemon, objetivo_global, tamano_objetivo);
    if(pos != -1){
        if(cantidad_repeticiones_de(objetivo_global[pos]) > 0){
            corte = 1;
            decrementar_repeticiones(&objetivo_global[pos]);
            if(cantidad_repeticiones_de(objetivo_global[pos]) > 0){
                if(pokemon_esta_por_eliminarse(objetivo_global[pos]) == 1){sacar_de_memoria(&objetivo_global[pos]);}
            }
            else{
                eliminar_elemento_lista_ids(objetivo_global[pos].id_mensaje);
                eliminar_pokemon_de_mensajes_get(objetivo_global[pos].pokemon);
            }
        }
        else{printf("Error team ya atrapo la cantidad necesaria de pokemon %s\n", pokemon);}
    }
    else{printf("Error pokemon %s no se ecuentra en objetivo_actual\n", pokemon);}//cambiar por el logger despues
    return corte;
}

void decrementar_repeticiones(elemento_objetivo* pokemon_objetivo){
    pokemon_objetivo->repeticiones-=1;
}

void guardar_en_memoria(elemento_objetivo* pokemon_objetivo){
    pokemon_objetivo->estado_en_memoria = 1;
}

void sacar_de_memoria(elemento_objetivo* pokemon_objetivo){
    pokemon_objetivo->estado_en_memoria = 0;
}

void poner_a_punto_de_eliminacion_de_memoria(elemento_objetivo* pokemon_objetivo){
    pokemon_objetivo->estado_en_memoria = 2;
}

void esta_en_memoria(elemento_objetivo pokemon_objetivo){
    int respuesta = 1;
    if(pokemon_objetivo.estado_en_memoria == 0){respuesta = 0;}
    return respuesta;
}

int pokemon_esta_por_eliminarse(elemento_objetivo pokemon_objetivo){
    int respuesta = 0;
    if(pokemon_objetivo.estado_en_memoria == 2){respuesta = 1;}
    return respuesta;
}

int buscar_pos_en_objetivo(char* pokemon, elemento_objetivo* objetivo, int tamano_objetivo){
    int pos = 0:
    while(pos<tamano_objetivo && son_iguales_char(pokemon, objetivo[pos].pokemon) == 0){pos++;}
    if(pos == tamano_objetivo){
        pos = -1;
        printf("Error pokemon %s no se ecuentra en objetivo_actual\n", pokemon);
    }
    return pos;
}

void verificar_cantidad_pokemon_a_partir_de_en(char* pokemon, int pos_inicial, char** objetivo, int tamano){
    if(sigue_habiendo_pokemon_a_partir_de_en(pokemon, pos_inicial, objetivo, tamano) == 0){
        eliminar_id_de_pokemon(pokemon);
    }
}

int sigue_habiendo_pokemon_a_partir_de_en(char* pokemon, int pos_inicial, char** objetivo, int tamano){
    int i, respuesta;
    respuesta = 0;
    i = pos_inicial;
    while(i<tamano && respuesta == 0){
        if(objetivo[i] != NULL && son_iguales_char(objetivo[i], pokemon) == 1){
            respuesta = 1;
	}
	i++;
    }
    return respuesta;
}

//////////////////-ENTRENADORES-/////////////////////
void cambiar_estado_a(d_entrenador* entrenador, int nuevo_estado){
    entrenador->estado = nuevo_estado;
}

void bloquear(d_entrenador* entrenador, int estado_bloqueado){
    if(entrenador->estado != BLOCKED){
	cambiar_estado_a(entrenador, BLOCKED);
    }
    entrenador->estado_block = estado_bloqueado;
}

int cumple_condicion_deadlock(d_entrenador entrenador){
    int respuesta = 0;
    if(entrenador.estado == BLOCKED && entrenador.estado_block == EN_ESPERA){
        respuesta = 1;
    }
    return respuesta;
}

int cant_objetivos(d_entrenador entrenador){
    int i = 0;
    while(entrenador.objetivo[i] != NULL){i++;}
    return i;
}

int cant_pokemones_actuales(d_entrenador entrenador, int cantidad_objetivos){
    int i = 0;
    while(i<cantidad_objetivos && entrenador.pokemones_actuales[i] != NULL){
        i++;
    }
    return i;
}

void cant_objetivos_y_actuales_de(d_entrenador entrenador, int* cantidad_objetivos, int* cantidad_actuales){
    int temp_objetivos, temp_actuales;
    temp_objetivos = cant_objetivos(entrenador);
    temp_actuales = cant_pokemones_actuales(entrenador, temp_objetivos);
    if(cantidad_objetivos != NULL){*cantidad_objetivos = temp_objetivos;}
    if(cantidad_actuales != NULL){*cantidad_actuales = temp_actuales;}
}

int esta_en_objetivo_individual(char* pokemon, char** objetivo){
    int respuesta = esta_en_null(pokemon, objetivo);
    return respuesta;
}

int cantidad_de_veces_en_objetivo_individual(char* pokemon, char** objetivo){
    int respuesta = se_encuentra_en_null(pokemon, objetivo);
    return respuesta;
}

int esta_en_actuales(char* pokemon, char** actuales, int tamano){
    int respuesta = esta_en_char_lleno(pokemon, actuales, tamano);
    return respuesta;
}

int cantidad_de_veces_en_actuales(char* pokemon, char** actuales, int tamano){
    int respuesta = se_encuentra_en_char_lleno(pokemon, actuales, tamano);
    return respuesta;
}

int diferencia_de_repeticiones_de_en_objetivos_y_actuales(char* pokemon, char** objetivo, char** actuales, int tamano_actuales){
    int diferencia = cantidad_de_veces_en_objetivo_individual(pokemon, objetivo) - cantidad_de_veces_en_actuales(pokemon, actuales, tamano_actuales);
    return diferencia;
}

int calcular_objetivo_propio(d_entrenador entrenador){
    int respuesta, objetivo, actuales;
    cant_objetivos_y_actuales_de(entrenador, &objetivo, &actuales);
    respuesta = objetivo - actuales;
    return respuesta;
}

int esta_en_el_limite(d_entrenador entrenador){
    int retorno = 0;
    if(calcular_objetivo_propio(entrenador) == 0){retorno = 1;}
    return retorno;
}

int esta_terminado(d_entrenador entrenador){
    int i, resultado, objetivos, actuales;
    i = 0;
    resultado = 0;
    cant_objetivos_y_actuales_de(entrenador, &objetivos, &actuales);
    while(i<objetivos && diferencia_de_repeticiones_de_en_objetivos_y_actuales(entrenador.objetivo[i], entrenador.objetivo, entrenador.pokemones_actuales, actuales) == 0){i++;}
    if(i == objetivos){resultado = 1;}
    return resultado;
}

void agregar_captura(d_entrenador* entrenador, char* pokemon){
    int i = 0;
    while(entrenador->pokemones_actuales[i] != NULL){i++;}//itero hasta encontrar un espacio vacio, se supone que cualquie entrenador en esta funcion NO debe estar completo
    entrenador->pokemones_actuales[i] = pokemon;
}

int pokemon_se_repitio_antes_en_objetivo(char* pokemon, char** objetivo, int pos_pokemon){
    int i, respuesta;
    i=0;
    respuesta = 0;
    while(i<pos_pokemon && son_iguales_char(objetivo[i], pokemon) == 0){i++;}
    if(i<pos_pokemon){respuesta = 1;}
    return respuesta;
}

int buscar_y_reemplazar_por_en_actuales_de(char* pokemon_a_buscar, char* pokemon_reemplazo, d_entrenador* entrenador){
    int i, actuales;
    i = 0; 
    cant_objetivos_y_actuales_de(*entrenador, NULL, &actuales);
    while(i<actuales && strcmp(entrenador->pokemones_actuales[i], pokemon_a_buscar) != 0){i++;}
    if(i<actuales){entrenador->pokemones_actuales[i] = pokemon_reemplazo;}
    else{i = -1;}
    return i;
}

void actualizar_estado_entrenador(d_entrenador* entrenador, int tipo_control){
    switch(tipo_control){
        case 0: 
            if(esta_en_el_limite(*entrenador) == 1 && esta_terminado(*entrenador) == 1){
                cambiar_estado_a(entrenador, EXIT);
            }
            break;
        case 1: 
            if(esta_en_el_limite(*entrenador) == 1){
                if(esta_terminado(*entrenador) == 1){
                   cambiar_estado_a(entrenador, EXIT); 
                }
                else{bloquear(entrenador, EN_ESPERA);}
            }
    }
}

///////////////////-COLA MENSAJES-/////////////////////
//ver despues si se le puede pasar a esta funcon mensaje como copia de una estructura y no como un puntero a una estructura
int filtrar_mensaje(void* mensaje, elemento_objetivo* objetivo_global, int tamano_objetivo){
    int respuesta, pos;
    respuesta = 0;
    char* pokemon = pokemon_de_mensaje(mensaje);
    pos = buscar_pos_en_objetivo(pokemon, objetivo_global, tamano_objetivo);
    if(pos != -1 && esta_en_memoria(objetivo_global[pos]) == 0){
        guardar_en_memoria(&objetivo_global[pos]);      
        respuesta = 1;
    }
    return respuesta;
}

char* pokemon_de_mensaje(Appeared* mensaje){return mensaje->pokemon;}
char* pokemon_de_mensaje(Localized* mensaje){return mensaje->pokemon;}

///////////////////-VECTORES CHAR-/////////////////////
int esta_en_char(char* pokemon, char** vector, int tamano){
    int i, corte;
    i = 0;
    corte = 0;
    while(i<tamano && corte == 0){
	if(vector[i] != NULL && strcmp(vector[i], pokemon) == 0){
            corte = 1;
        }
	i++;
    }    
    return corte;
}

int se_encuentra_en_char(char* pokemon, char** vector, int tamano){
    int i, resultado;
    resultado = 0;    
    for(i=0;i<tamano;i++){
	if(vector[i] != NULL && strcmp(vector[i], pokemon) == 0){
            resultado++;
        }
    }    
    return resultado;
}

int esta_en_null(char* pokemon, char** vector){
    int i, corte;
    i = 0;
    corte = 0;
    while(vector[i] != NULL && strcmp(vector[i], pokemon) != 0){i++;}
    if(vector[i] != NULL){corte = 1;}
    return corte;
}

int se_encuentra_en_null(char* pokemon, char** vector){
    int i, resultado;
    i = 0;
    resultado = 0;    
    while(vector[i] != NULL){
	if(strcmp(vector[i], pokemon) == 0){
            resultado++;
        }
	i++;
    }    
    return resultado;
}

int esta_en_char_lleno(char* pokemon, char** vector, int tamano){
    int i, corte;
    i = 0;
    corte = 0;
    while(i<tamano && son_iguales_char(vector[i], pokemon) == 0){i++;} 
    if(i<tamano){corte = 1;}
    return corte;
}

int se_encuentra_en_char_lleno(char* pokemon, char** vector, int tamano){
    int i, resultado;
    resultado = 0;    
    for(i=0;i<tamano;i++){
	if(son_iguales_char(vector[i], pokemon) == 1){
            resultado++;
        }
    }    
    return resultado;
}

int son_iguales_char(char* unString, char* otroString){
    int respuesta = 1;
    if(string_equals_ignore_case(unString, otroString) == 0){
        respuesta = 0;
    }
    return respuesta;
}

void llenar_con(int inicial, int limite, char** vector, char* pokemon){
    int i;
    for(i=inicial;i<limite;i++){
        vector[i] = pokemon;
    }
}

///////////////////-FUNCIONES MAIN-/////////////////////
int calcular_mas_cerca_de(int pos_x, int pos_y, d_entrenador* entrenadores, int cantidad){
    int i, distancia, distancia_minimo, pos_a_enviar;
    pos_a_enviar = -1;
    distancia_minimo = -1;
    for(i=0;i<cantidad;i++){
	if(entrenadores[i].estado == NEW || (entrenadores[i].estado == BLOCKED && entrenadores[i].estado_block == ACTIVO)){
            distancia = distancia_a(pos_x, pos_y, entrenadores[i].posicion[0], entrenadores[i].posicion[1]);
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

void informar_estado_actual(d_entrenador* entrenadores, int cantidad){
    int i;
    printf("Estado actual del sistema\n");
    for(i=0;i<cantidad;i++){
        printf("  Entrenador %i: estado %i\n", i, entrenadores[i].estado);
        if(entrenadores[i].estado == BLOCKED){
            int objetivos, actuales, j;
            cant_objetivos_y_actuales_de(entrenadores[i], &objetivos, &actuales);
            printf("    objetivos: ");
            for(j=0;j<objetivos;j++){printf("%s ", entrenadores[i].objetivo[j]);}
            printf("\n");
            printf("    actuales: ");
            for(j=0;j<actuales;j++){if(entrenadores[i].pokemones_actuales[j] != NULL){printf("%s ", entrenadores[i].pokemones_actuales[j]);}}
            printf("\n");
        }
    }
}

void se_cumplio_objetivo(d_entrenador* entrenadores, int cantidad){
    int i = 0;
    while(i<cantidad && entrenadores[i].estado == EXIT){
        i++;
    }
    if(i == cantidad){printf("\nTodos los entrenadores estan en exit\n");}
    else{printf("\nEntrenador %i no esta en exit\n", i);}
}

int cant_en_espera(d_entrenador* entrenadores, int cantidad){
    int cont, i;
    cont = 0;
    for(i=0;i<cantidad;i++){
        if(cumple_condicion_deadlock(entrenadores[i]) == 1){
            cont++;
	}
    }
    return cont;
}

///////////////////-MOVERSE-/////////////////////
void llegar_por_eje(d_entrenador* entrenador, int pos, int eje){
    while(entrenador->posicion[eje] != pos){
        //Sleep(1000);//delay X segundos por configuracion
	if((entrenador->posicion[eje] - pos) > 0){
            entrenador->posicion[eje]-=1;
	}
	else{entrenador->posicion[eje]+=1;}
    }
}

void llegar_por_eje_con_quantum(d_entrenador* entrenador, int pos, int eje, int rafagas, int* resto, int entrenador_pos){
    int cont = rafagas - (*resto);
    while(entrenador->posicion[eje] != pos){
        //Sleep(1000);//delay X segundos por configuracion
        if(cont < rafagas){
            if((entrenador->posicion[eje] - pos) > 0){
                entrenador->posicion[eje]-=1;
            }
            else{entrenador->posicion[eje]+=1;}
            cont++;
        }
        else{
            cont = 0;printf("quantum acabado entrenador %i\n", entrenador_pos);
            libero_exec_me_agrego_a_ready_y_espero(entrenador, entrenador_pos);
        }
    }
    *resto = rafagas - (cont);
}

void llegar_por_eje_con_seguimiento_de_rafaga(d_entrenador* entrenador, int pos, int eje, int entrenador_pos){
    while(entrenador->posicion[eje] != pos){
	//Sleep(1000);//delay X segundos por configuracion
        if((entrenador->posicion[eje] - pos) > 0){
            entrenador->posicion[eje]-=1;
	}
	else{entrenador->posicion[eje]+=1;}
        if(incrementar_rafaga_actual_en_exec(entrenador_pos) == -1){
            printf("desalojo entrenador%i\n", entrenador_pos);
            libero_exec_me_agrego_a_ready_y_espero(entrenador, entrenador_pos);
        }
    }
}




