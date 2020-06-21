/*
 * colaCaught.c
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#include "colaCaught.h"

int tamano_maximo;
int suscripcion = 0;
int envio = 0;
sem_t sem_envio;

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

void agregar_a_cola_caught(mensaje_caught* contenido){
    elemento_cola_caught* nuevoMensaje = malloc(sizeof(elemento_cola_caught));
    nuevoMensaje->mensaje = *contenido;
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

    sem_wait(&sem_envio);
    while(pos < tamano_maximo && vector_envios[pos] != num_envio){pos++;}
    sem_post(&sem_envio);
    if(pos == tamano_maximo){
        pos = -1;
        printf("error ningun entrenador figura que haya hecho el envio %i\n", num_envio);
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
    init_sem(&sem_envio, 1);
}

void liberar_vector_envios(){
    sem_destroy(&sem_envio);
    free(vector_envios);
}

void armar_enviar_catch(char* pokemon, int pos_x, int pos_y, int posicion, int conexion){
    Catch mensaje;
    mensaje.pokemon = pokemon;
    mensaje.pos_x = pos_x;
    mensaje.pos_y = pos_y;

    //este mandar mensaje tambien deberia tener en cuenta la variable envio que posteriormente es la que identifica al entrenador que envio el mensaje
    //mandar_mensaje(CATCH, &mensaje, conexion);
    /*
    if(suscripcion == 0){
        suscribirse_a(CAUGHT);
        suscripcion = 1;
    }
    */
    sem_wait(&sem_envio);
    vector_envios[posicion] = envio;
    envio++;
    sem_post(&sem_envio);
}




