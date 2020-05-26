/*
 * cargador.c
 *
 *  Created on: 18 abr. 2020
 *      Author: utnso
 */

#include "cargador.h"

//Deja un log listo para usar.
t_log* cargarUnLog(char* ruta, char* nombreLog)
{
	return log_create(ruta, nombreLog, 1, LOG_LEVEL_TRACE);
}

//carga las configuraciones del archivo config que le pasamos.
t_config* leerConfiguracion(char* ruta)
{
	return config_create(ruta);
}

//devuelve el valor para el switch case (modulo GAMEBOY)
uint32_t valor_para_switch_case(char* input)
{
	uint32_t switcher;
	if (strcmp(input,"TEAM") == 0)
	{
		switcher = TEAM;
	}

	if (strcmp(input,"BROKER") == 0)
	{
		switcher = BROKER;
	}

	if (strcmp(input,"GAMECARD") == 0)
	{
		switcher = GAMECARD;
	}
	if (strcmp(input,"SUSCRIPTOR") == 0)//mentira, es un placeholder, vamos a tener que tomar otra cosa por consola, en vez de "SUSCRIPTOR"
	{
		switcher = SUSCRIPTOR;
	}
	return switcher;
}
//---------------------------------------------------------------------------------------------------------------------
uint32_t elevar_a(uint32_t a, uint32_t b) //no deberia (debatible) estar aca, pero paja de crear un archivo nuevo ahora solo para esta cosa
{
	uint32_t resultado = a;
	uint32_t iterador = b;

	if(b == 0)
	{
		resultado = 1;
		return resultado;
	}
	else
		{
		while(iterador > 1)
		{
			resultado = resultado*a;
			iterador--;
		}
	}
	return resultado;
}

uint32_t cambia_a_int(char* enString)
{
	uint32_t enNumero = 0;
	uint32_t longitud = strlen(enString);
	int iterador = longitud-1; //NO TOCAR EL PUTO INT O EXPLOTA TODO!! NO CAMBIAR A UINT32_T
	uint32_t elevado = 0;

	while(iterador >=0)
	{
		elevado = elevar_a(10, ((longitud - 1) - iterador));
		enNumero = enNumero + ((enString[iterador] - 48) * elevado);
		iterador--;
	}

	return enNumero;
}
