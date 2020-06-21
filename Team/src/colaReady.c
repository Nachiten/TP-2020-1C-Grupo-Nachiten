/*
 * colaReady.c
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#include "colaReady.h"

int flag_nuevo_primero = 0;
int flag_sjf = 0;
float alfa = 0.5;
int tamano_vector_estimaciones = 0;

estructura_estimacion* vector_estimaciones;
cola_Ready cola_ready;

///////////////////-COLA READY-/////////////////////
void inicializar_cola_ready(){
	cola_ready.inicio = NULL;
	cola_ready.fondo = NULL;
}

void eliminar_cola_ready(){
    elemento_cola_ready* actual = cola_ready.inicio;
    while(actual != NULL){
	elemento_cola_ready* temp = actual;
        actual = actual->next;
	free(temp);
    }
    if(flag_sjf == 1){
        liberar_vector_estimaciones();
    }
}

int cola_ready_vacia(){
    int respuesta = 0;
    if(cola_ready.inicio == NULL){
        respuesta = 1;
    }
    return respuesta;
}

void actualizar_cola_ready(){
    elemento_cola_ready* temp = cola_ready.inicio;
    cola_ready.inicio = cola_ready.inicio->next;
    free(temp);
}

int pos_primero_de_ready(){
    int respuesta = cola_ready.inicio->pos_entrenador;
    actualizar_cola_ready();
    if(flag_sjf == 1){
        vector_estimaciones[respuesta].real_ant = 0;
        flag_nuevo_primero = 0;
    }
    return respuesta;
}

void agregar_a_ready(int posicion){
    elemento_cola_ready* nuevoElemento = malloc(sizeof(elemento_cola_ready));
    nuevoElemento->pos_entrenador = posicion;
    if(flag_sjf == 0){
        pusheo_en_ready(nuevoElemento);
    }
    else{
        calcular_estimacion(posicion);
        pusheo_ordenado_en_ready(nuevoElemento);
    }
}

void pusheo_en_ready(elemento_cola_ready* nuevoElemento){
    nuevoElemento->next = NULL;
    if(cola_ready_vacia() == 1){
        cola_ready.inicio = nuevoElemento;
        cola_ready.fondo = nuevoElemento;
    }
    else{
        cola_ready.fondo->next = nuevoElemento;
        cola_ready.fondo = nuevoElemento;
    }
}

void calcular_estimacion(int pos){
        //elemento->est_actual = (elemento->est_ant * alfa) + (elemento->real_ant * (1 - alfa));
        vector_estimaciones[pos].est_actual = (vector_estimaciones[pos].est_actual * alfa) + (vector_estimaciones[pos].real_ant * (1 - alfa));
}

void pusheo_ordenado_en_ready(elemento_cola_ready* nuevoElemento){
    if(cola_ready_vacia() == 1){
        nuevoElemento->next = NULL;
        cola_ready.inicio = nuevoElemento;
        cola_ready.fondo = nuevoElemento;
    }
    else{
        int corte = 0;
        elemento_cola_ready* auxiliar = cola_ready.inicio;
        if(tiene_menor_estimacion_que(nuevoElemento->pos_entrenador, auxiliar->pos_entrenador) == 1){
            nuevoElemento->next = auxiliar;
            cola_ready.inicio = nuevoElemento;
            flag_nuevo_primero = 1;
        }
        else{
            while(corte == 0 && auxiliar->next != NULL){
                if(tiene_menor_estimacion_que(nuevoElemento->pos_entrenador, auxiliar->next->pos_entrenador) == 1){
                    corte = 1;
                }
                if(corte == 0){
                    auxiliar = auxiliar->next;
                }
            }
            if(corte == 1){
                nuevoElemento->next = auxiliar->next;
                auxiliar->next = nuevoElemento;
            }
            else{
                nuevoElemento->next = NULL;
                auxiliar->next = nuevoElemento;
                cola_ready.fondo = nuevoElemento;
            }
        }
    }
}

int tiene_menor_estimacion_que(int pos_unElemento, int pos_otroElemento){
    int respuesta = 0;
    if(vector_estimaciones[pos_unElemento].est_actual < vector_estimaciones[pos_otroElemento].est_actual){
        respuesta = 1;
    }
    return respuesta;
}

///////////////////-VECTOR-ESTIMACIONES-/////////////////////
void inicializar_vector_estimaciones(int estimacion_base, int cantidad_entrenadores){
    int i;
    float valor_estimacion = (float)estimacion_base;
    flag_sjf = 1;
    vector_estimaciones = malloc(cantidad_entrenadores * sizeof(estructura_estimacion));
    for(i=0;i<cantidad_entrenadores;i++){
        vector_estimaciones[i].real_ant = valor_estimacion;
        vector_estimaciones[i].est_actual = valor_estimacion;
    }
    tamano_vector_estimaciones = cantidad_entrenadores;
}

void liberar_vector_estimaciones(){
    free(vector_estimaciones);
}

void notificar_rafagas_reales(int posicion, int cant_rafagas){
    vector_estimaciones[posicion].real_ant = (float)cant_rafagas;
}

int incrementar_rafaga_actual_en_exec(int posicion){
    int respuesta = 1;
    vector_estimaciones[posicion].real_ant+=1;
    if(flag_nuevo_primero == 1){
        int estimacion_actualizada = recalcular_estimacion(posicion);
        respuesta = comparar_estimaciones(estimacion_actualizada);
        if(respuesta == -1){
            setear_est_actual_ant_de(estimacion_actualizada, posicion);
        }
    }
    return respuesta;
}

void setear_nuevo_primero(int valor){
    flag_nuevo_primero = valor;
}

void setear_est_actual_ant_de(int valor, int posicion){
    vector_estimaciones[posicion].est_actual = valor;
    vector_estimaciones[posicion].real_ant = valor;
}

int recalcular_estimacion(int posicion){
    int respuesta = vector_estimaciones[posicion].est_actual - vector_estimaciones[posicion].real_ant;
    if(respuesta<0){respuesta = 0;}
    return respuesta;
}

int estimacion_primero_de_ready(){
    return vector_estimaciones[cola_ready.inicio->pos_entrenador].est_actual;
}

