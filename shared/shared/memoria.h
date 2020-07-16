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

//para empezar a agregar cosas al CACHE
void agregar_mensaje_a_Cache(void* CACHE, uint32_t tamanioMemoria, uint32_t tamanioMinParticion, char* ADMIN_MEMORIA, lista_particiones* laLista, char* algoritmoAsignacion, void* estructuraMensaje, uint32_t sizeDelMensaje, codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS);

void poner_en_particion(void* CACHE, lista_particiones* particionElegida, void* estructura, codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA);//poner el mensaje en la particion elegida
void poner_NEW_en_particion(void* CACHE, lista_particiones* particionElegida, New* estructura, uint32_t* NUMERO_VICTIMA);
void poner_APPEARED_en_particion(void* CACHE, lista_particiones* particionElegida, Appeared* estructura, uint32_t* NUMERO_VICTIMA);
void poner_GET_en_particion(void* CACHE, lista_particiones* particionElegida, Get* estructura, uint32_t* NUMERO_VICTIMA);
void poner_LOCALIZED_en_particion(void* CACHE, lista_particiones* particionElegida, Localized* estructura, uint32_t* NUMERO_VICTIMA);
void poner_CATCH_en_particion(void* CACHE, lista_particiones* particionElegida, Catch* estructura, uint32_t* NUMERO_VICTIMA);
void poner_CAUGHT_en_particion(void* CACHE, lista_particiones* particionElegida, Caught* estructura, uint32_t* NUMERO_VICTIMA);

void verificacionPosicion(uint32_t limiteSuperiorDeParticion, uint32_t posicionSuperiorUltimoDato); //para ver como quedó la información luego de meterla en la particion.




#endif /* SHARED_MEMORIA_H_ */
