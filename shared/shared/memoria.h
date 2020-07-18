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
void inicializar_lista_particiones(lista_particiones* laLista, char* algorAdminMemoria, uint32_t tamanioMemoria); //antes de empezar a meter particiones en CACHE hay que inicializarlo
lista_particiones* crear_particion(lista_particiones* laLista, uint32_t sizeDeLosDatos, char* algorAdminMemoria); //crea una referencia a una particion nueva y devuelve un puntero a esa particion
void corregirNumerosParticiones(lista_particiones* particionOriginal, uint32_t numeroParticionOriginal); //arregla los numeros de particiones despues de que la aberracion de BS cree sus particiones
void seleccionDeVictima(lista_particiones* laLista, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS, char* ALGORITMO_MEMORIA); //selecciona la particion a ser eliminada
void borrarReferenciaAParticion(lista_particiones* laLista, lista_particiones* particionABorrar, uint32_t* PARTICIONES_ELIMINADAS); //borra referencia a la particion (SOLO la marca como libre para que se la pueda pisar)
void consolidarParticion(lista_particiones* laLista, lista_particiones* particionABorrar); //consolida particiones en caso que sea necesario
void compactacion(); //compacta...
lista_particiones* seleccionar_particion_First_Fit(uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS, char* ADMIN_MEMORIA); //selecciona la primera particion que sirva y devuelve un puntero a esa particion
lista_particiones* seleccionar_particion_Best_Fit(uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS, char* ADMIN_MEMORIA);//selecciona la primera particion que sirva y devuelve un puntero a esa particion
lista_particiones* seleccionar_particion_Buddy_System(uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size, char* algoritmoAsignacion); //selecciona particion por BS (le voy a decir bullshit)
void matar_lista_particiones(lista_particiones* laLista); //usar cuando se quiere cerrar el programa
void revision_lista_particiones(lista_particiones* laLista, uint32_t tamanioMemoria); //para fines de control, muestrar por pantalla info de las particiones y espacio libre en memoria
//se fija si en la ultima particion de la memoria hay espacio para meter los datos o no.
uint32_t tenemosEspacio(lista_particiones** auxiliar, lista_particiones** particionElegida, uint32_t tamanioMemoria, uint32_t size);
lista_particiones* comparador_de_candidatas(particionesCandidatas* listaDeCandidatas); //compara particiones candidatas a ser elegidas para algoritmo Best Fit
void matar_lista_particiones_candidatas(particionesCandidatas* listaDeCandidatas); //libera memoria reservada para lista de candidatas del algoritmo Best Fit

//para agregar cosas al CACHE

void poner_en_particion(void* CACHE, lista_particiones* particionElegida, void* estructura, codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA);//poner el mensaje en la particion elegida
void poner_NEW_en_particion(void* CACHE, lista_particiones* particionElegida, New* estructura, uint32_t* NUMERO_VICTIMA);
void poner_APPEARED_en_particion(void* CACHE, lista_particiones* particionElegida, Appeared* estructura, uint32_t* NUMERO_VICTIMA);
void poner_GET_en_particion(void* CACHE, lista_particiones* particionElegida, Get* estructura, uint32_t* NUMERO_VICTIMA);
void poner_LOCALIZED_en_particion(void* CACHE, lista_particiones* particionElegida, Localized* estructura, uint32_t* NUMERO_VICTIMA);
void poner_CATCH_en_particion(void* CACHE, lista_particiones* particionElegida, Catch* estructura, uint32_t* NUMERO_VICTIMA);
void poner_CAUGHT_en_particion(void* CACHE, lista_particiones* particionElegida, Caught* estructura, uint32_t* NUMERO_VICTIMA);

void verificacionPosicion(uint32_t limiteSuperiorDeParticion, uint32_t posicionSuperiorUltimoDato); //para ver como quedó la información luego de meterla en la particion.
//para llamar a la seleccion de particion y la escritura de la misma
void agregar_mensaje_a_Cache(void* CACHE, uint32_t tamanioMemoria, uint32_t tamanioMinParticion, char* ADMIN_MEMORIA, lista_particiones* laLista, char* algoritmoAsignacion, void* estructuraMensaje, uint32_t sizeDelMensaje, codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS);

//para sacar los datos del CACHE

lista_particiones* buscarLaParticionDelMensaje(lista_particiones* laLista, int32_t ID_MENSAJE, int32_t* particionEncontrada); //busca la particion donde se guardo el mensaje con esa ID

//saca los datos de la particion que le dice buscarLaParticionDelMensaje
void sacar_de_particion(void* CACHE, lista_particiones* particionDelMensaje, void* estructura, codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO);
void sacar_NEW_de_particion(void* CACHE, lista_particiones* particionDelMensaje, New* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO);
void sacar_APPEARED_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Appeared* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO);
void sacar_GET_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Get* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO);
void sacar_LOCALIZED_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Localized* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO);
void sacar_CATCH_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Catch* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO);
void sacar_CAUGHT_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Caught* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO);

uint32_t sacar_mensaje_de_Cache(void* CACHE, lista_particiones* laLista, void* estructuraMensaje, int32_t ID_MENSAJE,codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO);

//deben ser las funciones mas idiotas que hice en este TP
uint32_t calcular_bytes_utiles_de_mensaje(void* mensaje, codigo_operacion tipoMensaje);
uint32_t calcular_bytes_utiles_de_mensaje_new(New* estructura);
uint32_t calcular_bytes_utiles_de_mensaje_appeared(Appeared* estructura);
uint32_t calcular_bytes_utiles_de_mensaje_get(Get* estructura);
uint32_t calcular_bytes_utiles_de_mensaje_localized(Localized* estructura);
uint32_t calcular_bytes_utiles_de_mensaje_catch(Catch* estructura);
uint32_t calcular_bytes_utiles_de_mensaje_caught(Caught* estructura);

#endif /* SHARED_MEMORIA_H_ */
