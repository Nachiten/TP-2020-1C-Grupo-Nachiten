/*
 * config.c
 *
 *  Created on: 2 may. 2020
 *      Author: Andres
 */

#include "config.h"

char** temp1;
d_entrenador* entrenador;

/* Lee los datos de la config en los parametros pasados por referencia:
PARAMS:
@config : Archivo de config
@entrenadores: lista de entrenadores
@objetivo_global: Todos los pokemones objetivo
@cant_entrenadores: Cantidad de entrenadores existentes
@objetivos: Cantidad de objetivos
*/
int extraer_valores_config(t_config* config, int* algoritmo_planificacion, int* quantum, int* estimacion_inicial, int* retardo, uint32_t* tiempo_reconexion){
    int respuesta = 1;
    char* algoritmo = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    *algoritmo_planificacion = convertir_algoritmo(algoritmo);
    if(*algoritmo_planificacion == -1)
    {
        printf("Error en extraccion de algoritmo de planificacion\n");
        respuesta = 0;
    }
    *quantum = config_get_int_value(config, "QUANTUM");
    *estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");
    *retardo = config_get_int_value(config, "RETARDO_CICLO_CPU");
    *tiempo_reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
    //revisa que sean todas con valores mayores o iguales a 0
    if(validar_datos(*quantum, *retardo, *tiempo_reconexion, *estimacion_inicial) == 0)
    {
        printf("error en extraccion de datos ints\n");
        respuesta = -1;
    }
    return respuesta;
}

int inicializar_entrenadores_con_config(t_config* config, d_entrenador** entrenadores, char*** objetivo_global, int* cant_entrenadores, int* objetivos){
    int i, cant_objetivos, cant_posiciones, respuesta;
    cant_objetivos = 0;
    respuesta = 1;
    i=0;

    //toma los valores de la config como arrays
    char** posicion_entrenador = config_get_array_value(config, "POSICIONES_ENTRENADORES");
    char** pokemones_actuales = config_get_array_value(config, "POKEMON_ENTRENADORES");
    char** objetivo = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
    //verifica que las CANTIDADES sean las mismas y las anota en cant_posiciones
    cant_posiciones = validar_tamano_vectores_extraidos(posicion_entrenador, objetivo, pokemones_actuales);

    // Calculo cuanto mide el array de pokemons actuales
    int cantidadElementosEnPokemonsActuales = calcular_elementos_null(pokemones_actuales);

    if(cant_posiciones > 0)
    {
		printf("La cantidad de entrenadores es: %i\n", cant_posiciones);
		entrenador = malloc(cant_posiciones * sizeof(d_entrenador));

		while(i<cant_posiciones && respuesta == 1)
		{
			entrenador[i].estado = ESTADO_NEW;
			entrenador[i].estado_block = ACTIVO;

			char* posicionCompleta = posicion_entrenador[i];

			char** posiciones = string_split(posicionCompleta,"|");

			entrenador[i].posicion[0] = atoi(posiciones[0]);
			entrenador[i].posicion[1] = atoi(posiciones[1]);

			// Printeo para probar
			//printf("PosicionX: %i, PosicionY: %i", entrenador[i].posicion[0], entrenador[i].posicion[1]);

			//entrenador[i].posicion[0] = convertir_a_int(posicion_entrenador[i][0]);
			// TODO | Modificar para q pueda tener numeros de mas de uno de largo
			//entrenador[i].posicion[1] = convertir_a_int(posicion_entrenador[i][2]);
			entrenador[i].numeroEntrenador = i + 1; // El numero de entrenador comienza en 1
			entrenador[i].cantCiclosCPU = 0; // Se inicializa en 0 para que no tire basura dsp

			if (i < cantidadElementosEnPokemonsActuales){
				llenar_objetivos_y_actuales_de_entrenador(&(entrenador[i]), objetivo[i], pokemones_actuales[i]);
			} else {
				llenar_objetivos_y_actuales_de_entrenador(&(entrenador[i]), objetivo[i], NULL);
			}

			i++;
		}
		if(respuesta == 1)
		{
			cant_objetivos = calcular_tamano_objetivo_global(entrenador, cant_posiciones);
			printf("La cantidad de objetivos es: %i\n", cant_objetivos);
			temp1 = malloc((cant_objetivos+1) * sizeof(char*));
			llenar_objetivo_global(entrenador, cant_posiciones, temp1, cant_objetivos);//anota los pokemones que les faltan al team completo
			*entrenadores = entrenador;
			*cant_entrenadores = cant_posiciones;
			*objetivo_global = temp1;//aca quedan guardados cuales y cuantos pokemones faltan al team para terminar su trabajo
			*objetivos = cant_objetivos;
		}
		else
		{
			printf("error en llenado de objetivos y actuales de entrenador %i\n", i-1);
		}
    }
    else{
        printf("error en validacion de vectores extraidos\n");
	respuesta = -2;
    }

    // Liberando memoria
    free(posicion_entrenador);
    free(objetivo);
    free(pokemones_actuales);

    return respuesta;
}

int calcular_elementos_null(char** vector){
    int i = 0;
    while(vector[i] != NULL){i++;}
    return i;
}

int validar_tamano_vectores_extraidos(char** posicion_entrenador, char** objetivo, char** pokemones_actuales){
	int cant_pokemones, cant_posiciones, cant_objetivos, respuesta;
	cant_pokemones = calcular_elementos_null(pokemones_actuales);
	cant_posiciones = calcular_elementos_null(posicion_entrenador);
	cant_objetivos = calcular_elementos_null(objetivo);
	//respuesta = niguno_es_cero_y_son_todos_iguales(cant_pokemones, cant_posiciones, cant_objetivos);
	respuesta = cant_posiciones;
	return respuesta;
}

int niguno_es_cero_y_son_todos_iguales(int primerNumero, int segundoNumero, int tercerNumero){
	int respuesta = primerNumero;

	if(primerNumero != segundoNumero || primerNumero != tercerNumero){
		respuesta = -1;
	}
	else{
		if(primerNumero == 0){
			respuesta = 0;
		}
	}

	return respuesta;
}

int convertir_a_int(char* unCaracter){
    int conversion = unCaracter - '0';
    return conversion;
}

int convertir_algoritmo(char* algoritmo){
    int i, respuesta;
    i=0;
    respuesta = -1;
    char* vector[4] = {"FIFO", "RR", "SJF-SD", "SJF-CD"};
    while(i<4 && respuesta == -1){
        if(son_iguales_char(vector[i], algoritmo) == 1){
            respuesta = i;
        }
        i++;
    }
    return respuesta;
}

int validar_datos(int quantum, int retardo, int tiempo_reconexion, int estimacion){
    int respuesta = 1;
    if(quantum < 0 || retardo < 0 || tiempo_reconexion < 0 || estimacion < 0){
        respuesta = 0;
    }
    return respuesta;
}

int llenar_objetivos_y_actuales_de_entrenador(d_entrenador* entrenador, char* vector_objetivos, char* vector_pokemones){
	int i, cont, aux_cont, respuesta;
	char** vector;
	respuesta = 1;

	// vetor_objetivos es tipo: Pikachu|Bulbasaur
	// vector_pokemones es idem anterior pero para los pokemones actuales

	vector = string_split(vector_objetivos, "|");
	cont = calcular_elementos_null(vector);

	entrenador->objetivo = malloc((cont+1) * sizeof(char*));

	// Llenar los objetivos del pokemon
	for(i=0;i<(cont+1);i++){
		entrenador->objetivo[i] = vector[i];
	}

	if(vector_pokemones != NULL ){
		vector = string_split(vector_pokemones, "|");
		// Cantidad de elementos de
		aux_cont = calcular_elementos_null(vector);
	} else {
		aux_cont = 0;
	}

	if(aux_cont <= cont){
		// Se hace malloc para los pokemons actuales
		entrenador->pokemones_actuales = malloc(cont * sizeof(char*));

		// Se llenan los pokemons actuales
		for(i=0;i<aux_cont;i++){
			entrenador->pokemones_actuales[i] = vector[i];
		}

		if(aux_cont < cont){
			for(i=aux_cont;i<cont;i++){
				entrenador->pokemones_actuales[i] = NULL;
			}
		}
		else{actualizar_estado_entrenador(entrenador, 1);}
	}
	else{respuesta = 1;}

	return respuesta;
}

int calcular_tamano_objetivo_global(d_entrenador* entrenadores, int cant_entrenadores){
    int i, j, cont, diferencia;
    cont = 0;
    for(i=0;i<cant_entrenadores;i++)
    {
        if(entrenadores[i].estado == ESTADO_NEW)
        {
            j=0;
            while(entrenadores[i].objetivo[j] != NULL)
            {
                if(pokemon_se_repitio_antes_en_objetivo(entrenadores[i].objetivo[j], entrenadores[i].objetivo, j) == 0 && pokemon_se_repitio_antes_en_objetivos_entrenadores(entrenadores[i].objetivo[j], i, entrenadores) == 0)
                {
                	//calcula cuantos faltan de C/U de los pokemones objetivos
                    diferencia = diferencia_cantidad_de_en_objetivos_y_pokemones_entrenadores(entrenadores[i].objetivo[j], entrenadores, cant_entrenadores);
                    if(diferencia < 0)
                    {
                        printf("error pokemon %s encontrado en actuales mas veces que en objetivos de entrenador %i\n", entrenadores[i].pokemones_actuales[j], i);
                    }
                    else{cont = cont + diferencia;}
                }
                j++;
            }
        }
    }
    return cont;
}

int pokemon_se_repitio_antes_en_objetivos_entrenadores(char* pokemon, int pos_entrenador, d_entrenador* entrenadores){
    int i, respuesta;
    i=0;
    respuesta = 0;
    while(i<pos_entrenador && esta_en_objetivo_individual(pokemon, entrenadores[i].objetivo) == 0){i++;}
    if(i<pos_entrenador){respuesta = 1;}
    return respuesta;
}

int diferencia_cantidad_de_en_objetivos_y_pokemones_entrenadores(char* pokemon, d_entrenador* entrenadores, int cant_entrenadores){
    //cuantos necesita de ESTE pokemon
	int cant_objetivos = cantidad_de_en_objetivos_entrenadores(pokemon, entrenadores, cant_entrenadores);
    //cuantos tienen los entrenadores de ESTE pokemon
	int cant_en_entrenadores = cantidad_de_en_pokemones_entrenadores(pokemon, entrenadores, cant_entrenadores);
    //cuantos faltan
	int respuesta = cant_objetivos - cant_en_entrenadores;
    return respuesta;
}

int cantidad_de_en_objetivos_entrenadores(char* pokemon, d_entrenador* entrenadores, int cant_entrenadores){
    int i, cont;
    cont = 0;
    for(i=0;i<cant_entrenadores;i++){
        cont = cont + cantidad_de_veces_en_objetivo_individual(pokemon, entrenadores[i].objetivo);
    }
    return cont;
}

int cantidad_de_en_pokemones_entrenadores(char* pokemon, d_entrenador* entrenadores, int cant_entrenadores){
    int i, cont;
    cont = 0;

    int j = 0;

    for(i=0;i<cant_entrenadores;i++){
        //cont = cont + cantidad_de_veces_en_actuales(pokemon, entrenadores[i].pokemones_actuales);

		//Cuenta la cantidad de pokemones que tiene cada entrenador
		while(entrenadores[i].pokemones_actuales[j] != NULL)
		{
			j++;
		}

    	cont = cont + cantidad_de_veces_en_actuales(pokemon, entrenadores[i].pokemones_actuales, j);
    	j = 0;
    }
    return cont;
}

void llenar_objetivo_global(d_entrenador* entrenadores, int cant_entrenadores, char** vector_objetivo_global, int cant_objetivos){
    int i, j, cont, diferencia;
    cont = 0;
    for(i=0;i<cant_entrenadores;i++){
        if(entrenadores[i].estado == ESTADO_NEW){
            j=0;
            while(entrenadores[i].objetivo[j] != NULL){
                if(pokemon_se_repitio_antes_en_objetivo(entrenadores[i].objetivo[j], entrenadores[i].objetivo, j) == 0 && pokemon_se_repitio_antes_en_objetivos_entrenadores(entrenadores[i].objetivo[j], i, entrenadores) == 0){
                    diferencia = diferencia_cantidad_de_en_objetivos_y_pokemones_entrenadores(entrenadores[i].objetivo[j], entrenadores, cant_entrenadores);
                    if(diferencia > 0){
                        llenar_con(cont, cont+diferencia, vector_objetivo_global, entrenadores[i].objetivo[j]);
                        cont = cont + diferencia;
                    }
                }
                j++;
            }
        }
    }
    vector_objetivo_global[cant_objetivos] = NULL;
}

void liberarConfig(){
    free(temp1);
    free(entrenador);
}



