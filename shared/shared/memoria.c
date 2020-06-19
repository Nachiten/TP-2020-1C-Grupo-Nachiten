/*
 * memoria.c
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#include "memoria.h"

//borrar esta funcion? hace agua pero feo
void cargar_config_memoria(t_config* config, char* TAMANIO_MEM, char* TAMANIO_MIN_PART, char* ADMIN_MEMORIA, char* ALGOR_REEMPLAZO, char* ALGOR_ASIGN_PARTICION, char* FRECUEN_COMPACT)
{
	TAMANIO_MEM = config_get_string_value(config,"TAMANO_MEMORIA");
	TAMANIO_MIN_PART = config_get_string_value(config,"TAMANO_MINIMO_PARTICION");
	ADMIN_MEMORIA = config_get_string_value(config,"ALGORITMO_MEMORIA");
	ALGOR_REEMPLAZO = config_get_string_value(config,"ALGORITMO_REEMPLAZO");
	ALGOR_ASIGN_PARTICION = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
	FRECUEN_COMPACT = config_get_string_value(config,"FRECUENCIA_COMPACTACION");
}

void inicializar_lista_particiones(lista_particiones* laLista)
{
	laLista = (lista_particiones*) malloc(sizeof(lista_particiones)); //borrar el (lista_particiones*)??? probar
	laLista->laParticion.estaLibre = 1;
	laLista->laParticion.limiteInferior = 0;
	laLista->laParticion.limiteSuperior = 0;
	laLista->numero_de_particion = 0;
	laLista->anter_particion = NULL;
	laLista->sig_particion = NULL;
}

lista_particiones* crear_particion(lista_particiones* laLista, uint32_t sizeDeLosDatos)
{
	if((laLista->numero_de_particion == 0) && (laLista->laParticion.limiteSuperior == 0))
	{
		laLista->laParticion.estaLibre = 0;
		laLista->laParticion.limiteSuperior = sizeDeLosDatos;
		laLista->numero_de_particion++;
		return laLista;
	}
	else
	{
		lista_particiones* particionACrear = malloc(sizeof(lista_particiones));

		laLista->sig_particion = particionACrear;

		particionACrear->numero_de_particion = laLista->numero_de_particion++;
		particionACrear->anter_particion = laLista;
		particionACrear->sig_particion = NULL;
		particionACrear->laParticion.estaLibre = 0;
		particionACrear->laParticion.limiteInferior = laLista->laParticion.limiteSuperior;
		particionACrear->laParticion.limiteSuperior = sizeDeLosDatos;
		return particionACrear;
	}
}

void matar_lista_particiones(lista_particiones* laLista)
{
	lista_particiones* particionABorrar = laLista;
	lista_particiones* anteriorABorrar;

	//Me voy al final de la lista
	while(particionABorrar->sig_particion != NULL)
	{
		anteriorABorrar = particionABorrar;
		particionABorrar = particionABorrar->sig_particion;
	}

	//Una vez estoy en el final, hago un free y me paro en la posicion anterior
	while(particionABorrar->anter_particion != NULL)
	{
		printf("Borrando referencia a particion de memoria Nº %u \n", particionABorrar->numero_de_particion);
		free(particionABorrar);
		particionABorrar = anteriorABorrar;
	}

	//una vez llego al principio de la lista, libero la particion original
	printf("Borrando referencia a particion de memoria original, Nº %u\n", particionABorrar->numero_de_particion);
	free(particionABorrar);
}

lista_particiones* seleccionar_particion_First_Fit(uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size)
{
	lista_particiones* auxiliar = malloc(sizeof(lista_particiones*)); //esto es necesario? me parece que no
	auxiliar = laLista;
	lista_particiones* particionElegida = NULL;
	uint32_t encontreUnaParticionUtil = 0;

	//si estoy al principio de la lista, y no hay + particiones, la eleccion es facil...
	if((auxiliar->numero_de_particion == 0) && (auxiliar->sig_particion == NULL))
	{
		particionElegida = auxiliar;
	}

	//si no se puede hacer en la 1ra particion, hago todas las otras verificaciones
	else
	{
		//seguir recorriendo la lista hasta que encuentre una particion util, o llegue al final
		while(encontreUnaParticionUtil == 0 && (auxiliar->sig_particion != NULL))
		{
			//si la particion esta ocupada, busco otra
			if(auxiliar->laParticion.estaLibre == 0)
			{
				auxiliar = auxiliar->sig_particion;
			}
			else
			{
				//una vez encuentro una particion libre, tengo que ver si el espacio que tiene me sirve
				if((auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior) >= size)
				{
					encontreUnaParticionUtil = 1; //bingo
				}
				//esta libre, pero no me alcanza el espacio
				else
				{
					auxiliar = auxiliar->sig_particion;
				}
			}
		}

		//ver si sali porque encontre una particion, o si llegue al final
		if(encontreUnaParticionUtil == 0)
		{
			//estoy en la particion final, esta libre?
			if(auxiliar->laParticion.estaLibre == 1)
			{
				//su espacio me alcanza?
				if((tenemosEspacio(auxiliar,particionElegida,tamanioMemoria, size) == 0))//devuelve 1 si SI, 0 si NO
				{
					//ToDo ACA TENDRIA QUE ENTRAR COMPILACION? COMPLETAR CUANDO ESTE CLARO QUE HACER SI NO HAY MANERA DE METER LOS DATOS
				}
			}
			//no esta libre, hay que crear una particion nueva
			else
			{
				//el espacio que resta en la memoria no me alcanza?
				if((tenemosEspacio(auxiliar,particionElegida,tamanioMemoria, size) == 0))//devuelve 1 si SI, 0 si NO
				{
					//ToDo ACA TENDRIA QUE ENTRAR COMPILACION? COMPLETAR CUANDO ESTE CLARO QUE HACER SI NO HAY MANERA DE METER LOS DATOS
				}
				//el espacio que resta en memoria SI me alcanza, por lo que creo una particion nueva a continuacion de la particion en que estoy parado
				else
				{
					particionElegida = crear_particion(auxiliar, size);
				}
			}
		}

		//llegue hasta acá porque SI encontre una particion util Y NO es la última
		else
		{
			particionElegida = auxiliar;
		}
	}
	puts("Particion elegida exitosamente"); //borrar en el futuro?
	free(auxiliar);//atencion con esto
	return particionElegida;
}

uint32_t tenemosEspacio(lista_particiones* auxiliar, lista_particiones* particionElegida, uint32_t tamanioMemoria, uint32_t size)
{
	uint32_t resultado;
	//si todavia hay espacio para meter algo...
	if(auxiliar->laParticion.limiteSuperior <= tamanioMemoria)
	{
		//...veo si ese espacio me sirve
		if((auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior) >= size)
		{
			particionElegida = &auxiliar; //tenemos un ganador
			resultado = 1;
		}
		//el espacio no me sirve y la memoria esta totalmente copada
		else
		{
			puts("Desesperaos mortales, el fin se acerca.");
			resultado = 0;
		}
	}
	return resultado;
}

void poner_en_particion(void* CACHE, lista_particiones* particionElegida, void* estructura, codigo_operacion tipoMensaje)
{
	switch(tipoMensaje)
	{
			case NEW:
				//estructura = malloc (sizeof(New));
				poner_NEW_en_particion(CACHE, particionElegida, estructura);
				free(estructura);
				break;

			case APPEARED:
				//estructura = malloc (sizeof(Appeared));
				poner_APPEARED_en_particion(CACHE, particionElegida, estructura);
				free(estructura);
				break;

			case GET:
				//estructura = malloc (sizeof(Get));
				poner_GET_en_particion(CACHE, particionElegida, estructura);
				free(estructura);
				break;

			case LOCALIZED://esto no lo puedo hacer todavia porque la estructura esta INCOMPLETA
				/*estructura = malloc (sizeof(Localized));
				poner_LOCALIZED_en_particion(CACHE, particionElegida, estructura);
				free(estructura);
				*/
				break;

			case CATCH:
				//estructura = malloc (sizeof(Catch));
				poner_CATCH_en_particion(CACHE, particionElegida, estructura);
				free(estructura);
				break;

			case CAUGHT:
				//estructura = malloc (sizeof(Caught));
				poner_CAUGHT_en_particion(CACHE, particionElegida, estructura);
				free(estructura);
				break;

			case TEST://que se joda test
				break;
	}

}

void poner_NEW_en_particion(void* CACHE, lista_particiones* particionElegida, New* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon));
	desplazamiento += sizeof(estructura->nombrePokemon);

	//meto coordenada X del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);

	//meto coordenada Y del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);

	//meto cantidad del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->cantPokemon), sizeof(estructura->cantPokemon));
	desplazamiento += sizeof(estructura->cantPokemon);

	//meto ID del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->ID), sizeof(estructura->ID));
	desplazamiento += sizeof(estructura->ID);

	//meto ID Correlativa del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->corrID), sizeof(estructura->corrID));
	desplazamiento += sizeof(estructura->corrID);

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_APPEARED_en_particion(void* CACHE, lista_particiones* particionElegida, Appeared* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon));
	desplazamiento += sizeof(estructura->nombrePokemon);

	//meto coordenada X del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);

	//meto coordenada Y del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);

	//meto ID del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->ID), sizeof(estructura->ID));
	desplazamiento += sizeof(estructura->ID);

	//meto ID Correlativa del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->corrID), sizeof(estructura->corrID));
	desplazamiento += sizeof(estructura->corrID);

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_GET_en_particion(void* CACHE, lista_particiones* particionElegida, Get* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon));
	desplazamiento += sizeof(estructura->nombrePokemon);

	//meto ID del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->ID), sizeof(estructura->ID));
	desplazamiento += sizeof(estructura->ID);

	//meto ID Correlativa del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->corrID), sizeof(estructura->corrID));
	desplazamiento += sizeof(estructura->corrID);

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_LOCALIZED_en_particion(void* CACHE, lista_particiones* particionElegida, Localized* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon));
	desplazamiento += sizeof(estructura->nombrePokemon);

	//meto la cantidad de lugares donde se encuentra el pokemon, en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->cantPosciciones), sizeof(estructura->cantPosciciones));
	desplazamiento += sizeof(estructura->cantPosciciones);

	//ESTRUCTURA INCOMPLETA <------------------------------------------------------------------------------------------------------------------------

	//meto ID del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->ID), sizeof(estructura->ID));
	desplazamiento += sizeof(estructura->ID);

	//meto ID Correlativa del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->corrID), sizeof(estructura->corrID));
	desplazamiento += sizeof(estructura->corrID);

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_CATCH_en_particion(void* CACHE, lista_particiones* particionElegida, Catch* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon));
	desplazamiento += sizeof(estructura->nombrePokemon);

	//meto coordenada X del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);

	//meto coordenada Y del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);

	//meto ID del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->ID), sizeof(estructura->ID));
	desplazamiento += sizeof(estructura->ID);

	//meto ID Correlativa del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->corrID), sizeof(estructura->corrID));
	desplazamiento += sizeof(estructura->corrID);

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_CAUGHT_en_particion(void* CACHE, lista_particiones* particionElegida, Caught* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon));
	desplazamiento += sizeof(estructura->nombrePokemon);

	//meto el resultado del intento de CATCH en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->pudoAtrapar), sizeof(estructura->pudoAtrapar));
	desplazamiento += sizeof(estructura->pudoAtrapar);

	//meto ID del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->ID), sizeof(estructura->ID));
	desplazamiento += sizeof(estructura->ID);

	//meto ID Correlativa del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->corrID), sizeof(estructura->corrID));
	desplazamiento += sizeof(estructura->corrID);

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void verificacionPosicion(uint32_t limiteSuperiorDeParticion, uint32_t posicionSuperiorUltimoDato)
{
	if(limiteSuperiorDeParticion == posicionSuperiorUltimoDato)
	{
		puts("Información agregada correctamente en la partición.\n");
	}
	else
	{
		if(posicionSuperiorUltimoDato < limiteSuperiorDeParticion)
		{
			puts("Información agregada con fragmentación interna.\n");
		}
		else
		{
			puts("ATENCION! LA INFORMACIÓN SE SALIÓ DE LA PARTICION!!.\n");
		}
	}
}

void agregar_mensaje_a_Cache(void* CACHE, uint32_t tamanioMemoria, lista_particiones* laLista, char* algoritmoAsignacion, void* estructuraMensaje, codigo_operacion tipoMensaje)
{
	lista_particiones* particionElegida;
	uint32_t sizeDelMensaje = sizeof(estructuraMensaje);

	//para poder meter el mensaje en Cache, primero hay que buscarle una particion
	if(strcmp(algoritmoAsignacion,"FF") == 0)
	{
		particionElegida = seleccionar_particion_First_Fit(tamanioMemoria, laLista, sizeDelMensaje);
	}
	else
	{
		if(strcmp(algoritmoAsignacion,"BF") == 0)
		{
			//ToDo
			//particionElegida = seleccionar_particion_Best_Fit();
			puts("Falta implementar");
		}
		else
		{
			puts("Alguien escribió mal el campo de ALGORITMO_PARTICION_LIBRE del archivo config.\n");
		}
	}

	//ahora que tenemos la particion, metemos los datos
	poner_en_particion(CACHE, particionElegida, estructuraMensaje, tipoMensaje);

//PASO 3: profit???

}

