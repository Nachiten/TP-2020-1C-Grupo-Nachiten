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

			laLista->sig_particion = particionACrear;

			particionACrear->numero_de_particion = (laLista->numero_de_particion +1);
			particionACrear->anter_particion = laLista;
			particionACrear->sig_particion = NULL;
			particionACrear->laParticion.estaLibre = 0;
			particionACrear->laParticion.limiteInferior = laLista->laParticion.limiteSuperior;
			particionACrear->laParticion.limiteSuperior = (particionACrear->laParticion.limiteInferior + sizeDeLosDatos);
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

//			if(laLista->anter_particion != NULL)
//			{
//				laLista = laLista->anter_particion;
//			}

			printf("limite sup: %u\n",laLista->laParticion.limiteSuperior);
		}
		corregirNumerosParticiones(laLista, numeroParticionOriginal);
		return laLista;
	}
}

void corregirNumerosParticiones(lista_particiones* particionOriginal, uint32_t numeroParticionOriginal)
{
	while(particionOriginal->sig_particion != NULL)
	{
		particionOriginal = particionOriginal->sig_particion;
		numeroParticionOriginal++;
		particionOriginal->numero_de_particion = numeroParticionOriginal;
	}
}


void seleccionDeVictima(lista_particiones* laLista, uint32_t FRECUEN_COMPACT, uint32_t PARTICIONES_ELIMINADAS, char* ALGORITMO_MEMORIA)
{
	lista_particiones* particionABorrar = laLista;
	uint32_t masViejoUsado = particionABorrar->numero_de_victima;
	uint32_t victima = 0;

	//recorremos la lista hasta el final buscando una victima
	while(particionABorrar != NULL)
	{
		//cuanto mas viejo fue su uso, mejor opcion como victima
		if(particionABorrar->numero_de_victima < masViejoUsado)
		{
			victima = particionABorrar->numero_de_particion;
			masViejoUsado = particionABorrar->numero_de_victima;
		}
		particionABorrar = particionABorrar->sig_particion;
	}
	particionABorrar = laLista;
	//para este punto ya tenemos nuestra victima, solo hay que seleccionarla y mandarla a borrar
	while(laLista->numero_de_particion != victima)
	{
		particionABorrar = particionABorrar->sig_particion;
	}

	//mandamos la victima al matadero
	borrarReferenciaAParticion(particionABorrar, PARTICIONES_ELIMINADAS);

	//si frecuencia de compactacion es -1, 0 o 1 se compacta siempre
	//sino, solo cuando la cantidad de particiones eliminadas sea igual a la frecuencia que piden
	//Y SI Y SOLO SI NO SE ESTA USANDO Bullshit System
	if(((FRECUEN_COMPACT == -1) || (FRECUEN_COMPACT == 0) || (FRECUEN_COMPACT == 1) || (FRECUEN_COMPACT == PARTICIONES_ELIMINADAS)) && (strcmp(ALGORITMO_MEMORIA,"BS") != 0))
	{
		compactacion();
		PARTICIONES_ELIMINADAS = 0;
	}
}

void borrarReferenciaAParticion(lista_particiones* particionABorrar, uint32_t PARTICIONES_ELIMINADAS)
{
	particionABorrar->laParticion.estaLibre = 1;
	printf("La particion %u ahora está libre!\n", particionABorrar->numero_de_particion);
	PARTICIONES_ELIMINADAS++;
	consolidarParticion(particionABorrar);
}

void consolidarParticion(lista_particiones* particionABorrar)
{
	uint32_t numPart = particionABorrar->numero_de_particion;
	uint32_t numPartAnt = 0;
	uint32_t numPartSig = 0;
	uint32_t resultado = 0;
	uint32_t consolidado = 0;

	if(particionABorrar->anter_particion != NULL)
	{
		numPartAnt = particionABorrar->anter_particion->numero_de_particion;

		//si la particion anterior a la que borro esta libre, las consolido
		if(particionABorrar->anter_particion->laParticion.estaLibre == 1)
		{
			//expando el tamaño de la particion anterior
			particionABorrar->anter_particion->laParticion.limiteSuperior = particionABorrar->laParticion.limiteSuperior;
			resultado = particionABorrar->anter_particion->numero_de_particion;

			//si la particion que estoy borrando no apunta a NULL, entonces hago que la anterior apunte a su siguiente y viceversa
			if(particionABorrar->sig_particion != NULL)
			{
				particionABorrar->anter_particion->sig_particion = particionABorrar->sig_particion;
				particionABorrar->sig_particion->anter_particion = particionABorrar->anter_particion;
			}

			consolidado = 1;
			printf("La particion %u fue consolidada con la particion %u y ahora se llaman partición %u.\n\n;", numPart, numPartAnt, resultado);
		}
	}

	if(particionABorrar->sig_particion != NULL)
	{
		numPartSig = particionABorrar->sig_particion->numero_de_particion;

		//si la particion siguiente a la que borro esta libre, las consolido
		if(particionABorrar->sig_particion->laParticion.estaLibre == 1)
		{
			//expando el tamaño de la particion siguiente
			particionABorrar->sig_particion->laParticion.limiteInferior = particionABorrar->laParticion.limiteInferior;
			particionABorrar->sig_particion->numero_de_particion = particionABorrar->numero_de_particion;

			resultado = particionABorrar->numero_de_particion;

			//si la particion que estoy borrando no "viene de" NULL, entonces hago que la siguiente apunte a su anterior y viceversa
			if(particionABorrar->anter_particion != NULL)
			{
				particionABorrar->sig_particion->anter_particion = particionABorrar->anter_particion;
				particionABorrar->anter_particion->sig_particion = particionABorrar->sig_particion;
			}

			consolidado = 1;
			printf("La particion %u fue consolidada con la particion %u y ahora se llaman partición %u.\n\n", numPart, numPartSig, resultado);
		}
	}

	if(consolidado == 1)
	{
		free(particionABorrar);
	}
}

void compactacion ()
{
	puts("todavia no hago un zorongo, asi q bye bye");
	abort();
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

void revision_lista_particiones(lista_particiones* laLista, uint32_t tamanioMemoria)
{
	lista_particiones* auxiliar = laLista;
	uint32_t espacioLibre = 0;
	uint32_t espacioOcupado = 0;
	uint32_t control = 1;

	puts("Realizando control del estado de memoria y particiones...");
	puts("------------------------------------");


	//recorro las particiones hasta la ultima
	while(control == 1)
	{
		printf("Nº de particion: %u.\n", auxiliar->numero_de_particion);
		if(auxiliar->laParticion.estaLibre == 1)
		{
			puts("La partición está libre.");
			espacioLibre += (auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior); //si hay una particion libre, suma el espacio "desperdiciado"
		}
		else
		{
			puts("La particion está en uso.");
			espacioOcupado += (auxiliar->laParticion.limiteSuperior - auxiliar->laParticion.limiteInferior); //si hay una particion ocupada, suma el espacio en uso
		}
		printf("Límite inferior de la partición: %u.\n", auxiliar->laParticion.limiteInferior);
		printf("Límite superior de la partición: %u.\n", auxiliar->laParticion.limiteSuperior);
		puts("------------------------------------");

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
	printf("\nEspacio desperdiciado en particiones libres: %u.\n", espacioLibre);
	printf("Espacio libre en CACHE (No particionado): %u.\n", (tamanioMemoria - espacioOcupado - espacioLibre));
	printf("Espacio total libre: %u.\n", (tamanioMemoria - espacioOcupado));

	puts("---No se tuvo en cuenta la fragmentación interna durante el checkeo---\n");
}

lista_particiones* seleccionar_particion_First_Fit(uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size)
{
	lista_particiones* auxiliar = laLista;
	lista_particiones* particionElegida = NULL;
	uint32_t encontreUnaParticionUtil = 0;

	//si estoy al principio de la lista, y no hay + particiones, la eleccion es facil...
	if((auxiliar->numero_de_particion == 0) && (auxiliar->laParticion.estaLibre == 1) && (auxiliar->sig_particion == NULL))
	{
		particionElegida = auxiliar;
		crear_particion(particionElegida, size, "PD");//ToDo puede la primera particion ser la unica, estar vacia y TENER un tamaño mayor a 0?????
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
				if((tenemosEspacio(&auxiliar,&particionElegida,tamanioMemoria, size) == 0))//devuelve 1 si SI, 0 si NO
				{
					//ToDo ACA TENDRIA QUE ENTRAR COMPACTACION? COMPLETAR CUANDO ESTE CLARO QUE HACER SI NO HAY MANERA DE METER LOS DATOS
					//seleccionDeVictima();
				}
			}
			//no esta libre, hay que crear una particion nueva
			else
			{
				//el espacio que resta en la memoria me alcanza?
				if((auxiliar->laParticion.limiteSuperior < tamanioMemoria) && ((tamanioMemoria - auxiliar->laParticion.limiteSuperior)>= size))
				{
					//el espacio que resta en memoria SI me alcanza, por lo que creo una particion nueva a continuacion de la particion en que estoy parado
					particionElegida = crear_particion(auxiliar, size, "PD");

				}
				//el espacio que resta en memoria NO me alcanza
				else
				{
					//ToDo ACA TENDRIA QUE ENTRAR COMPACTACION? COMPLETAR CUANDO ESTE CLARO QUE HACER SI NO HAY MANERA DE METER LOS DATOS
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
	return particionElegida;
}

lista_particiones* seleccionar_particion_Best_Fit(uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size)
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
		crear_particion(particionElegida, size, "PD");//ToDo puede la primera particion ser la unica, estar vacia y TENER un tamaño mayor a 0?????
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
			}

			//hay + de una particion potable, las comparo
			else
			{
				particionElegida = comparador_de_candidatas(candidata);
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
				particionElegida = crear_particion(auxiliar, size, "PD");

			}
			//el espacio que resta en memoria NO me alcanza
			else
			{
				//ToDo ACA TENDRIA QUE ENTRAR COMPACTACION? COMPLETAR CUANDO ESTE CLARO QUE HACER SI NO HAY MANERA DE METER LOS DATOS
			}
		}
	}

	//libero el espacio para la lista de particiones candidatas
	matar_lista_particiones_candidatas(candidata);

	puts("Particion elegida exitosamente"); //borrar en el futuro?
	return particionElegida;
}

lista_particiones* seleccionar_particion_Buddy_System(uint32_t tamanioMemoria, lista_particiones* laLista, uint32_t size, char* algoritmoAsignacion)
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
		particionElegida = crear_particion(auxiliar, size, "BS");
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
				//ToDo time to call grim Reaper
				puts("eliminar no implementado");
				abort();
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
					particionElegida = crear_particion(candidata->puntero_a_particion_candidata, size, "BS");
				}

				//hay + de una particion potable, las comparo
				else
				{
					particionElegida = crear_particion(comparador_de_candidatas(candidata), size, "BS");
				}
			}

			//nop, no sirve nada
			else
			{
				//ToDo time to call grim Reaper
				puts("eliminar no implementado");
				abort();
			}
		}
	}//esta llave es el cierre de "la primera esta en uso, revisar las otras..."

	//si para este punto TODAVIA no se eligio una particion, entonces ninguna servia, por lo que corrio el algoritmo para eliminar una particion
	if(particionElegida == NULL)
	{
		//por lo que llamamos a buddy system de nuevo
		particionElegida = seleccionar_particion_Buddy_System(tamanioMemoria, laLista, size, algoritmoAsignacion);
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

uint32_t tenemosEspacio(lista_particiones** auxiliar, lista_particiones** particionElegida, uint32_t tamanioMemoria, uint32_t size)
{
	lista_particiones* punteroAAuxiliar = *auxiliar;

	uint32_t resultado;
	//si todavia hay espacio para meter algo...
	if(punteroAAuxiliar->laParticion.limiteSuperior <= tamanioMemoria)
	{
		//...veo si ese espacio me sirve
		if((punteroAAuxiliar->laParticion.limiteSuperior - punteroAAuxiliar->laParticion.limiteInferior) >= size)
		{
			*particionElegida = *auxiliar; //tenemos un ganador
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

//ToDo actualizar los PONER EN PARTICION!!
void poner_en_particion(void* CACHE, lista_particiones* particionElegida, void* estructura, codigo_operacion tipoMensaje)
{
	switch(tipoMensaje)
	{
			case NEW:
				poner_NEW_en_particion(CACHE, particionElegida, estructura);
				free(estructura);//esto me causaria problemas? parece que no...
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

void poner_NEW_en_particion(void* CACHE, lista_particiones* particionElegida, New* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre+1);
	desplazamiento += estructura->largoNombre+1;

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

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	/*//PARA PROBAR LECTURA DE CACHE
	uint32_t mostrar_numero = 0;
	char* mostrar_texto = malloc(10);
	desplazamiento = 0;

	memcpy(&mostrar_numero, CACHE + desplazamiento, sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);
	printf("lo que tiene la variable mostrar_numero es: %u\n", mostrar_numero);

	memcpy(mostrar_texto, CACHE + desplazamiento, estructura->largoNombre+1);
	desplazamiento += estructura->largoNombre+1;
	printf("lo que tiene la variable mostrar_texto es: %s\n", mostrar_texto);

	memcpy(&mostrar_numero, CACHE + desplazamiento, sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);
	printf("lo que tiene la variable mostrar_numero es: %u\n", mostrar_numero);

	memcpy(&mostrar_numero, CACHE + desplazamiento, sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);
	printf("lo que tiene la variable mostrar_numero es: %u\n", mostrar_numero);

	memcpy(&mostrar_numero, CACHE + desplazamiento, sizeof(estructura->cantPokemon));
	desplazamiento += sizeof(estructura->cantPokemon);
	printf("lo que tiene la variable mostrar_numero es: %u\n", mostrar_numero);

	memcpy(&mostrar_numero, CACHE + desplazamiento, sizeof(estructura->ID));
	desplazamiento += sizeof(estructura->ID);
	printf("lo que tiene la variable mostrar_numero es: %u\n", mostrar_numero);

	memcpy(&mostrar_numero, CACHE + desplazamiento, sizeof(estructura->corrID));
	desplazamiento += sizeof(estructura->corrID);
	printf("lo que tiene la variable mostrar_numero es: %u\n", mostrar_numero);
	*/

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_APPEARED_en_particion(void* CACHE, lista_particiones* particionElegida, Appeared* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre+1);
	desplazamiento += estructura->largoNombre+1;

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

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_GET_en_particion(void* CACHE, lista_particiones* particionElegida, Get* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre+1);
	desplazamiento += estructura->largoNombre+1;

	//meto ID del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->ID), sizeof(estructura->ID));
	desplazamiento += sizeof(estructura->ID);

	//meto ID Correlativa del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->corrID), sizeof(estructura->corrID));
	desplazamiento += sizeof(estructura->corrID);

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_LOCALIZED_en_particion(void* CACHE, lista_particiones* particionElegida, Localized* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre+1);
	desplazamiento += estructura->largoNombre+1;

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

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_CATCH_en_particion(void* CACHE, lista_particiones* particionElegida, Catch* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre+1);
	desplazamiento += estructura->largoNombre+1;

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

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

	verificacionPosicion(particionElegida->laParticion.limiteSuperior, desplazamiento);
}

void poner_CAUGHT_en_particion(void* CACHE, lista_particiones* particionElegida, Caught* estructura)
{
	uint32_t desplazamiento = particionElegida->laParticion.limiteInferior;
	printf("Inicio de la particion: %u\n", particionElegida->laParticion.limiteInferior);

	//meto el largo del nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->largoNombre), sizeof(estructura->largoNombre));
	desplazamiento += sizeof(estructura->largoNombre);

	//meto nombre del pokemon en CACHE
	memcpy(CACHE + desplazamiento, estructura->nombrePokemon, estructura->largoNombre+1);
	desplazamiento += estructura->largoNombre+1;

	//meto el resultado del intento de CATCH en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->pudoAtrapar), sizeof(estructura->pudoAtrapar));
	desplazamiento += sizeof(estructura->pudoAtrapar);

	//meto ID del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->ID), sizeof(estructura->ID));
	desplazamiento += sizeof(estructura->ID);

	//meto ID Correlativa del mensaje en CACHE
	memcpy(CACHE + desplazamiento, &(estructura->corrID), sizeof(estructura->corrID));
	desplazamiento += sizeof(estructura->corrID);

	printf("Fin de la particion: %u\n", particionElegida->laParticion.limiteSuperior);
	particionElegida->laParticion.estaLibre = 0;

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

void agregar_mensaje_a_Cache(void* CACHE, uint32_t tamanioMemoria, uint32_t tamanioMinParticion, char* algorAdminMemoria, lista_particiones* laLista, char* algoritmoAsignacion, void* estructuraMensaje, uint32_t sizeDelMensaje, codigo_operacion tipoMensaje)
{
	lista_particiones* particionElegida;
	uint32_t tamanioAAsignar = 0;
	uint32_t iterador = 0;

	puts("Agregando mensaje a Cache...");

	//se administra con Particiones Dinamicas
	if(strcmp(algorAdminMemoria, "PD") == 0)
	{
		tamanioAAsignar =tamanioMinParticion;

		//si el tamanio de los datos del mensaje es mayor al tamaño min de particion, se asigna el tamanio del mensaje
		if(tamanioMinParticion < sizeDelMensaje)
		{
			tamanioAAsignar = sizeDelMensaje;
		}

		//para poder meter el mensaje en Cache, primero hay que buscarle una particion
		if(strcmp(algoritmoAsignacion,"FF") == 0)
		{
			particionElegida = seleccionar_particion_First_Fit(tamanioMemoria, laLista, tamanioAAsignar);
		}
		else
		{
			if(strcmp(algoritmoAsignacion,"BF") == 0)
			{
				particionElegida = seleccionar_particion_Best_Fit(tamanioMemoria, laLista, tamanioAAsignar);
				puts("Implementando...");
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
		particionElegida = seleccionar_particion_Buddy_System(tamanioMemoria, laLista, tamanioAAsignar, algoritmoAsignacion);
	}

	//ahora que tenemos la particion, metemos los datos
	poner_en_particion(CACHE, particionElegida, estructuraMensaje, tipoMensaje);

//PASO 3: profit?
}

