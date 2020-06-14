/*
 * memoria.h
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#ifndef SHARED_MEMORIA_H_
#define SHARED_MEMORIA_H_

#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include"estructuras.h"

//para tomar los valores del archivo config que dicen como manejar la memoria
void cargar_config_memoria(t_config* config, char* TAMANIO_MEM, char* TAMANIO_MIN_PART, char* ADMIN_MEMORIA, char* ALGOR_REEMPLAZO, char* ALGOR_ASIGN_PARTICION, char* FRECUEN_COMPACT);

//para manejo de la lista de las particiones existentes en CACHE
void inicializar_lista_particiones(lista_particiones* laLista); //antes de empezar a meter particiones en CACHE hay que inicializarlo
lista_particiones* crear_particion(lista_particiones* laLista, uint32_t sizeDeLosDatos); //crea una referencia a una particion nueva y devuelve un puntero a esa particion
void matar_lista_particiones(lista_particiones* laLista); //usar cuando se quiere cerrar el programa

//para empezar a agregar cosas al CACHE
void agregar_mensaje_a_Cache(void* CACHE, t_mensaje* estructuraMensaje, codigo_operacion tipoMensaje);



#endif /* SHARED_MEMORIA_H_ */
