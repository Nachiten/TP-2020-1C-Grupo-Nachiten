/*
 * cargador.h
 *
 *  Created on: 18 abr. 2020
 *      Author: utnso
 */

#ifndef SHARED_CARGADOR_H_
#define SHARED_CARGADOR_H_

#include"commons/log.h"
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

//Usar para dejar cargadas las cosas que necesitamos.

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

t_log* cargarUnLog(char*, char*); //tirarle la ruta del log y el nombre del log
t_config* leerConfiguracion(char*); //tirarle la ruta del config
int valor_para_switch_case(char* input); //tirarle el primer input del modulo GAMEBOY.



#endif /* SHARED_CARGADOR_H_ */
