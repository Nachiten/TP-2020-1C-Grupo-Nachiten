/*
 * memoria.c
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#include "memoria.h"

//borrar?
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
	laLista = malloc(sizeof(lista_particiones));
	laLista->laParticion.estaLibre = 1;
	laLista->laParticion.limiteInferior = 0;
	laLista->numero_de_particion = 0;//cambiar?
	laLista->anter_particion = NULL;
	laLista->sig_particion = NULL;
}

lista_particiones* crear_particion(lista_particiones* laLista, uint32_t sizeDeLosDatos)
{
	if(laLista->numero_de_particion == 0)
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

void agregar_mensaje_a_Cache(void* CACHE, t_mensaje* estructuraMensaje, codigo_operacion tipoMensaje)
{

}

