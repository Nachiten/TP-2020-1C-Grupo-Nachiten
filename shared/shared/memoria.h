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

//para manejo de la lista de las particiones existentes en CACHE
void inicializar_lista_particiones(lista_particiones* laLista); //antes de empezar a meter particiones en CACHE hay que inicializarlo
lista_particiones* crear_particion(lista_particiones* laLista, uint32_t sizeDeLosDatos); //crea una referencia a una particion nueva y devuelve un puntero a esa particion
lista_particiones* seleccionar_particion_First_Fit(uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size); //selecciona la primera particion que sirva y devuelve un puntero a esa particion



void matar_lista_particiones(lista_particiones* laLista); //usar cuando se quiere cerrar el programa

//para no repetir codigo 2 veces, se fija si en la ultima particion de la memoria hay espacio para meter los datos o no.
uint32_t tenemosEspacio(lista_particiones* auxiliar, lista_particiones* particionElegida, uint32_t tamanioMemoria, uint32_t size);

//para empezar a agregar cosas al CACHE
void agregar_mensaje_a_Cache(void* CACHE, uint32_t tamanioMemoria, uint32_t tamanioMinParticion, lista_particiones* laLista, char* algoritmoAsignacion, void* estructuraMensaje, uint32_t sizeDelMensaje, codigo_operacion tipoMensaje);

void poner_en_particion(void* CACHE, lista_particiones* particionElegida, void* estructura, codigo_operacion tipoMensaje);//poner el mensaje en la particion elegida
void poner_NEW_en_particion(void* CACHE, lista_particiones* particionElegida, New* estructura);
void poner_APPEARED_en_particion(void* CACHE, lista_particiones* particionElegida, Appeared* estructura);
void poner_GET_en_particion(void* CACHE, lista_particiones* particionElegida, Get* estructura);
void poner_LOCALIZED_en_particion(void* CACHE, lista_particiones* particionElegida, Localized* estructura);
void poner_CATCH_en_particion(void* CACHE, lista_particiones* particionElegida, Catch* estructura);
void poner_CAUGHT_en_particion(void* CACHE, lista_particiones* particionElegida, Caught* estructura);

void verificacionPosicion(uint32_t limiteSuperiorDeParticion, uint32_t posicionSuperiorUltimoDato); //para ver como quedó la información luego de meterla en la particion.


#endif /* SHARED_MEMORIA_H_ */
