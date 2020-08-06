/*
 * colaCaught.c
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#include "colaCaught.h"

int tamano_maximo;
int suscripcion = 0;
//int envio = 0;
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

    while(pos < tamano_maximo && vector_envios[pos] != num_envio)
    {
    	pos++;
    }

    sem_post(&sem_envio);
    if(pos == tamano_maximo){
        pos = -1;
        printf("Ningún entrenador figura que haya hecho el envio: %i, mensaje descartado.\n", num_envio);
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

void armar_enviar_catch(char* pokemon, int pos_x, int pos_y, int posicion, d_entrenador* entrenador)
{
	int32_t size = 1;
	int32_t tamanioRecibido = 1;
	codigo_operacion cod_op;
	idMensaje* estructura = malloc(sizeof(idMensaje));

	Catch* mensaje = malloc(sizeof(Catch));
    mensaje->largoNombre = strlen(pokemon);
    mensaje->nombrePokemon = pokemon;
    mensaje->posPokemon.x = pos_x;
    mensaje->posPokemon.y = pos_y;
	mensaje->ID = 0;
	mensaje->corrID = -2;

    //logueamos el intento de atrapar un Pokemon
	sem_wait(semLog);
	log_info(logger, "El entrenador [%i] está intentando atrapar un %s!! en las coordenadas [%i,%i].", (posicion+1), pokemon, pos_x, pos_y);
	sem_post(semLog);

	sleep(retardo);
	sumarUnCicloCPU(entrenador);

	int32_t elSocketoide = establecer_conexion(IP,PUERTO);

	if(elSocketoide > 0)
	{
		mandar_mensaje(mensaje, CATCH, elSocketoide);

		tamanioRecibido = recv(elSocketoide, &cod_op, sizeof(codigo_operacion),MSG_WAITALL);
		bytesRecibidos(tamanioRecibido);

		tamanioRecibido = recv(elSocketoide, &size, sizeof(int32_t),MSG_WAITALL);
		bytesRecibidos(tamanioRecibido);

		recibir_mensaje(estructura, IDMENSAJE, elSocketoide);

		sem_wait(semLog);
		log_info(logger, "LLego un mensaje IDMENSAJE, datos:\nID del mensaje guardado en Broker:%i.", estructura->id_mensaje);
		sem_post(semLog);

	    sem_wait(&sem_envio);
	    vector_envios[posicion] = estructura->id_mensaje;
	    //envio++;
	    sem_post(&sem_envio);
	}

	//Broker no esta andando, va a comportamiento x default
	else
	{
		sem_wait(semLog);
		log_info(logger, "Conexión a Broker fallida, Se inicia operación por default: Asumir que el Entrenador logró atrapar al Pokemon.");
		sem_post(semLog);

		// En el caso default se supone que el pokemon SI se atrapó
		mensaje_caught* resultadoExitosoTrucho = malloc(sizeof(mensaje_caught));
		resultadoExitosoTrucho->resultado = 1;

		//le invento una ID que solo existe en TEAM
		sem_wait(&semIDFALSATEAM);
		iDFalsaTeam++;
		resultadoExitosoTrucho->num_envio = iDFalsaTeam;
		sem_post(&semIDFALSATEAM);

	    sem_wait(&sem_envio);
	    //vector_envios[posicion] = iDFalsaTeam;
	    vector_envios[posicion] = resultadoExitosoTrucho->num_envio;
	    sem_post(&sem_envio);

		pthread_mutex_lock(&colaCaught_mutex);
		agregar_a_cola_caught(resultadoExitosoTrucho);//toda respuesta la guarda en la cola caught o "cola de resultados"
		pthread_mutex_unlock(&colaCaught_mutex);
		sem_post(&colaCaught_llenos);
	}
    free(estructura);
}
