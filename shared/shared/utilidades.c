/*
 * utilidades.c
 *
 *  Created on: 28 may. 2020
 *      Author: utnso
 */

#include "utilidades.h"

//devuelve el valor para el switch case (modulo GAMEBOY)
uint32_t valor_para_switch_case(char* input) //tirarle el primer input del modulo GAMEBOY.
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

uint32_t elevar_a(uint32_t a, uint32_t b)
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

void bytesRecibidos (int32_t cantidad)
{
	printf("Recibí %d bytes sin crashear (Todavía).\n",cantidad);
}

void bytesEnviados (int32_t cantidad)
{
	printf("Envié %d bytes sin crashear (Todavía).\n",cantidad);
}

void libero_estructura_appeared(Appeared* estructura)
{
	free(estructura->nombrePokemon);
	free(estructura);
}

void libero_estructura_New(New* estructura)
{
	free(estructura->nombrePokemon);
	free(estructura);
}

void libero_estructura_Catch(Catch* estructura)
{
	free(estructura->nombrePokemon);
	free(estructura);
}

void libero_estructura_Caught(Caught* estructura)
{
	free(estructura->nombrePokemon);
	free(estructura);
}

void libero_estructura_Get(Get* estructura)
{
	free(estructura->nombrePokemon);
	free(estructura);
}

void libero_estructura_Suscripcion(Suscripcion* estructura)
{
	free(estructura);
}

void libero_estructura_Dessuscripcion(Dessuscripcion* estructura)
{
	free(estructura);
}


/*este codigo es para que reciba cosas por consola, al final no se implemento
	char* entrada1 = malloc(sizeof(char*)); //aca se guarda la primera orden que llega al gameboy
	char* entrada2 = malloc(sizeof(char*));

//leo lo que llegue por consola/terminal.
	scanf("%s %s", entrada1, entrada2);

	free(entrada1);
	free(entrada2);
*/
