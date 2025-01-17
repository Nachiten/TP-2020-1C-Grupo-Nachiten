/*
 * memoria.c
 *
 *  Created on: 12 jun. 2020
 *      Author: utnso
 */

#include "memoria.h"

void inicializar_lista_particiones(lista_particiones* laLista, char* algorAdminMemoria, uint32_t tamanioMemoria)
{
	if(strcmp(algorAdminMemoria, "PD") == 0)
	{
		laLista->laParticion.estaLibre = 1;
		laLista->laParticion.limiteInferior = 0;
		laLista->laParticion.limiteSuperior = 0;
		laLista->numero_de_particion = 0;
		laLista->anter_particion = NULL;
		laLista->sig_particion = NULL;
		laLista->numero_de_victima = 0;
		laLista->cola = ERROR;
		laLista->ID_MENSAJE_GUARDADO = -1;
	}
	else
	{
		if(strcmp(algorAdminMemoria, "BS") == 0)
		{
			laLista->laParticion.estaLibre = 1;
			laLista->laParticion.limiteInferior = 0;
			laLista->laParticion.limiteSuperior = tamanioMemoria;
			laLista->numero_de_particion = 0;
			laLista->anter_particion = NULL;
			laLista->sig_particion = NULL;
			laLista->numero_de_victima = 0;
			laLista->cola = ERROR;
			laLista->ID_MENSAJE_GUARDADO = -1;
		}
		else
		{
			puts("Alguien escribió mal el campo de ALGORITMO_MEMORIA del archivo config, asi que Hasta La vista, Baby!");
			abort();
		}
	}
	puts("Lista de particiones inicializada.\n");
}

lista_particiones* crear_particion(lista_particiones* laLista, uint32_t sizeDeLosDatos, char* algorAdminMemoria)
{
	//si uso particiones dinamicas, inicializo asi...
	if(strcmp(algorAdminMemoria, "PD") == 0)
	{
		//estoy creando la primera particion
		if((laLista->numero_de_particion == 0) && (laLista->laParticion.limiteSuperior == 0))
		{
			laLista->laParticion.estaLibre = 0;
			laLista->laParticion.limiteSuperior = sizeDeLosDatos;
			return laLista;
		}
		//ya hay particiones creadas
		else
		{
			lista_particiones* particionACrear = malloc(sizeof(lista_particiones));

			if(laLista->sig_particion == NULL)//estoy en la ultima particion
			{
				if(((laLista->laParticion.limiteSuperior - laLista->laParticion.limiteInferior) == sizeDeLosDatos) && laLista->laParticion.estaLibre == 1)//encaja perfectamente
				{
					laLista->laParticion.estaLibre = 0;
					laLista->numero_de_victima = 0;
					laLista->ID_MENSAJE_GUARDADO = -1;
					free(particionACrear);
				}

				else//no entran perfectamente en la ultima particion
				{	//sobra espacio?
					if(((laLista->laParticion.limiteSuperior - laLista->laParticion.limiteInferior) > sizeDeLosDatos) && laLista->laParticion.estaLibre == 1)//divido la ultima particion porque tiene espacio de mas
					{
						int guardarLimSup = laLista->laParticion.limiteSuperior;

						laLista->laParticion.estaLibre = 0;
						laLista->laParticion.limiteSuperior = (laLista->laParticion.limiteInferior + sizeDeLosDatos);
						laLista->numero_de_victima = 0;
						laLista->ID_MENSAJE_GUARDADO = -1;

						particionACrear->sig_particion = laLista->sig_particion;
						particionACrear->anter_particion = laLista;
						laLista->sig_particion = particionACrear;

						particionACrear->laParticion.estaLibre = 1;
						particionACrear->laParticion.limiteInferior = laLista->laParticion.limiteSuperior;
						if(particionACrear->sig_particion != NULL)
						{
							particionACrear->laParticion.limiteSuperior = particionACrear->sig_particion->laParticion.limiteInferior;
						}
						else
						{
							particionACrear->laParticion.limiteSuperior = guardarLimSup;
						}

						particionACrear->numero_de_victima = 0;
						particionACrear->ID_MENSAJE_GUARDADO = -1;
						particionACrear->numero_de_particion = (laLista->numero_de_particion + 1);

						corregirNumerosYParticiones(particionACrear, particionACrear->numero_de_particion);
					}

					else//creo una particion al final de la lista porque en la ultima particion no entraba
					{
						laLista->sig_particion = particionACrear;
						particionACrear->numero_de_particion = (laLista->numero_de_particion +1);
						particionACrear->anter_particion = laLista;
						particionACrear->sig_particion = NULL;
						particionACrear->laParticion.estaLibre = 0;
						particionACrear->laParticion.limiteInferior = laLista->laParticion.limiteSuperior;
						particionACrear->laParticion.limiteSuperior = (particionACrear->laParticion.limiteInferior + sizeDeLosDatos);
						particionACrear->numero_de_victima = 0;
						particionACrear->ID_MENSAJE_GUARDADO = -1;
					}
				}
			}

			else//creo en una particion que tiene otras particiones despues todo VOLVER ACAAA
			{
				laLista->laParticion.estaLibre = 0;
				laLista->laParticion.limiteSuperior = (laLista->laParticion.limiteInferior + sizeDeLosDatos);
				laLista->numero_de_victima = 0;
				laLista->ID_MENSAJE_GUARDADO = -1;

				particionACrear->sig_particion = laLista->sig_particion;
				particionACrear->anter_particion = laLista;
				laLista->sig_particion = particionACrear;

				particionACrear->laParticion.estaLibre = 1;
				particionACrear->laParticion.limiteInferior = laLista->laParticion.limiteSuperior;
				particionACrear->laParticion.limiteSuperior = particionACrear->sig_particion->laParticion.limiteInferior;
				particionACrear->numero_de_victima = 0;
				particionACrear->ID_MENSAJE_GUARDADO = -1;
				particionACrear->numero_de_particion = (laLista->numero_de_particion + 1);

				corregirNumerosYParticiones(particionACrear, particionACrear->numero_de_particion);
			}

			return particionACrear;
		}
	}
	//si uso BS, inicializo asa...
	else
	{
		uint32_t mitad = 0;
		uint32_t numeroParticionOriginal = laLista->numero_de_particion;

		while(((laLista->laParticion.limiteSuperior - laLista->laParticion.limiteInferior)/2) >= sizeDeLosDatos)
		{
			mitad = laLista->laParticion.limiteInferior + ((laLista->laParticion.limiteSuperior - laLista->laParticion.limiteInferior)/2);

			lista_particiones* particionACrear = malloc(sizeof(lista_particiones));
			particionACrear->sig_particion = laLista->sig_particion;
			laLista->sig_particion = particionACrear;

			particionACrear->laParticion.limiteSuperior = laLista->laParticion.limiteSuperior;
			laLista->laParticion.limiteSuperior = mitad;

			particionACrear->anter_particion = laLista;
			particionACrear->laParticion.estaLibre = 1;
			particionACrear->laParticion.limiteInferior = mitad;
			particionACrear->numero_de_victima = 0;
			particionACrear->ID_MENSAJE_GUARDADO = -1;

//			if(laLista->anter_particion != NULL)
//			{
//				laLista = laLista->anter_particion;
//			}

			//printf("limite sup: %u\n",laLista->laParticion.limiteSuperior);
		}
		corregirNumerosYParticiones(laLista, numeroParticionOriginal);
		return laLista;
	}
}

void corregirNumerosYParticiones(lista_particiones* particionOriginal, uint32_t numeroParticionOriginal)
{
	lista_particiones* particionActual = particionOriginal;

	while(particionOriginal->sig_particion != NULL)
	{
		particionOriginal = particionOriginal->sig_particion;
		particionOriginal->anter_particion = particionActual;
		particionActual = particionOriginal;
		numeroParticionOriginal++;
		particionOriginal->numero_de_particion = numeroParticionOriginal;
	}
}

void seleccionDeVictima(void* CACHE, lista_particiones* laLista, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS, char* ADMIN_MEMORIA, t_log* logger, sem_t* semLog, sem_t* semParticiones, sem_t* semParticionesEliminadas)
{
	lista_particiones* particionABorrar = laLista;
	uint32_t masViejoUsado = 99999;
	uint32_t victima = particionABorrar->numero_de_particion;

	//recorremos la lista hasta el final buscando una victima
	while(particionABorrar != NULL)
	{
		//cuanto mas viejo fue su uso, mejor opcion como victima
		if((particionABorrar->numero_de_victima < masViejoUsado) && particionABorrar->laParticion.estaLibre != 1)
		{
			victima = particionABorrar->numero_de_particion;
			masViejoUsado = particionABorrar->numero_de_victima;
		}
		particionABorrar = particionABorrar->sig_particion;
	}
	particionABorrar = laLista;
	//para este punto ya tenemos nuestra victima, solo hay que seleccionarla y mandarla a borrar
	while(particionABorrar->numero_de_particion != victima)
	{
		particionABorrar = particionABorrar->sig_particion;
	}

	//mandamos la victima al matadero
	borrarReferenciaAParticion(laLista, particionABorrar, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas, 0);

	//si frecuencia de compactacion es -1, 0 o 1 se compacta siempre
	//sino, solo cuando la cantidad de particiones eliminadas sea igual a la frecuencia que piden
	//Y SI Y SOLO SI NO SE ESTA USANDO Bullshit System
	if(((FRECUEN_COMPACT == -1) || (FRECUEN_COMPACT == 0) || (FRECUEN_COMPACT == 1) || (FRECUEN_COMPACT == *PARTICIONES_ELIMINADAS)) && (strcmp(ADMIN_MEMORIA,"BS") != 0))
	{
		sem_wait(semLog);
		log_info(logger, "Compactando...");
		sem_post(semLog);
		compactacion(CACHE, laLista, logger, semLog, semParticiones, semParticionesEliminadas);
		sem_wait(semParticionesEliminadas);
		*PARTICIONES_ELIMINADAS = 0;
		sem_post(semParticionesEliminadas);
	}
}

void borrarReferenciaAParticion(lista_particiones* laLista, lista_particiones* particionABorrar, uint32_t* PARTICIONES_ELIMINADAS, char* ADMIN_MEMORIA, t_log* logger, sem_t* semLog, sem_t* semParticiones, sem_t* semParticionesEliminadas, uint32_t flagCompactacion)
{
	particionABorrar->laParticion.estaLibre = 1;
	sem_wait(semLog);
	log_info(logger, "La particion %u que iniciaba en %u ahora está libre!", particionABorrar->numero_de_particion, particionABorrar->laParticion.limiteInferior);
	sem_post(semLog);
	puts("\n");
	if(flagCompactacion == 0)
	{
		sem_wait(semParticionesEliminadas);
		*PARTICIONES_ELIMINADAS = *PARTICIONES_ELIMINADAS +1;
		sem_post(semParticionesEliminadas);
	}
	sem_wait(semParticiones);
	consolidarParticion(laLista, particionABorrar, ADMIN_MEMORIA, logger, semLog);
	sem_post(semParticiones);
}

void consolidarParticion(lista_particiones* laLista, lista_particiones* particionABorrar, char* ADMIN_MEMORIA, t_log* logger, sem_t* semLog)
{
	uint32_t numPart = particionABorrar->numero_de_particion;
	uint32_t inicioPart = particionABorrar->laParticion.limiteInferior;
	uint32_t numPartAnt = 0;
	uint32_t numPartSig = 0;
	uint32_t inicioPartSig = 0;
	uint32_t resultado = 0;
	uint32_t consolidado = 0;
	lista_particiones* correctorDeNumeros = particionABorrar->anter_particion;
	lista_particiones* auxiliame;
	uint32_t numeroACorregir = 0;

	if(correctorDeNumeros != NULL)
	{
		numeroACorregir = particionABorrar->anter_particion->numero_de_particion;
	}

	if(strcmp(ADMIN_MEMORIA,"BS") != 0)
	{
		if(particionABorrar->anter_particion != NULL)
		{
			numPartAnt = particionABorrar->anter_particion->numero_de_particion;

			//si la particion anterior a la que borro esta libre, las consolido
			if(particionABorrar->anter_particion->laParticion.estaLibre == 1)
			{
				//expando el tamaño de la particion anterior
				particionABorrar->anter_particion->laParticion.limiteSuperior = particionABorrar->laParticion.limiteSuperior;
				resultado = particionABorrar->anter_particion->numero_de_particion;
				particionABorrar->anter_particion->sig_particion = particionABorrar->sig_particion;

				//si la particion que estoy borrando no apunta a NULL, entonces hago que la siguiente apunte a su anterior
				if(particionABorrar->sig_particion != NULL)
				{
					particionABorrar->sig_particion->anter_particion = particionABorrar->anter_particion;
				}

				consolidado = 1;
				printf("La particion %u fue consolidada con la particion %u y ahora se llaman partición %u.\n\n", numPart, numPartAnt, resultado);
			}
		}

		if(particionABorrar->sig_particion != NULL)
		{
			numPartSig = particionABorrar->sig_particion->numero_de_particion;

			//si la particion siguiente a la que quiero borrar esta libre, las consolido (Y BORRO LA SIGUIENTE)
			if(particionABorrar->sig_particion->laParticion.estaLibre == 1)
			{
				resultado = particionABorrar->numero_de_particion;

				auxiliame = particionABorrar;

				//avanzo particionABorrar a la siguiente (porque es la que voy a borrar en realidad)
				particionABorrar = particionABorrar->sig_particion;

				//expando el tamaño de la particion incluyendo el espacio de la siguiente
				auxiliame->laParticion.limiteSuperior = particionABorrar->laParticion.limiteSuperior;

				//si la particion siguiente (LA QUE VOY A BORRAR) no apunta a NULL, entonces hago que SU siguiente apunte a esta particion
				if(particionABorrar->sig_particion != NULL)
				{
					particionABorrar->sig_particion->anter_particion = auxiliame;
				}

				//apunto a la siguiente de mi siguiente
				auxiliame->sig_particion = particionABorrar->sig_particion;

				consolidado = 1;
				printf("La particion %u fue consolidada con la particion %u y ahora se llaman partición %u.\n\n", numPart, numPartSig, resultado);
			}
		}

		if(consolidado == 1)
		{
			while(correctorDeNumeros != NULL)
			{
				correctorDeNumeros->numero_de_particion = numeroACorregir;
				numeroACorregir++;
				correctorDeNumeros = correctorDeNumeros->sig_particion;
			}
			free(particionABorrar);
		}
	}

	if(strcmp(ADMIN_MEMORIA,"BS") == 0)
	{
		if(particionABorrar->sig_particion != NULL)
		{
			numPartSig = particionABorrar->sig_particion->numero_de_particion;
			inicioPartSig = particionABorrar->sig_particion->laParticion.limiteInferior;

			//si la particion siguiente a la que quiero borrar esta libre, y tienen el mismo tamaño las consolido (Y BORRO LA SIGUIENTE)
			if((particionABorrar->sig_particion->laParticion.estaLibre == 1 ) && ((particionABorrar->laParticion.limiteSuperior - particionABorrar->laParticion.limiteInferior) == (particionABorrar->sig_particion->laParticion.limiteSuperior - particionABorrar->sig_particion->laParticion.limiteInferior)))
			{
				resultado = particionABorrar->numero_de_particion;
				correctorDeNumeros = particionABorrar;
				auxiliame = particionABorrar;

				//avanzo particionABorrar a la siguiente (porque es la que voy a borrar en realidad)
				particionABorrar = particionABorrar->sig_particion;

				//expando el tamaño de la particion incluyendo el espacio de la siguiente
				auxiliame->laParticion.limiteSuperior = particionABorrar->laParticion.limiteSuperior;

				//si la particion siguiente (LA QUE VOY A BORRAR) no apunta a NULL, entonces hago que SU siguiente apunte a esta particion
				if(particionABorrar->sig_particion != NULL)
				{
					particionABorrar->sig_particion->anter_particion = auxiliame;
				}

				//apunto a la siguiente de mi siguiente
				auxiliame->sig_particion = particionABorrar->sig_particion;

				consolidado = 1;
				sem_wait(semLog);
				log_info(logger, "La particion %u (Inicio de particion: %u) fue consolidada con la particion %u (Inicio de particion: %u) y ahora se llaman partición %u.\n",numPart, inicioPart, numPartSig, inicioPartSig, resultado);
				sem_post(semLog);
				puts("\n");
			}

			if(consolidado == 1)
			{
				while(correctorDeNumeros != NULL)
				{
					correctorDeNumeros->numero_de_particion = numeroACorregir;
					numeroACorregir++;
					correctorDeNumeros = correctorDeNumeros->sig_particion;
				}
				correctorDeNumeros = NULL;
				free(particionABorrar);
			}

			//ahora hago lo mismo desde el principio en caso de haya que consolidar mas particiones
			particionABorrar = laLista;
			while(particionABorrar->sig_particion != NULL)
			{
				numPart = particionABorrar->numero_de_particion;
				inicioPart = particionABorrar->laParticion.limiteInferior;
				numPartSig = particionABorrar->sig_particion->numero_de_particion;
				inicioPartSig = particionABorrar->sig_particion->laParticion.limiteInferior;
				numeroACorregir = 0;
				consolidado = 0;

				//si la particion siguiente a la que quiero borrar esta libre, y tienen el mismo tamaño las consolido (Y BORRO LA SIGUIENTE)
				if((particionABorrar->sig_particion->laParticion.estaLibre == 1 ) && ((particionABorrar->laParticion.limiteSuperior - particionABorrar->laParticion.limiteInferior) == (particionABorrar->sig_particion->laParticion.limiteSuperior - particionABorrar->sig_particion->laParticion.limiteInferior)))
				{
					resultado = particionABorrar->numero_de_particion;
					correctorDeNumeros = particionABorrar;
					auxiliame = particionABorrar;

					//avanzo particionABorrar a la siguiente (porque es la que voy a borrar en realidad)
					particionABorrar = particionABorrar->sig_particion;

					//expando el tamaño de la particion incluyendo el espacio de la siguiente
					auxiliame->laParticion.limiteSuperior = particionABorrar->laParticion.limiteSuperior;

					//si la particion siguiente (LA QUE VOY A BORRAR) no apunta a NULL, entonces hago que SU siguiente apunte a esta particion
					if(particionABorrar->sig_particion != NULL)
					{
						particionABorrar->sig_particion->anter_particion = auxiliame;
					}

					//apunto a la siguiente de mi siguiente
					auxiliame->sig_particion = particionABorrar->sig_particion;

					consolidado = 1;
					sem_wait(semLog);
					log_info(logger, "La particion %u (Inicio de particion: %u) fue consolidada con la particion %u (Inicio de particion: %u) y ahora se llaman partición %u.\n",numPart, inicioPart, numPartSig, inicioPartSig, resultado);
					sem_post(semLog);
					puts("\n");

					printf("La particion %u fue consolidada con la particion %u y ahora se llaman partición %u.\n\n", numPart, numPartSig, resultado);

					if(consolidado == 1)
					{
						while(correctorDeNumeros != NULL)
						{
							correctorDeNumeros->numero_de_particion = numeroACorregir;
							numeroACorregir++;
							correctorDeNumeros = correctorDeNumeros->sig_particion;
						}
						correctorDeNumeros = NULL;
						free(particionABorrar);
					}
					particionABorrar = laLista;
				}
				else
				{
					particionABorrar = particionABorrar->sig_particion;
				}
			}
		}
	}
}

void compactacion(void* CACHE, lista_particiones* laLista, t_log* logger, sem_t* semLog, sem_t* semParticiones, sem_t* semParticionesEliminadas)
{
	uint32_t variableDeAdorno = 0;
	lista_particiones* particionLibre = laLista;
	lista_particiones* particionOcupada = laLista;
	lista_particiones* auxilio = laLista;
	uint32_t posicionAEscribir = 0;
	uint32_t tamanioACopiar = 0;
	uint32_t particionMovida = 0;

	//avanzo hasta la primera particion libre, si llego al final, se acabo
	while(particionLibre != NULL)
	{
		//si la particion no esta libre, no me interesa
		if(particionLibre->laParticion.estaLibre == 1)
		{
			posicionAEscribir = particionLibre->laParticion.limiteInferior;
			//me paro en la particion libre porque las particiones ocupadas anteriores a esa no me importan
			particionOcupada = particionLibre;
			//avanzo hasta la primera particion ocupada
			while(particionOcupada != NULL && particionMovida == 0)
			{
				//si la particion no esta ocupada, no me interesa
				 if(particionOcupada->laParticion.estaLibre == 0)
				 {
					 tamanioACopiar = particionOcupada->laParticion.limiteSuperior - particionOcupada->laParticion.limiteInferior;

					 memcpy(CACHE + posicionAEscribir, (CACHE + particionOcupada->laParticion.limiteInferior),tamanioACopiar);

					 //ahora la particion que estaba libre tiene los datos de la particion que movi
					 particionLibre->laParticion.estaLibre = 0;
					 particionLibre->laParticion.limiteSuperior = particionLibre->laParticion.limiteInferior + tamanioACopiar;
					 particionLibre->ID_MENSAJE_GUARDADO = particionOcupada->ID_MENSAJE_GUARDADO;

					 //la particion que movi, ya no tiene los datos de ningun mensaje
					 particionOcupada->ID_MENSAJE_GUARDADO = -1;

					 auxilio = particionOcupada->anter_particion;
					 borrarReferenciaAParticion(laLista, particionOcupada, &variableDeAdorno, "PD", logger, semLog, semParticiones, semParticionesEliminadas, 1);
					 particionOcupada = auxilio;
					 particionOcupada->sig_particion->laParticion.limiteInferior = particionOcupada->laParticion.limiteSuperior;
					 particionMovida = 1;
				 }
				 if(particionMovida == 0)
				 {
					particionOcupada = particionOcupada->sig_particion;
				 }
			}
			particionOcupada = particionLibre;
			particionMovida = 0;
		}
		particionLibre = particionLibre->sig_particion;
	}

	//y ahora me deshago de todas las particiones libres que hayan quedado al final de la lista
	particionLibre = laLista;	//me paro de nuevo al principio
	auxilio = laLista;
	while(particionLibre != NULL)//recorro la lista
	{
		//si la particion no esta libre, no me interesa
		if(particionLibre->laParticion.estaLibre == 1)
		{
			if(particionLibre->numero_de_particion == 0)//caso borde
			{
				particionLibre->laParticion.limiteInferior = 0;
				particionLibre->laParticion.limiteSuperior = 1;
			}
			else//caso normal, me deshago de la particion
			{
				particionLibre->anter_particion->sig_particion = NULL;
				auxilio = particionLibre->sig_particion;
				free(particionLibre);
				particionLibre = auxilio;
			}
		}
		if(particionLibre != NULL)
		{
			particionLibre = particionLibre->sig_particion;
		}
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
		particionABorrar->sig_particion = NULL;
		if(anteriorABorrar->anter_particion != NULL)
		{
			anteriorABorrar = anteriorABorrar->anter_particion;
		}
	}

	//una vez llego al principio de la lista, libero la particion original
	printf("Borrando referencia a particion de memoria original, Nº %u\n", particionABorrar->numero_de_particion);
	particionABorrar->laParticion.estaLibre = 1;
	particionABorrar->numero_de_victima = 0;
	particionABorrar->ID_MENSAJE_GUARDADO = 0;
	free(particionABorrar);
}

void matar_lista_particiones_candidatas(particionesCandidatas* listaDeCandidatas)
{
	particionesCandidatas* aBorrar = NULL;
	particionesCandidatas* seleccionAnterior = NULL;

	//si tengo mas de una particion candidata
	while(listaDeCandidatas->sig_candidata != NULL)
	{
		aBorrar = listaDeCandidatas->sig_candidata;//selecciono la 2da candidata
		seleccionAnterior = listaDeCandidatas; //esta solo está para que no crashee
		while(aBorrar->sig_candidata != NULL)//siempre que haya mas candidatas
		{
			seleccionAnterior = aBorrar;
			aBorrar = aBorrar->sig_candidata;//avanzo a la siguiente candidata
		}
		free(aBorrar);//borro la ultima candidata
		seleccionAnterior->sig_candidata = NULL;
	}
	free(listaDeCandidatas); //borro la primera candidata
}

void revision_lista_particiones(void* CACHE, lista_particiones* laLista, uint32_t tamanioMemoria, t_log* dumpCache)
{
	lista_particiones* auxiliar = laLista;
	uint32_t espacioLibre = 0;
	uint32_t espacioOcupado = 0;
	uint32_t control = 1;
	time_t fecha;
	time(&fecha);

	log_info(dumpCache, "Realizando control del estado de memoria y particiones...");
	log_info(dumpCache, "Dump solicitado: %s",ctime(&fecha));
	log_info(dumpCache, "----------------------------------------------------------");

	//recorro las particiones hasta la ultima
	while(control == 1)
	{
		log_info(dumpCache, "Nº de particion: %u.", auxiliar->numero_de_particion);
		if(auxiliar->laParticion.estaLibre == 1)
		{
			log_info(dumpCache, "La partición está libre.");
			espacioLibre += (auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior); //si hay una particion libre, suma el espacio "desperdiciado"
		}
		else
		{
			log_info(dumpCache, "La particion está en uso.");
			espacioOcupado += (auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior); //si hay una particion ocupada, suma el espacio en uso
		}
		log_info(dumpCache, "Límite inferior de la partición: %u.", auxiliar->laParticion.limiteInferior);
		log_info(dumpCache, "Límite superior de la partición: %u.", auxiliar->laParticion.limiteSuperior);
		//log_info(dumpCache, "%s", mem_hexstring(CACHE, auxiliar->laParticion.limiteSuperior));
		log_info(dumpCache, "Posiciones de memoria: de %p hasta %p.", (CACHE + auxiliar->laParticion.limiteInferior), (CACHE + auxiliar->laParticion.limiteSuperior));
		log_info(dumpCache, "Tamaño: %u bytes.", auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior);
		if(auxiliar->laParticion.estaLibre == 0)
		{
			log_info(dumpCache, "Número de víctima: %u.", auxiliar->numero_de_victima);
			log_info(dumpCache, "Cola a la que pertenece el mensaje: %i.", auxiliar->cola);
		}
		if(auxiliar->ID_MENSAJE_GUARDADO != -1)
		{
			log_info(dumpCache, "En esta partición se guardan los datos del mensaje ID: %i.", auxiliar->ID_MENSAJE_GUARDADO);
		}
		log_info(dumpCache, "----------------------------------------------------------");

		//avanzo
		if(auxiliar->sig_particion != NULL)
		{
			auxiliar = auxiliar->sig_particion;
		}
		else
		{
			control = 0;
		}
	}
	log_info(dumpCache, "Espacio desperdiciado en particiones libres: %u.", espacioLibre);
	log_info(dumpCache, "Espacio libre en CACHE (No particionado): %u.", (tamanioMemoria - espacioOcupado - espacioLibre));
	log_info(dumpCache, "Espacio total libre: %u.", (tamanioMemoria - espacioOcupado));

	log_info(dumpCache, "---No se tuvo en cuenta la fragmentación interna durante el checkeo---\n");
}

lista_particiones* seleccionar_particion_First_Fit(void* CACHE, uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS, char* ADMIN_MEMORIA, t_log* logger, sem_t* semLog, sem_t* semParticiones, sem_t* semParticionesEliminadas)
{
	lista_particiones* auxiliar = laLista;
	lista_particiones* particionElegida = NULL;
	uint32_t encontreUnaParticionUtil = 0;

	//si estoy al principio de la lista, y no hay + particiones, la eleccion es facil...
	if((auxiliar->numero_de_particion == 0) && (auxiliar->laParticion.estaLibre == 1) && (auxiliar->sig_particion == NULL))
	{
		particionElegida = auxiliar;
		//por fin encontre el fucking UNICO caso en el que la primera particion es la unica y con un tamaño distinto de 0
		if((particionElegida->laParticion.limiteSuperior - particionElegida->laParticion.limiteInferior) < size)
		{
			sem_wait(semParticiones);
			crear_particion(particionElegida, size, "PD");
			sem_post(semParticiones);
		}
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

		//ver si sali del while porque encontre una particion, o si llegue al final
		if(encontreUnaParticionUtil == 0)
		{
			//estoy en la particion final, esta libre?
			if(auxiliar->laParticion.estaLibre == 1)
			{
				//su espacio me alcanza?
				if((tenemosEspacio(&auxiliar,&particionElegida,tamanioMemoria, size, semParticiones) == 0))//devuelve 1 si SI, 0 si NO
				{
					//hay que fletar una particion
					seleccionDeVictima(CACHE, laLista, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
					particionElegida = seleccionar_particion_First_Fit(CACHE, tamanioMemoria, laLista, size, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
				}
			}
			//no esta libre, hay que crear una particion nueva
			else
			{
				//el espacio que resta en la memoria me alcanza?
				if((auxiliar->laParticion.limiteSuperior < tamanioMemoria) && ((tamanioMemoria - auxiliar->laParticion.limiteSuperior)>= size))
				{
					//el espacio que resta en memoria SI me alcanza, por lo que creo una particion nueva a continuacion de la particion en que estoy parado
					sem_wait(semParticiones);
					particionElegida = crear_particion(auxiliar, size, "PD");
					sem_post(semParticiones);
				}
				//el espacio que resta en memoria NO me alcanza
				else
				{
					//hay que fletar una particion
					seleccionDeVictima(CACHE, laLista, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
					particionElegida = seleccionar_particion_First_Fit(CACHE, tamanioMemoria, laLista, size, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
				}
			}
		}

		//llegue hasta acá porque SI encontre una particion util Y NO es la última
		else
		{
			particionElegida = auxiliar;//todo rever
			sem_wait(semParticiones);
			crear_particion(particionElegida, size, "PD");
			sem_post(semParticiones);
		}
	}
	return particionElegida;
}

lista_particiones* seleccionar_particion_Best_Fit(void* CACHE, uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS, char* ADMIN_MEMORIA, t_log* logger, sem_t* semLog, sem_t* semParticiones, sem_t* semParticionesEliminadas)
{
	lista_particiones* auxiliar = laLista;
	lista_particiones* particionElegida = NULL;
	particionesCandidatas* candidata = malloc(sizeof(particionesCandidatas));
	candidata->numero_de_particion = -1;
	candidata->sig_candidata = NULL;
	particionesCandidatas* nuevaCandidata = candidata;
	particionesCandidatas* manejo_de_candidatas = NULL;
	uint32_t variableControlRecorrerLista = 1;

	//si estoy al principio de la lista, la particion esta libre y no hay + particiones, la eleccion es facil...
	if((auxiliar->numero_de_particion == 0) && (auxiliar->laParticion.estaLibre == 1) && (auxiliar->sig_particion == NULL))
		{
			particionElegida = auxiliar;
			//por fin encontre el fucking UNICO caso en el que la primera particion es la unica y con un tamaño distinto de 0
			if((particionElegida->laParticion.limiteSuperior - particionElegida->laParticion.limiteInferior) < size)
			{
				sem_wait(semParticiones);
				crear_particion(particionElegida, size, "PD");
				sem_post(semParticiones);
			}
		}

	//si no se puede hacer en la 1ra particion, hago todas las otras verificaciones
	else
	{
		//seguir recorriendo la lista hasta que llegue al final
		while(variableControlRecorrerLista == 1)
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
					//me sirve, lo "anoto" en la lista de candidatas
					nuevaCandidata->numero_de_particion = auxiliar->numero_de_particion;
					nuevaCandidata->size_de_particion = (auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior);
					nuevaCandidata->puntero_a_particion_candidata = auxiliar;

					//burocracia para armar la lista de candidatas
					manejo_de_candidatas = nuevaCandidata;
					nuevaCandidata->sig_candidata = malloc(sizeof(particionesCandidatas));
					nuevaCandidata = nuevaCandidata->sig_candidata;
					nuevaCandidata->sig_candidata = NULL;
					nuevaCandidata->numero_de_particion = -1;
					manejo_de_candidatas->sig_candidata = nuevaCandidata;

					//y me muevo en la lista de particiones
					auxiliar = auxiliar->sig_particion;
				}
				//esta libre, pero no me alcanza el espacio
				else
				{
					auxiliar = auxiliar->sig_particion;
				}
			}
			//si se llego al final de la lista, freno el while
			if(auxiliar == NULL)
			{
				variableControlRecorrerLista = 0;
			}
		}

		//Encontre una particion util?
		if(candidata->numero_de_particion != -1)
		{
			if(candidata->sig_candidata->numero_de_particion == -1)//solo encontre 1 particion util => uso esa :P
			{
				particionElegida = candidata->puntero_a_particion_candidata;

				sem_wait(semParticiones);//todo agregado
				crear_particion(particionElegida, size, "PD");
				sem_post(semParticiones);
			}

			//hay + de una particion potable, las comparo
			else
			{
				particionElegida = comparador_de_candidatas(candidata);
				sem_wait(semParticiones);//todo agregado
				crear_particion(particionElegida, size, "PD");
				sem_post(semParticiones);
			}
		}

		//nop, no sirve nada
		else
		{
			auxiliar = laLista;
			//me posiciono al final de la lista
			while(auxiliar->sig_particion != NULL)
			{
				auxiliar = auxiliar->sig_particion;
			}

			//el espacio que resta en la memoria me alcanza?
			if((auxiliar->laParticion.limiteSuperior < tamanioMemoria) && ((tamanioMemoria - auxiliar->laParticion.limiteSuperior)>= size))
			{
				//el espacio que resta en memoria SI me alcanza, por lo que creo una particion nueva a continuacion de la particion en que estoy parado
				sem_wait(semParticiones);
				particionElegida = crear_particion(auxiliar, size, "PD");
				sem_post(semParticiones);
			}
			//el espacio que resta en memoria NO me alcanza
			else
			{
				//hay que fletar una particion
				seleccionDeVictima(CACHE, laLista, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
				particionElegida = seleccionar_particion_Best_Fit(CACHE, tamanioMemoria, laLista, size, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
			}
		}
	}

	//libero el espacio para la lista de particiones candidatas
	matar_lista_particiones_candidatas(candidata);

	return particionElegida;
}

lista_particiones* seleccionar_particion_Buddy_System(void* CACHE, uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size, char* algoritmoAsignacion, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS, char* ADMIN_MEMORIA, t_log* logger, sem_t* semLog, sem_t* semParticiones, sem_t* semParticionesEliminadas)
{
	lista_particiones* auxiliar = laLista;
	lista_particiones* particionElegida = NULL;
	uint32_t variableControlRecorrerLista = 1;
	uint32_t encontreUnaParticionUtil = 0;
	particionesCandidatas* candidata = malloc(sizeof(particionesCandidatas));
	candidata->numero_de_particion = -1;
	candidata->sig_candidata = NULL;
	particionesCandidatas* nuevaCandidata = candidata;
	particionesCandidatas* manejo_de_candidatas = NULL;

	//si estoy al principio de la lista, la particion esta libre y no hay + particiones, comienza la locura...
	if((auxiliar->numero_de_particion == 0) && (auxiliar->laParticion.estaLibre == 1) && (auxiliar->sig_particion == NULL))
	{
		//como es la primera, todas las particiones se acomodan en base a esta info, asi que se termina eligiendo la primera
		sem_wait(semParticiones);
		particionElegida = crear_particion(auxiliar, size, "BS");
		sem_post(semParticiones);
	}

	//la primera esta en uso, asi que sigo buscando siempre que no llegue al final
	else
	{
		//busco particion con FF
		if(strcmp(algoritmoAsignacion,"FF") == 0)
		{
			//seguir recorriendo la lista hasta que llegue al final o encuentre algo
			while(variableControlRecorrerLista == 1)
			{
				//si la particion esta ocupada, busco otra
				if(auxiliar->laParticion.estaLibre == 0)
				{
					auxiliar = auxiliar->sig_particion;
				}

				//encontre una libre
				else
				{
					//si encuentro una particion lo suficientemente grande como para poner mis datos, es la candidata para ser mutilada
					if((auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior) >= size)
					{
						//victima
						particionElegida = crear_particion(auxiliar, size, "BS");

						//No hace falta seguir recorriendo la lista si ya encontre candidata
						variableControlRecorrerLista = 0;
						encontreUnaParticionUtil = 1;
					}
					//esta libre, pero no me alcanza el espacio
					else
					{
						auxiliar = auxiliar->sig_particion;
					}
				}
				//si se llego al final de la lista, freno el while
				if(auxiliar == NULL)
				{
					variableControlRecorrerLista = 0;
				}
			}

			//no me sirve ninguna particion?
			if(encontreUnaParticionUtil != 1)
			{
				//hay que fletar una particion
				seleccionDeVictima(CACHE, laLista, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
			}
		}

		//busco particion con Best Fit
		if(strcmp(algoritmoAsignacion,"BF") == 0)
		{
			//seguir recorriendo la lista hasta que llegue al final
			while(variableControlRecorrerLista == 1)
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
						//me sirve, lo "anoto" en la lista de candidatas
						nuevaCandidata->numero_de_particion = auxiliar->numero_de_particion;
						nuevaCandidata->size_de_particion = (auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior);
						nuevaCandidata->puntero_a_particion_candidata = auxiliar;

						//burocracia para armar la lista de candidatas
						manejo_de_candidatas = nuevaCandidata;
						nuevaCandidata->sig_candidata = malloc(sizeof(particionesCandidatas));
						nuevaCandidata = nuevaCandidata->sig_candidata;
						nuevaCandidata->sig_candidata = NULL;
						nuevaCandidata->numero_de_particion = -1;
						manejo_de_candidatas->sig_candidata = nuevaCandidata;

						//y me muevo en la lista de particiones
						auxiliar = auxiliar->sig_particion;
					}
					//esta libre, pero no me alcanza el espacio
					else
					{
						auxiliar = auxiliar->sig_particion;
					}
				}
				//si se llego al final de la lista, freno el while
				if(auxiliar == NULL)
				{
					variableControlRecorrerLista = 0;
				}
			}

			//Encontre una particion util?
			if(candidata->numero_de_particion != -1)
			{
				if(candidata->sig_candidata->numero_de_particion == -1)//solo encontre 1 particion util => uso esa :P
				{
					sem_wait(semParticiones);
					particionElegida = crear_particion(candidata->puntero_a_particion_candidata, size, "BS");
					sem_post(semParticiones);
				}

				//hay + de una particion potable, las comparo
				else
				{
					sem_wait(semParticiones);
					particionElegida = crear_particion(comparador_de_candidatas(candidata), size, "BS");
					sem_post(semParticiones);
				}
			}

			//nop, no sirve nada
			else
			{
				//hay que fletar una particion
				seleccionDeVictima(CACHE, laLista, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
			}
		}
	}//esta llave es el cierre de "la primera esta en uso, revisar las otras..."

	//si para este punto TODAVIA no se eligio una particion, entonces ninguna servia, por lo que corrio el algoritmo para eliminar una particion
	if(particionElegida == NULL)
	{
		//por lo que llamamos a buddy system de nuevo
		particionElegida = seleccionar_particion_Buddy_System(CACHE, tamanioMemoria, laLista, size, algoritmoAsignacion, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
	}

	//libero el espacio para la lista de particiones candidatas
	matar_lista_particiones_candidatas(candidata);

	return particionElegida;
}

lista_particiones* comparador_de_candidatas(particionesCandidatas* listaDeCandidatas)
{
	//asumo que la primera particion es la mas chica
	lista_particiones* elegida = listaDeCandidatas->puntero_a_particion_candidata;
	uint32_t sizeMenor = listaDeCandidatas->size_de_particion;
	uint32_t variableControlRecorrerLista = 1;
	uint32_t mostrarElegida = listaDeCandidatas->numero_de_particion;

	//recorro la lista de particiones candidatas entera para comparar todos los sizes
	while(variableControlRecorrerLista == 1)
	{
		if((sizeMenor > listaDeCandidatas->size_de_particion) && (listaDeCandidatas->numero_de_particion != -1)) //si alguno tiene un size mas chico, se elige ese
		{
			sizeMenor = listaDeCandidatas->size_de_particion;
			elegida = listaDeCandidatas->puntero_a_particion_candidata;
			mostrarElegida = listaDeCandidatas->numero_de_particion;
		}

		listaDeCandidatas = listaDeCandidatas->sig_candidata;

		if(listaDeCandidatas == NULL)
		{
			variableControlRecorrerLista = 0;
		}
	}
	printf("Algoritmo BF: Se eligió la partición: %u.\n", mostrarElegida);
	return elegida;
}

uint32_t tenemosEspacio(lista_particiones** auxiliar, lista_particiones** particionElegida, uint32_t tamanioMemoria, uint32_t size, t_log* semParticiones)
{
	lista_particiones* punteroAAuxiliar = *auxiliar;
	uint32_t resultado;

	//me sirve la ultima particion?
	if((punteroAAuxiliar->laParticion.limiteSuperior - punteroAAuxiliar->laParticion.limiteInferior) >= size)
	{
		*particionElegida = *auxiliar; //tenemos un ganador
		sem_wait(semParticiones);
		crear_particion(*particionElegida, size, "PD");
		sem_post(semParticiones);		//todo aca poner particion nuevaaaaa
		resultado = 1;
	}

	else
	{
		//no me sirve, si todavia hay espacio en memoria para meter algo...
		if(punteroAAuxiliar->laParticion.limiteSuperior < tamanioMemoria)
		{
			//...veo si ese espacio me sirve
			if(tamanioMemoria - punteroAAuxiliar->laParticion.limiteSuperior >= size)
			{
				//el espacio que resta en memoria SI me alcanza, por lo que creo una particion nueva a continuacion de la particion en que estoy parado
				sem_wait(semParticiones);
				*particionElegida = crear_particion(punteroAAuxiliar, size, "PD");
				sem_post(semParticiones);
			}
			//el espacio que resta en memoria no me alcanza
			else
			{
				resultado = 0;
			}
		}

		//la memoria esta totalmente copada
		else
		{
			resultado = 0;
		}
	}
	return resultado;
}

void poner_en_particion(void* CACHE, lista_particiones* particionElegida, void* estructura, codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA, sem_t* semNumeroVictima)
{
	switch(tipoMensaje)
	{
			case NEW:
				poner_NEW_en_particion(CACHE, particionElegida, estructura, NUMERO_VICTIMA, semNumeroVictima, tipoMensaje);
				break;

			case APPEARED:
				poner_APPEARED_en_particion(CACHE, particionElegida, estructura, NUMERO_VICTIMA, semNumeroVictima, tipoMensaje);
				break;

			case GET:
				poner_GET_en_particion(CACHE, particionElegida, estructura, NUMERO_VICTIMA, semNumeroVictima, tipoMensaje);
				break;

			case LOCALIZED:
				poner_LOCALIZED_en_particion(CACHE, particionElegida, estructura, NUMERO_VICTIMA, semNumeroVictima, tipoMensaje);
				break;

			case CATCH:
				poner_CATCH_en_particion(CACHE, particionElegida, estructura, NUMERO_VICTIMA, semNumeroVictima, tipoMensaje);
				break;

			case CAUGHT:
				poner_CAUGHT_en_particion(CACHE, particionElegida, estructura, NUMERO_VICTIMA, semNumeroVictima, tipoMensaje);
				break;

			case SUSCRIPCION://Estos 7 están solo para que no salga el WARNING, no sirven para nada aca
				break;

			case DESSUSCRIPCION:
				break;

			case TEST://que se joda test
				break;

			case DESCONEXION:
				break;

			case ERROR:
				break;

			case CONFIRMACION:
				break;

			case IDMENSAJE:
				break;
	}

}

void poner_NEW_en_particion(void* CACHE, lista_particiones* particionElegida, New* estructura, uint32_t* NUMERO_VICTIMA, sem_t* semNumeroVictima, codigo_operacion tipoMensaje)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	//printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre);
	desplazamiento += estructura->largoNombre;

	//meto coordenada X del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);

	//meto coordenada Y del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);

	//meto cantidad del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->cantPokemon), sizeof(estructura->cantPokemon));
	desplazamiento += sizeof(estructura->cantPokemon);

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	//aumento su numero de victima
	sem_wait(semNumeroVictima);
	*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
	sem_post(semNumeroVictima);
	particionElegida->numero_de_victima = *NUMERO_VICTIMA;

	//me guardo a QUE ID pertenecen estos datos
	particionElegida->ID_MENSAJE_GUARDADO = estructura->ID;

	//me guardo en que cola se encuentra este mensaje
	particionElegida->cola = tipoMensaje;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_APPEARED_en_particion(void* CACHE, lista_particiones* particionElegida, Appeared* estructura, uint32_t* NUMERO_VICTIMA, sem_t* semNumeroVictima, codigo_operacion tipoMensaje)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	//printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre);
	desplazamiento += estructura->largoNombre;

	//meto coordenada X del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);

	//meto coordenada Y del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	//aumento su numero de victima
	sem_wait(semNumeroVictima);
	*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
	sem_post(semNumeroVictima);
	particionElegida->numero_de_victima = *NUMERO_VICTIMA;

	//me guardo a QUE ID pertenecen estos datos
	particionElegida->ID_MENSAJE_GUARDADO = estructura->ID;

	//me guardo en que cola se encuentra este mensaje
	particionElegida->cola = tipoMensaje;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_GET_en_particion(void* CACHE, lista_particiones* particionElegida, Get* estructura, uint32_t* NUMERO_VICTIMA, sem_t* semNumeroVictima, codigo_operacion tipoMensaje)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	//printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre);
	desplazamiento += estructura->largoNombre;

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	//aumento su numero de victima
	sem_wait(semNumeroVictima);
	*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
	sem_post(semNumeroVictima);
	particionElegida->numero_de_victima = *NUMERO_VICTIMA;

	//me guardo a QUE ID pertenecen estos datos
	particionElegida->ID_MENSAJE_GUARDADO = estructura->ID;

	//me guardo en que cola se encuentra este mensaje
	particionElegida->cola = tipoMensaje;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_LOCALIZED_en_particion(void* CACHE, lista_particiones* particionElegida, Localized* estructura, uint32_t* NUMERO_VICTIMA, sem_t* semNumeroVictima, codigo_operacion tipoMensaje)
{
	uint32_t iterador = 0;
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	//printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre);
	desplazamiento += estructura->largoNombre;

	//meto la cantidad de lugares donde se encuentra el pokemon, en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->cantPosciciones), sizeof(estructura->cantPosciciones));
	desplazamiento += sizeof(estructura->cantPosciciones);

	if(estructura->cantPosciciones == 0)
	{
		memcpy(CACHE + desplazamiento, &(estructura->coords[0]), sizeof(estructura->coords[0]));
		desplazamiento += sizeof(estructura->coords[0]);

		memcpy(CACHE + desplazamiento, &(estructura->coords[0]), sizeof(estructura->coords[0]));
		desplazamiento += sizeof(estructura->coords[0]);
	}

	else
	{
		//meto pares de coordenadas donde hay pokemons en CACHE
		while(iterador <= (estructura->cantPosciciones * 2) - 1)
		{
			memcpy(CACHE + desplazamiento, &(estructura->coords[iterador]), sizeof(estructura->coords[iterador]));
			desplazamiento += sizeof(estructura->coords[iterador]);
			iterador++;
		}
	}

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	//aumento su numero de victima
	sem_wait(semNumeroVictima);
	*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
	sem_post(semNumeroVictima);
	particionElegida->numero_de_victima = *NUMERO_VICTIMA;

	//me guardo a QUE ID pertenecen estos datos
	particionElegida->ID_MENSAJE_GUARDADO = estructura->ID;

	//me guardo en que cola se encuentra este mensaje
	particionElegida->cola = tipoMensaje;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_CATCH_en_particion(void* CACHE, lista_particiones* particionElegida, Catch* estructura, uint32_t* NUMERO_VICTIMA, sem_t* semNumeroVictima, codigo_operacion tipoMensaje)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	//printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre);
	desplazamiento += estructura->largoNombre;

	//meto coordenada X del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);

	//meto coordenada Y del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	//aumento su numero de victima
	sem_wait(semNumeroVictima);
	*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
	sem_post(semNumeroVictima);
	particionElegida->numero_de_victima = *NUMERO_VICTIMA;

	//me guardo a QUE ID pertenecen estos datos
	particionElegida->ID_MENSAJE_GUARDADO = estructura->ID;

	//me guardo en que cola se encuentra este mensaje
	particionElegida->cola = tipoMensaje;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_CAUGHT_en_particion(void* CACHE, lista_particiones* particionElegida, Caught* estructura, uint32_t* NUMERO_VICTIMA, sem_t* semNumeroVictima, codigo_operacion tipoMensaje)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	//printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

//	//meto el largo del nombre del pokemon en CACHE
//	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
//	desplazamiento += sizeof(estructura->largoNombre);
//
//	//meto nombre del pokemon en CACHE
//	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre+1);
//	desplazamiento += estructura->largoNombre+1;

	//meto el resultado del intento de CATCH en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->pudoAtrapar), sizeof(estructura->pudoAtrapar));
	desplazamiento += sizeof(estructura->pudoAtrapar);

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	//aumento su numero de victima
	sem_wait(semNumeroVictima);
	*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
	sem_post(semNumeroVictima);
	particionElegida->numero_de_victima = *NUMERO_VICTIMA;

	//me guardo a QUE ID pertenecen estos datos
	particionElegida->ID_MENSAJE_GUARDADO = estructura->ID;

	//me guardo en que cola se encuentra este mensaje
	particionElegida->cola = tipoMensaje;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void verificacionPosicion(uint32_t limiteSuperiorDeParticion, uint32_t posicionSuperiorUltimoDato)
{
	if(limiteSuperiorDeParticion == posicionSuperiorUltimoDato)
	{
		puts("Información agregada ocupando toda la partición.\n");
	}
	else
	{
		if(posicionSuperiorUltimoDato < limiteSuperiorDeParticion)
		{
			puts("Información agregada con fragmentación interna.\n");
		}
		else
		{
			puts("ATENCION! LA INFORMACIÓN SE SALIÓ DE LA PARTICION!!\n");
		}
	}
}

void agregar_mensaje_a_Cache(void* CACHE, uint32_t tamanioMemoria, uint32_t tamanioMinParticion, char* ADMIN_MEMORIA, lista_particiones* laLista, char* algoritmoAsignacion, void* estructuraMensaje, uint32_t sizeDelMensaje, codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA, uint32_t FRECUEN_COMPACT, uint32_t* PARTICIONES_ELIMINADAS, t_log* logger, sem_t* semLog, sem_t* semCache, sem_t* semParticiones, sem_t* semNumeroVictima, sem_t* semParticionesEliminadas)
{
	lista_particiones* particionElegida;
	uint32_t tamanioAAsignar = tamanioMinParticion;
	uint32_t iterador = 0;

	sem_wait(semLog);
	log_info(logger, "Agregando mensaje a Cache...\n");
	sem_post(semLog);

	//se administra con Particiones Dinamicas
	if(strcmp(ADMIN_MEMORIA, "PD") == 0)
	{
		//si el tamanio de los datos del mensaje es mayor al tamaño min de particion, se asigna el tamanio del mensaje
		if(tamanioMinParticion < sizeDelMensaje)
		{
			tamanioAAsignar = sizeDelMensaje;
		}

		//para poder meter el mensaje en Cache, primero hay que buscarle una particion
		if(strcmp(algoritmoAsignacion,"FF") == 0)
		{
			particionElegida = seleccionar_particion_First_Fit(CACHE, tamanioMemoria, laLista, tamanioAAsignar, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
		}
		else
		{
			if(strcmp(algoritmoAsignacion,"BF") == 0)
			{
				particionElegida = seleccionar_particion_Best_Fit(CACHE, tamanioMemoria, laLista, tamanioAAsignar, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
			}
			else
			{
				puts("Alguien escribió mal el campo de ALGORITMO_PARTICION_LIBRE del archivo config, asi que Hasta La vista, Baby!");
				abort();
			}
		}
	}

	//se administra con Buddy System
	else
	{
		while(tamanioAAsignar < sizeDelMensaje)//el espacio a asignar tiene que ser una potencia de 2...
		{
			tamanioAAsignar = elevar_a(2, iterador);
			iterador++;
		}
		particionElegida = seleccionar_particion_Buddy_System(CACHE, tamanioMemoria, laLista, tamanioAAsignar, algoritmoAsignacion, FRECUEN_COMPACT, PARTICIONES_ELIMINADAS, ADMIN_MEMORIA, logger, semLog, semParticiones, semParticionesEliminadas);
	}

	printf("Particion %u elegida exitosamente.\n", particionElegida->numero_de_particion); //borrar en el futuro?
	sem_wait(semLog);
	log_info(logger, "Inicio de la partición elegida: %u.", particionElegida->laParticion.limiteInferior);
	sem_post(semLog);

	//ahora que tenemos la particion, metemos los datos
	sem_wait(semCache);
	sem_wait(semParticiones);
	poner_en_particion(CACHE, particionElegida, estructuraMensaje, tipoMensaje, NUMERO_VICTIMA, semNumeroVictima);
	sem_post(semParticiones);
	sem_post(semCache);

//PASO 3: profit?
}

lista_particiones* buscarLaParticionDelMensaje(lista_particiones* laLista, int32_t ID_MENSAJE, int32_t* particionEncontrada)
{
	lista_particiones* auxiliar = laLista;

	while(auxiliar != NULL && *particionEncontrada == -1)
	{
		if(auxiliar->ID_MENSAJE_GUARDADO == ID_MENSAJE)
		{
			*particionEncontrada = 1;
			printf("Los datos del mensaje ID: %u fueron encontrados en la partición %u.\n", ID_MENSAJE, auxiliar->numero_de_particion);
		}
		else
		{
			auxiliar = auxiliar->sig_particion;
		}
	}
	return auxiliar;
}

void sacar_de_particion(void* CACHE, lista_particiones* particionDelMensaje, void* estructura, codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO, sem_t* semNumeroVictima)
{
	switch(tipoMensaje)
	{
			case NEW:
				sacar_NEW_de_particion(CACHE, particionDelMensaje, estructura, NUMERO_VICTIMA, ALGOR_REEMPLAZO, semNumeroVictima);
				break;

			case APPEARED:
				sacar_APPEARED_de_particion(CACHE, particionDelMensaje, estructura, NUMERO_VICTIMA, ALGOR_REEMPLAZO, semNumeroVictima);
				break;

			case GET:
				sacar_GET_de_particion(CACHE, particionDelMensaje, estructura, NUMERO_VICTIMA, ALGOR_REEMPLAZO, semNumeroVictima);
				break;

			case LOCALIZED:
				sacar_LOCALIZED_de_particion(CACHE, particionDelMensaje, estructura, NUMERO_VICTIMA, ALGOR_REEMPLAZO, semNumeroVictima);
				break;

			case CATCH:
				sacar_CATCH_de_particion(CACHE, particionDelMensaje, estructura, NUMERO_VICTIMA, ALGOR_REEMPLAZO, semNumeroVictima);
				break;

			case CAUGHT:
				sacar_CAUGHT_de_particion(CACHE, particionDelMensaje, estructura, NUMERO_VICTIMA, ALGOR_REEMPLAZO, semNumeroVictima);
				break;

			case SUSCRIPCION://Estos 6 están solo para que no salga el WARNING, no sirven para nada aca
				break;

			case DESSUSCRIPCION:
				break;

			case TEST://que se joda test
				break;

			case DESCONEXION:
				break;

			case ERROR:
				break;

			case CONFIRMACION:
				break;
	}
}

void sacar_NEW_de_particion(void* CACHE, lista_particiones* particionDelMensaje, New* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO, sem_t* semNumeroVictima)
{
	//hago que empiece a leer al principio de la particion
	uint32_t desplazamiento = particionDelMensaje->laParticion.limiteInferior;

	char* referenciaTexto;

	//saco el largo del nombre del pokemon de CACHE
	memcpy(&estructura->largoNombre, CACHE + desplazamiento, sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);
	printf("largo nombre: %u\n", estructura->largoNombre);

	//preparo un espacio para almacenar el nombre del pokemon
	referenciaTexto = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon de CACHE
	memcpy(referenciaTexto, CACHE + desplazamiento, estructura->largoNombre);
	referenciaTexto[estructura->largoNombre] = '\0';

	//hago que la estructura se guarde ese nombre (no se puede hacer memcpy directamente a la estructura)
	estructura->nombrePokemon = referenciaTexto;
	desplazamiento += estructura->largoNombre;
	printf("nombre pokemon: %s\n", estructura->nombrePokemon);

	//saco coordenada X del pokemon de CACHE
	memcpy(&estructura->posPokemon.x, CACHE + desplazamiento, sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);
	printf("pos X: %u\n", estructura->posPokemon.x);

	//saco coordenada Y del pokemon de CACHE
	memcpy(&estructura->posPokemon.y, CACHE + desplazamiento, sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);
	printf("pos Y: %u\n", estructura->posPokemon.y);

	//saco cantidad de pokemones de CACHE
	memcpy(&estructura->cantPokemon, CACHE + desplazamiento, sizeof(estructura->cantPokemon));
	desplazamiento += sizeof(estructura->cantPokemon);
	printf("cantidad: %u\n", estructura->cantPokemon);

	if(strcmp(ALGOR_REEMPLAZO,"LRU") == 0)
	{
		//aumento su numero de victima
		sem_wait(semNumeroVictima);
		*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
		sem_post(semNumeroVictima);
		particionDelMensaje->numero_de_victima = *NUMERO_VICTIMA;
	}
}

void sacar_APPEARED_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Appeared* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO, sem_t* semNumeroVictima)
{
	//hago que empiece a leer al principio de la particion
	uint32_t desplazamiento = particionDelMensaje->laParticion.limiteInferior;

	char* referenciaTexto;

	//saco el largo del nombre del pokemon de CACHE
	memcpy(&estructura->largoNombre, CACHE + desplazamiento, sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);
	printf("largo nombre: %u\n", estructura->largoNombre);

	//preparo un espacio para almacenar el nombre del pokemon
	referenciaTexto = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon de CACHE
	memcpy(referenciaTexto, CACHE + desplazamiento, estructura->largoNombre);
	referenciaTexto[estructura->largoNombre] = '\0';

	//hago que la estructura se guarde ese nombre (no se puede hacer memcpy directamente a la estructura)
	estructura->nombrePokemon = referenciaTexto;
	desplazamiento += estructura->largoNombre;
	printf("nombre pokemon: %s\n", estructura->nombrePokemon);

	//saco coordenada X del pokemon de CACHE
	memcpy(&estructura->posPokemon.x, CACHE + desplazamiento, sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);
	printf("pos X: %u\n", estructura->posPokemon.x);

	//saco coordenada Y del pokemon de CACHE
	memcpy(&estructura->posPokemon.y, CACHE + desplazamiento, sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);
	printf("pos Y: %u\n", estructura->posPokemon.y);

	if(strcmp(ALGOR_REEMPLAZO,"LRU") == 0)
	{
		//aumento su numero de victima
		sem_wait(semNumeroVictima);
		*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
		sem_post(semNumeroVictima);
		particionDelMensaje->numero_de_victima = *NUMERO_VICTIMA;
	}
}

void sacar_GET_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Get* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO, sem_t* semNumeroVictima)
{
	//hago que empiece a leer al principio de la particion
	uint32_t desplazamiento = particionDelMensaje->laParticion.limiteInferior;

	char* referenciaTexto;

	//saco el largo del nombre del pokemon de CACHE
	memcpy(&estructura->largoNombre, CACHE + desplazamiento, sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);
	printf("largo nombre: %u\n", estructura->largoNombre);

	//preparo un espacio para almacenar el nombre del pokemon
	referenciaTexto = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon de CACHE
	memcpy(referenciaTexto, CACHE + desplazamiento, estructura->largoNombre);
	referenciaTexto[estructura->largoNombre] = '\0';

	//hago que la estructura se guarde ese nombre (no se puede hacer memcpy directamente a la estructura)
	estructura->nombrePokemon = referenciaTexto;
	desplazamiento += estructura->largoNombre;
	printf("nombre pokemon: %s\n", estructura->nombrePokemon);

	if(strcmp(ALGOR_REEMPLAZO,"LRU") == 0)
	{
		//aumento su numero de victima
		sem_wait(semNumeroVictima);
		*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
		sem_post(semNumeroVictima);
		particionDelMensaje->numero_de_victima = *NUMERO_VICTIMA;
	}
}

void sacar_LOCALIZED_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Localized* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO, sem_t* semNumeroVictima)
{
	//hago que empiece a leer al principio de la particion
	uint32_t desplazamiento = particionDelMensaje->laParticion.limiteInferior;

	char* referenciaTexto;
	uint32_t iterador = 0;

	//saco el largo del nombre del pokemon de CACHE
	memcpy(&estructura->largoNombre, CACHE + desplazamiento, sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);
	printf("largo nombre: %u\n", estructura->largoNombre);

	//preparo un espacio para almacenar el nombre del pokemon
	referenciaTexto = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon de CACHE
	memcpy(referenciaTexto, CACHE + desplazamiento, estructura->largoNombre);
	referenciaTexto[estructura->largoNombre] = '\0';

	//hago que la estructura se guarde ese nombre (no se puede hacer memcpy directamente a la estructura)
	estructura->nombrePokemon = referenciaTexto;
	desplazamiento += estructura->largoNombre;
	printf("nombre pokemon: %s\n", estructura->nombrePokemon);

	//saco la cantidad de lugares donde se encuentra el pokemon, de CACHE
	memcpy(&estructura->cantPosciciones, CACHE + desplazamiento, sizeof(estructura->cantPosciciones));
	desplazamiento += sizeof(estructura->cantPosciciones);
	printf("cantidad pos: %u\n", estructura->cantPosciciones);

	//saco pares de coordenadas donde hay pokemons en CACHE
	if(estructura->cantPosciciones == 0)
	{
		memcpy(&estructura->coords[0], CACHE + desplazamiento, sizeof(estructura->coords[0]));
		desplazamiento += sizeof(estructura->coords[0]);
		printf("coordenada: %u\n", estructura->coords[0]);

		memcpy(&estructura->coords[1], CACHE + desplazamiento, sizeof(estructura->coords[1]));
		desplazamiento += sizeof(estructura->coords[1]);
		printf("coordenada: %u\n", estructura->coords[1]);
	}

	else
	{
		while(iterador <= (estructura->cantPosciciones * 2) - 1)
		{
			memcpy(&estructura->coords[iterador], CACHE + desplazamiento, sizeof(estructura->coords[iterador]));
			desplazamiento += sizeof(estructura->coords[iterador]);

			printf("coordenada: %u\n", estructura->coords[iterador]);
			iterador++;
		}
	}

	if(strcmp(ALGOR_REEMPLAZO,"LRU") == 0)
	{
		//aumento su numero de victima
		sem_wait(semNumeroVictima);
		*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
		sem_post(semNumeroVictima);
		particionDelMensaje->numero_de_victima = *NUMERO_VICTIMA;
	}
}

void sacar_CATCH_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Catch* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO, sem_t* semNumeroVictima)
{
	//hago que empiece a leer al principio de la particion
	uint32_t desplazamiento = particionDelMensaje->laParticion.limiteInferior;

	char* referenciaTexto;

	//saco el largo del nombre del pokemon de CACHE
	memcpy(&estructura->largoNombre, CACHE + desplazamiento, sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);
	printf("largo nombre: %u\n", estructura->largoNombre);

	//preparo un espacio para almacenar el nombre del pokemon
	referenciaTexto = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon de CACHE
	memcpy(referenciaTexto, CACHE + desplazamiento, estructura->largoNombre);
	referenciaTexto[estructura->largoNombre] = '\0';

	//hago que la estructura se guarde ese nombre (no se puede hacer memcpy directamente a la estructura)
	estructura->nombrePokemon = referenciaTexto;
	desplazamiento += estructura->largoNombre;
	printf("nombre pokemon: %s\n", estructura->nombrePokemon);

	//saco coordenada X del pokemon de CACHE
	memcpy(&estructura->posPokemon.x, CACHE + desplazamiento, sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);
	printf("pos X: %u\n", estructura->posPokemon.x);

	//saco coordenada Y del pokemon de CACHE
	memcpy(&estructura->posPokemon.y, CACHE + desplazamiento, sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);
	printf("pos Y: %u\n", estructura->posPokemon.y);

	if(strcmp(ALGOR_REEMPLAZO,"LRU") == 0)
	{
		//aumento su numero de victima
		sem_wait(semNumeroVictima);
		*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
		sem_post(semNumeroVictima);
		particionDelMensaje->numero_de_victima = *NUMERO_VICTIMA;
	}
}

void sacar_CAUGHT_de_particion(void* CACHE, lista_particiones* particionDelMensaje, Caught* estructura, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO, sem_t* semNumeroVictima)
{
	//hago que empiece a leer al principio de la particion
	uint32_t desplazamiento = particionDelMensaje->laParticion.limiteInferior;

//	char* referenciaTexto;
//
//	//saco el largo del nombre del pokemon de CACHE
//	memcpy(&estructura->largoNombre, CACHE + desplazamiento, sizeof(estructura->largoNombre));
//	desplazamiento += sizeof(estructura->largoNombre);
//	printf("largo nombre: %u\n", estructura->largoNombre);
//
//	//preparo un espacio para almacenar el nombre del pokemon
//	referenciaTexto = malloc(estructura->largoNombre+1);
//
//	//saco el nombre del pokemon de CACHE
//	memcpy(referenciaTexto, CACHE + desplazamiento, estructura->largoNombre+1);
//
//	//hago que la estructura se guarde ese nombre (no se puede hacer memcpy directamente a la estructura)
//	estructura->nombrePokemon = referenciaTexto;
//	desplazamiento += estructura->largoNombre+1;
//	printf("nombre pokemon: %s\n", estructura->nombrePokemon);

	//saco el resultado del intento de CATCH de CACHE
	memcpy(&estructura->pudoAtrapar, CACHE + desplazamiento, sizeof(estructura->pudoAtrapar));
	desplazamiento += sizeof(estructura->pudoAtrapar);
	printf("pudo atrapar?: %u\n", estructura->pudoAtrapar);

	if(strcmp(ALGOR_REEMPLAZO,"LRU") == 0)
	{
		//aumento su numero de victima
		sem_wait(semNumeroVictima);
		*NUMERO_VICTIMA = *NUMERO_VICTIMA + 1;
		sem_post(semNumeroVictima);
		particionDelMensaje->numero_de_victima = *NUMERO_VICTIMA;
	}
}

uint32_t sacar_mensaje_de_Cache(void* CACHE, lista_particiones* laLista, void* estructuraMensaje, int32_t ID_MENSAJE,codigo_operacion tipoMensaje, uint32_t* NUMERO_VICTIMA, char* ALGOR_REEMPLAZO, sem_t* semNumeroVictima, sem_t* semCache)
{
	lista_particiones* particionDelMensaje;
	int32_t particionEncontrada = -1;

	puts("Buscando mensaje en CACHE...");
	particionDelMensaje = buscarLaParticionDelMensaje(laLista, ID_MENSAJE, &particionEncontrada);

	if(particionEncontrada == 1)
	{
		sem_wait(semCache);
		sacar_de_particion(CACHE, particionDelMensaje, estructuraMensaje, tipoMensaje, NUMERO_VICTIMA, ALGOR_REEMPLAZO, semNumeroVictima);
		sem_post(semCache);
	}

	else
	{
		puts("Los datos del mensaje ya no se encuentran en CACHE.");
		particionEncontrada = 0;
	}
	return particionEncontrada;
}

uint32_t calcular_bytes_utiles_de_mensaje(void* mensaje, codigo_operacion tipoMensaje)
{
	uint32_t totalAAsignar = 0;

	switch(tipoMensaje){
		case NEW:
				totalAAsignar = calcular_bytes_utiles_de_mensaje_new(mensaje);
			break;

		case APPEARED:
				totalAAsignar = calcular_bytes_utiles_de_mensaje_appeared(mensaje);
			break;

		case GET:
				totalAAsignar = calcular_bytes_utiles_de_mensaje_get(mensaje);
			break;

		case LOCALIZED:
				totalAAsignar = calcular_bytes_utiles_de_mensaje_localized(mensaje);
			break;

		case CATCH:
				totalAAsignar = calcular_bytes_utiles_de_mensaje_catch(mensaje);
			break;

		case CAUGHT:
				totalAAsignar = calcular_bytes_utiles_de_mensaje_caught(mensaje);
			break;

		case TEST:
			//Estos 6 están solo para que no salga el WARNING, no sirven para nada aca
			break;

		case SUSCRIPCION:
			break;

		case DESSUSCRIPCION:
			break;

		case DESCONEXION:
			break;

		case ERROR:
			break;

		case CONFIRMACION:
			break;
	}
	return totalAAsignar;
}

uint32_t calcular_bytes_utiles_de_mensaje_new(New* estructura)
{
	uint32_t totalAAsignar = 0;

	totalAAsignar += sizeof(estructura->largoNombre);
	totalAAsignar += estructura->largoNombre;
	totalAAsignar += sizeof(estructura->posPokemon.x);
	totalAAsignar += sizeof(estructura->posPokemon.y);
	totalAAsignar += sizeof(estructura->cantPokemon);

	return totalAAsignar;
}

uint32_t calcular_bytes_utiles_de_mensaje_appeared(Appeared* estructura)
{
	uint32_t totalAAsignar = 0;

	totalAAsignar += sizeof(estructura->largoNombre);
	totalAAsignar += estructura->largoNombre;
	totalAAsignar += sizeof(estructura->posPokemon.x);
	totalAAsignar += sizeof(estructura->posPokemon.y);

	return totalAAsignar;
}

uint32_t calcular_bytes_utiles_de_mensaje_get(Get* estructura)
{
	uint32_t totalAAsignar = 0;

	totalAAsignar += sizeof(estructura->largoNombre);
	totalAAsignar += estructura->largoNombre;

	return totalAAsignar;
}

uint32_t calcular_bytes_utiles_de_mensaje_localized(Localized* estructura)
{
	uint32_t totalAAsignar = 0;

	totalAAsignar += sizeof(estructura->largoNombre);
	totalAAsignar += estructura->largoNombre;
	totalAAsignar += sizeof(estructura->cantPosciciones);
	if(estructura->cantPosciciones == 0)
	{
		totalAAsignar += sizeof(estructura->coords[0]) * 2;
	}
	else
	{
		totalAAsignar += sizeof(estructura->coords[0]) * 2 * estructura->cantPosciciones;
	}

	return totalAAsignar;
}

uint32_t calcular_bytes_utiles_de_mensaje_catch(Catch* estructura)
{
	uint32_t totalAAsignar = 0;

	totalAAsignar += sizeof(estructura->largoNombre);
	totalAAsignar += estructura->largoNombre;
	totalAAsignar += sizeof(estructura->posPokemon.x);
	totalAAsignar += sizeof(estructura->posPokemon.y);

	return totalAAsignar;
}

uint32_t calcular_bytes_utiles_de_mensaje_caught(Caught* estructura)
{
	uint32_t totalAAsignar = 0;

//	totalAAsignar += sizeof(estructura->largoNombre);
//	totalAAsignar += estructura->largoNombre + 1;
	totalAAsignar += sizeof(estructura->pudoAtrapar);

	return totalAAsignar;
}
