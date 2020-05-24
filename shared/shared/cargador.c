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
int valor_para_switch_case(char* input)
{
	int switcher;
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
int elevar_a(int a, int b) //no deberia estar aca, pero paja de crear un archivo nuevo ahora solo para esta cosa
{
    int resultado = a;
    int iterador = b;

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

int cambia_a_int(char* enString)
{
    int enNumero = 0;
    int longitud = 4;//harcodeado por ahora, poner strlen
    int iterador = longitud-1;
    int elevado = 0;

    while(iterador >=0)
    {
        elevado = elevar_a(10, ((longitud - 1) - iterador));
        enNumero = enNumero + ((enString[iterador] - 48) * elevado);
        iterador--;
    }

    return enNumero;
}
