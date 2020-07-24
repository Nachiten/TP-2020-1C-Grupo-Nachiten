/*
 * colaCaught.c
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#include "colaCaught.h"

int tamano_maximo;
int* vector_envios;

cola_Caught cola_caught;

///////////////////-COLA CAUGHT-/////////////////////
void inicializar_cola_caught(int cant_entrenadores){
    cola_caught.inicio = NULL;
    cola_caught.fondo = NULL;
    inicializar_vector_envios(cant_entrenadores);
}

void eliminar_cola_caught(){
    elemento_cola_caught* actual = cola_caught.inicio;
    while(actual != NULL){
	elemento_cola_caught* temp = actual;
	actual = actual->next;
	free(temp);
    }    
    liberar_vector_envios();
}

int esta_vacia_cola_caught(){
    int respuesta = 0;    
    if(cola_caught.inicio == NULL){
	respuesta = 1;
    }
    return respuesta;
}

void actualizar_cola_caught(){
    elemento_cola_caught* temp = cola_caught.inicio;
    cola_caught.inicio = cola_caught.inicio->next;
    free(temp);
}

void agregar_a_cola_caught(Caught* contenido){
    elemento_cola_caught* nuevoMensaje = malloc(sizeof(elemento_cola_caught));
    nuevoMensaje->mensaje.num_envio = contenido->corrID;
    nuevoMensaje->mensaje.resultado = contenido->resultado;
    nuevoMensaje->next = NULL;
    if(esta_vacia_cola_caught() == 1){
	cola_caught.inicio = nuevoMensaje;
	cola_caught.fondo = nuevoMensaje;
    }
    else{
	cola_caught.fondo->next = nuevoMensaje;
	cola_caught.fondo = nuevoMensaje;
    }
}

int resultado_primero_cola_caught(){
    int respuesta = cola_caught.inicio->mensaje.resultado;
    actualizar_cola_caught();
    return respuesta;
}

int posicion_primero_cola_caught(){
    int pos, num_envio;
    pos = 0;
    num_envio = cola_caught.inicio->mensaje.num_envio;
    if(num_envio != -1){
        while(pos < tamano_maximo && vector_envios[pos] != num_envio){pos++;}
        if(pos < tamano_maximo){eliminar_elemento_lista_ids(num_envio);}
        else{
            pos = -1;
            printf("error ningun entrenador figura que haya hecho el envio %i\n", num_envio);
        } 
    }
    else{
        pos = cola_caught.inicio->mensaje.resultado;//el valor -1 esta reservado para mensaje de comportamiento default, en ese caso resultado es la posicion del entrenador     
        cola_caught.inicio->mensaje.resultado = 1;
    }
    return pos;
}

///////////////////-VECTOR ENVIOS-/////////////////////
void inicializar_vector_envios(int tamano){
    int i;
    vector_envios = malloc(tamano * sizeof(int));
    for(i=0;i<tamano;i++){
        vector_envios[i] = -1;
    }
    tamano_maximo = tamano;
}

void liberar_vector_envios(){
    free(vector_envios);
}

void registrar_id_mensaje_catch(int idMensaje, int posicion){
    vector_envios[posicion] = idMensaje;
}




