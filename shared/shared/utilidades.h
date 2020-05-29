/*
 * utilidades.h
 *
 *  Created on: 28 may. 2020
 *      Author: utnso
 */

#ifndef SHARED_UTILIDADES_H_
#define SHARED_UTILIDADES_H_

#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include"estructuras.h"

//usar para el switch case del modulo GAMEBOY
typedef enum{
	TEAM,
	BROKER,
	GAMECARD,
	SUSCRIPTOR,
	DEFAULT = -5
}input1;

typedef enum{
	CATCH_POKEMON,
	CAUGHT_POKEMON,
	GET_POKEMON,
	APPEARED_POKEMON,
	NEW_POKEMON,
}input2;

//devuelve el valor para el switch case (modulo GAMEBOY)
uint32_t valor_para_switch_case(char* input); //tirarle el primer input del modulo GAMEBOY.


//las proximas 2 funciones son para transformar de STRING a INT----------------

//eleva el 1er numero que recibe al 2do que recibe
uint32_t elevar_a(uint32_t a, uint32_t b);
//recibe un string y lo transforma en int (mandar solo numeros, no sean forros)
uint32_t cambia_a_int(char* enString);

//-----------------------------------------------------------------------------

//muestra por pantalla los bytes recibidos de RECV
void bytesRecibidos (int cantidad); //si, tiene que ser un int, o sino rompe, no cambiar

#endif /* SHARED_UTILIDADES_H_ */
