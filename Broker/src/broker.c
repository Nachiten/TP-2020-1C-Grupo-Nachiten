#include "Broker.h"


typedef struct Entrenador{

	// Temporal | Num Pokedex
	int* listaPokemones;

	struct Entrenador* siguiente;
	struct Entrenador* anterior;

}Entrenador;

// *************************************************
void mostrameMemoria(int32_t senial);

int main(void) {

	t_config* config;
	char* IP_BROKER;
	char* PUERTO_BROKER;

	id_inicial = -1;
	inicializar_colas();
	inicializar_semaforos();

	printf("Process ID del Broker: %i\n", getpid());
	signal(SIGUSR1, mostrameMemoria);
	signal(SIGUSR2, mostrameMemoria);

	//Cargo las configuraciones del .config
	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/Broker.config");

	if (config == NULL){
		printf("No se pudo leer la config\n");
		return EXIT_FAILURE;
	} else {
		printf("La config fue leida correctamente\n");
	}

	// Leer path del log
	char* pathLogs = config_get_string_value(config,"LOG_FILE");
	//leer path del dump de la cache
	char* pathDump = config_get_string_value(config,"RUTA_DUMP_FILE");

	if (pathLogs == NULL || pathDump == NULL){
		printf("No se pudo leer el path del log de la config\n");
		return EXIT_FAILURE;
	} else {
		printf("El path del log fue leido correctamente\n");
	}

	//Dejo cargado un logger para loguear los eventos y otro para el Dump de la CACHE.
	logger = cargarUnLog(pathLogs, "Broker");
	dumpCache = cargarUnLog(pathDump, "CACHE");

	IP_BROKER = config_get_string_value(config,"IP_BROKER");

	if (IP_BROKER == NULL){
		printf("No se pudo leer la ip de la config\n");
		return EXIT_FAILURE;
	} else {
		printf("La IP fue leida correctamente\n");
	}

	PUERTO_BROKER = config_get_string_value(config,"PUERTO_BROKER");

	if (PUERTO_BROKER == NULL){
		printf("No se pudo leer el puerto de la config\n");
		return EXIT_FAILURE;
	} else {
		printf("El puerto fue leido correctamente\n");
	}

	hoja_de_particiones = malloc(sizeof(lista_particiones));
	PARTICIONES_ELIMINADAS = 0;
	NUMERO_VICTIMA = 0;

	puts("****************************************************");
	puts("Cargando configuraciones de memoria...\n");

	//aca cargo las los valores del config para manejar la memoria
	TAMANIO_MEM = config_get_int_value(config,"TAMANO_MEMORIA");
	printf("-El tamaño para la memoria Cache es: %u bytes.\n", TAMANIO_MEM);
	TAMANIO_MIN_PART = config_get_int_value(config,"TAMANO_MINIMO_PARTICION");
	printf("-El tamaño minimo de partición es: %u bytes.\n", TAMANIO_MIN_PART);
	ADMIN_MEMORIA = config_get_string_value(config,"ALGORITMO_MEMORIA");
	printf("-La memoria será administrada usando el algoritmo %s.\n", ADMIN_MEMORIA);
	ALGOR_REEMPLAZO = config_get_string_value(config,"ALGORITMO_REEMPLAZO");
	printf("-Algoritmo de reemplazo en uso: %s.\n", ALGOR_REEMPLAZO);
	ALGOR_ASIGN_PARTICION = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
	printf("-Algoritmo de asignacion de partición: %s.\n", ALGOR_ASIGN_PARTICION);
	FRECUEN_COMPACT = config_get_int_value(config,"FRECUENCIA_COMPACTACION");
	printf("-Frecuencia de compactación: %u.\n", FRECUEN_COMPACT);

	puts("\nConfiguraciones de memoria cargadas correctamente.");
	puts("****************************************************");

	//preparo el espacio de memoria cache
	CACHE = malloc(TAMANIO_MEM);
	puts("Malloc de memoria completo");

	//Preparo la lista de referencias a las particiones dentro de CACHE
	inicializar_lista_particiones(hoja_de_particiones, ADMIN_MEMORIA, TAMANIO_MEM);

	//Arranco el Broker como servidor.
	puts("Arrancando servidor Broker...\n");
	iniciar_server(IP_BROKER, PUERTO_BROKER);

	//borramos toda referencia a particiones dentro de CACHE
	matar_lista_particiones(hoja_de_particiones);
	puts("Referencias a particiones eliminadas.");

	//no olvidarse de reventar la memoria reservada para CACHE
	puts("Liberando memoria Cache...");
	free(CACHE);
	puts("Memoria Cache Liberada.");

	return EXIT_SUCCESS;
}
// *************************************************

void inicializar_colas(){

	colaDescartesNew = malloc(sizeof(t_cola));
	colaDescartesNew->tipoCola = NEW;
	colaDescartesNew->mensajes = list_create();
	colaDescartesNew->subs = list_create();

	colaDescartesAppeared = malloc(sizeof(t_cola));
	colaDescartesAppeared->tipoCola = NEW;
	colaDescartesAppeared->mensajes = list_create();
	colaDescartesAppeared->subs = list_create();

	colaDescartesGet = malloc(sizeof(t_cola));
	colaDescartesGet->tipoCola = NEW;
	colaDescartesGet->mensajes = list_create();
	colaDescartesGet->subs = list_create();

	colaDescartesLocalized = malloc(sizeof(t_cola));
	colaDescartesLocalized->tipoCola = NEW;
	colaDescartesLocalized->mensajes = list_create();
	colaDescartesLocalized->subs = list_create();

	colaDescartesCatch = malloc(sizeof(t_cola));
	colaDescartesCatch->tipoCola = NEW;
	colaDescartesCatch->mensajes = list_create();
	colaDescartesCatch->subs = list_create();

	colaDescartesCaught = malloc(sizeof(t_cola));
	colaDescartesCaught->tipoCola = NEW;
	colaDescartesCaught->mensajes = list_create();
	colaDescartesCaught->subs = list_create();

	colaNew = malloc(sizeof(t_cola));
	colaNew->tipoCola = NEW;
	colaNew->mensajes = list_create();
	colaNew->subs = list_create();

	colaAppeared = malloc(sizeof(t_cola));
	colaAppeared->tipoCola = APPEARED;
	colaAppeared->mensajes = list_create();
	colaAppeared->subs = list_create();

	colaGet = malloc(sizeof(t_cola));
	colaGet->tipoCola = GET;
	colaGet->mensajes = list_create();
	colaGet->subs = list_create();

	colaLocalized = malloc(sizeof(t_cola));
	colaLocalized->tipoCola = LOCALIZED;
	colaLocalized->mensajes = list_create();
	colaLocalized->subs = list_create();

	colaCatch = malloc(sizeof(t_cola));
	colaCatch->tipoCola = CATCH;
	colaCatch->mensajes = list_create();
	colaCatch->subs = list_create();

	colaCaught = malloc(sizeof(t_cola));
	colaCaught->tipoCola = CAUGHT;
	colaCaught->mensajes = list_create();
	colaCaught->subs = list_create();
}

void inicializar_semaforos(){
	semNew = malloc(sizeof(sem_t));
	semAppeared = malloc(sizeof(sem_t));
	semGet = malloc(sizeof(sem_t));
	semLocalized = malloc(sizeof(sem_t));
	semCatch = malloc(sizeof(sem_t));
	semCaught = malloc(sizeof(sem_t));
	semId = malloc(sizeof(sem_t));
	semLog = malloc(sizeof(sem_t));
	semCache = malloc(sizeof(sem_t));
	semParticiones = malloc(sizeof(sem_t));
	semNumeroVictima = malloc(sizeof(sem_t));
	semParticionesEliminadas = malloc(sizeof(sem_t));

	semRecibirMensajes = malloc(sizeof(sem_t)); //todo prueba
	sem_init(semRecibirMensajes, 0, 1);




	sem_init(semNew, 0, 1);
	sem_init(semAppeared, 0, 1);
	sem_init(semGet, 0, 1);
	sem_init(semLocalized, 0, 1);
	sem_init(semCatch, 0, 1);
	sem_init(semCaught, 0, 1);
	sem_init(semId, 0, 1);
	sem_init(semLog, 0, 1);
	sem_init(semCache, 0, 1);
	sem_init(semParticiones, 0, 1);
	sem_init(semNumeroVictima, 0, 1);
	sem_init(semParticionesEliminadas, 0, 1);
}

int32_t crear_id(){
	sem_wait(semId);
	id_inicial += 1;
	sem_post(semId);
	return id_inicial;
}

t_sub* crear_sub(uint32_t pId, int32_t elSocket){
	t_sub* nuevo = malloc(sizeof(t_sub));
	nuevo->pID = pId;
	nuevo->elSocket = elSocket;
	nuevo->recibido = 0;
	nuevo->suscripto = 1;
	return nuevo;
}

t_mensaje* crear_mensaje(int32_t id, int32_t id_correlativo, void* mensaje, uint32_t sizeDeMensaje){
	t_mensaje* nuevo = malloc(sizeof(t_mensaje));
	nuevo->id = id;
	nuevo->id_correlativo = id_correlativo;
	nuevo->mensaje = mensaje;
	nuevo->tamanioMensaje = sizeDeMensaje;
	nuevo->subs = list_create();
	return nuevo;
}

//agrega el sub a todos los mensajes de la cola, si no hay mensajes no hace nada
void suscribir(t_sub* sub,t_cola* cola)
{
	int32_t control = 0;
	if(cola->mensajes->head != NULL)
	{
		for(int i = 0;i < cola->mensajes->elements_count; i++)
		{
			t_mensaje* mensaje;
			mensaje = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes

			if(mensaje->subs->head != NULL)
			{
				for(int j = 0;j < mensaje->subs->elements_count; j++)
				{
					t_sub* subDelMensaje;
					subDelMensaje = list_get(mensaje->subs,j);
					if((subDelMensaje->recibido == sub->recibido) && (subDelMensaje->pID == sub->pID) && (subDelMensaje->suscripto == sub->suscripto))
					{
						control++;
					}
				}
			}
			if(control == 0)
			{
				list_add(mensaje->subs,sub);
			}
		}
	}
}

// devuelve un -1 si encuentra un mensaje con el mismo id correlativo en esa cola
int32_t buscar_en_cola(int32_t id_correlativo, t_cola* cola){
	if(cola->mensajes->head != NULL){
		for(int i = 0; i < cola->mensajes->elements_count; i++){
			t_mensaje* mensaje;
			mensaje = list_get(cola->mensajes,i);
			if(mensaje->id_correlativo == id_correlativo){
				return -1;
			}
		}
	}
	return 1;
}

// crea un mensaje con los datos que le pasan y lo agrega a la cola, el mensajes tiene los mismos suscriptores que la cola
void agregar_mensaje_new(New* mensaje, uint32_t sizeMensaje){
	if(buscar_en_cola(mensaje->corrID, colaNew) != -1){
		t_mensaje* new;
		int32_t id = crear_id();
		int32_t idCorr;
		t_sub* sub;
		t_sub* auxiliar;

		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaNew);
		new = crear_mensaje(id,idCorr,mensaje, sizeMensaje);
		if(colaNew->subs->head != NULL)
		{
			for(int j = 0; j < colaNew->subs->elements_count; j++)
			{ //avanza hasta el final de la cola de subs
				auxiliar = malloc(sizeof(t_sub));
				sub = list_get(colaNew->subs,j); // busca el j elemento de la lista subs
				auxiliar->pID = sub->pID;
				auxiliar->suscripto = sub->suscripto;
				auxiliar->recibido = 0;
				auxiliar->elSocket = sub->elSocket;
				list_add(new->subs, auxiliar);
			}

		}
		list_add(colaNew->mensajes,new);
		sem_wait(semLog);
		log_info(logger, "Se agrego un mensaje a la cola New");
		sem_post(semLog);
	}
}

void agregar_mensaje_appeared(Appeared* mensaje, uint32_t sizeMensaje){
	if(buscar_en_cola(mensaje->corrID, colaAppeared) != -1){
		t_mensaje* new;
		int32_t id = crear_id();
		int32_t idCorr;
		t_sub* sub;
		t_sub* auxiliar;

		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaAppeared);
		new = crear_mensaje(id,idCorr,mensaje, sizeMensaje);
		if(colaAppeared->subs->head != NULL)
		{
			for(int j = 0; j < colaAppeared->subs->elements_count; j++)
			{ //avanza hasta el final de la cola de subs
				auxiliar = malloc(sizeof(t_sub));
				sub = list_get(colaAppeared->subs,j); // busca el j elemento de la lista subs
				auxiliar->pID = sub->pID;
				auxiliar->suscripto = sub->suscripto;
				auxiliar->recibido = 0;
				auxiliar->elSocket = sub->elSocket;
				list_add(new->subs, auxiliar);
			}

		}
		list_add(colaAppeared->mensajes,new);
		sem_wait(semLog);
		log_info(logger, "Se agrego un mensaje a la cola Appeared");
		sem_post(semLog);
	}
}

void agregar_mensaje_get(Get* mensaje, uint32_t sizeMensaje){
	if(buscar_en_cola(mensaje->corrID, colaGet) != -1){
		t_mensaje* new;
		int32_t id = crear_id();
		int32_t idCorr;
		t_sub* sub;
		t_sub* auxiliar;

		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaGet);
		new = crear_mensaje(id,idCorr,mensaje, sizeMensaje);
		if(colaGet->subs->head != NULL)
		{
			for(int j = 0; j < colaGet->subs->elements_count; j++)
			{ //avanza hasta el final de la cola de subs
				auxiliar = malloc(sizeof(t_sub));
				sub = list_get(colaGet->subs,j); // busca el j elemento de la lista subs
				auxiliar->pID = sub->pID;
				auxiliar->suscripto = sub->suscripto;
				auxiliar->recibido = 0;
				auxiliar->elSocket = sub->elSocket;
				list_add(new->subs, auxiliar);
			}

		}
		list_add(colaGet->mensajes,new);
		sem_wait(semLog);
		log_info(logger, "Se agrego un mensaje a la cola Get");
		sem_post(semLog);
	}
}

void agregar_mensaje_localized(Localized* mensaje, uint32_t sizeMensaje){
	if(buscar_en_cola(mensaje->corrID, colaLocalized) != -1){
		t_mensaje* new;
		int32_t id = crear_id();
		int32_t idCorr;
		t_sub* sub;
		t_sub* auxiliar;

		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaLocalized);
		new = crear_mensaje(id,idCorr,mensaje, sizeMensaje);
		if(colaLocalized->subs->head != NULL)
		{
			for(int j = 0; j < colaLocalized->subs->elements_count; j++)
			{ //avanza hasta el final de la cola de subs
				auxiliar = malloc(sizeof(t_sub));
				sub = list_get(colaLocalized->subs,j); // busca el j elemento de la lista subs
				auxiliar->pID = sub->pID;
				auxiliar->suscripto = sub->suscripto;
				auxiliar->recibido = 0;
				auxiliar->elSocket = sub->elSocket;
				list_add(new->subs, auxiliar);
			}

		}
		list_add(colaLocalized->mensajes,new);
		sem_wait(semLog);
		log_info(logger, "Se agrego un mensaje a la cola Localized");
		sem_post(semLog);
	}
}

void agregar_mensaje_catch(Catch* mensaje, uint32_t sizeMensaje, idMensaje* idCatch){
	if(buscar_en_cola(mensaje->corrID, colaCatch) != -1){
		t_mensaje* new;
		int32_t id = crear_id();
		int32_t idCorr;
		t_sub* sub;
		t_sub* auxiliar;

		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		idCatch->id_mensaje = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaCatch);
		new = crear_mensaje(id,idCorr,mensaje, sizeMensaje);
		if(colaCatch->subs->head != NULL)
		{
			for(int j = 0; j < colaCatch->subs->elements_count; j++)
			{ //avanza hasta el final de la cola de subs
				auxiliar = malloc(sizeof(t_sub));
				sub = list_get(colaCatch->subs,j); // busca el j elemento de la lista subs
				auxiliar->pID = sub->pID;
				auxiliar->suscripto = sub->suscripto;
				auxiliar->recibido = 0;
				auxiliar->elSocket = sub->elSocket;
				list_add(new->subs, auxiliar);
			}

		}
		list_add(colaCatch->mensajes,new);
		sem_wait(semLog);
		log_info(logger, "Se agrego un mensaje a la cola Catch");
		sem_post(semLog);
	}
}

void agregar_mensaje_caught(Caught* mensaje, uint32_t sizeMensaje){
	if(buscar_en_cola(mensaje->corrID, colaCaught) != -1){
		t_mensaje* new;
		int32_t id = crear_id();
		int32_t idCorr;
		t_sub* sub;
		t_sub* auxiliar;

		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaCaught);
		new = crear_mensaje(id,idCorr,mensaje, sizeMensaje);
		if(colaCaught->subs->head != NULL)
		{
			for(int j = 0; j < colaCaught->subs->elements_count; j++)
			{ //avanza hasta el final de la cola de subs
				auxiliar = malloc(sizeof(t_sub));
				sub = list_get(colaCaught->subs,j); // busca el j elemento de la lista subs
				auxiliar->pID = sub->pID;
				auxiliar->suscripto = sub->suscripto;
				auxiliar->recibido = 0;
				auxiliar->elSocket = sub->elSocket;
				list_add(new->subs, auxiliar);
			}

		}
		list_add(colaCaught->mensajes,new);
		sem_wait(semLog);
		log_info(logger, "Se agrego un mensaje a la cola Caught");
		sem_post(semLog);
	}
}

void agregar_sub(uint32_t pId, t_cola* cola, int32_t elSocket){
	t_sub* new;
	new = crear_sub(pId, elSocket);
	list_add(cola->subs,new);
	agregar_mensajes_viejos(pId,cola);
	suscribir(new, cola);
	mandar_mensajes_broker(cola);
}

// agrega los mensajes de la cola de descartes a la cola donde hay un nuevo sub
void agregar_mensajes_viejos(uint32_t pId, t_cola* cola){
	switch(cola->tipoCola){
	case NEW:
		buscar_mensajes_descarte(pId, cola, colaDescartesNew);
		break;
	case APPEARED:
		buscar_mensajes_descarte(pId, cola, colaDescartesAppeared);
		break;
	case GET:
		buscar_mensajes_descarte(pId, cola, colaDescartesGet);
		break;
	case LOCALIZED:
		buscar_mensajes_descarte(pId, cola, colaDescartesLocalized);
		break;
	case CATCH:
		buscar_mensajes_descarte(pId, cola, colaDescartesCatch);
		break;
	case CAUGHT:
		buscar_mensajes_descarte(pId, cola, colaDescartesCaught);
		break;
	default:
		break;
	}
}

// busca los mensajes de la lista de descartes que no se hayan mandado al suscriptor y los agrega a la cola
void buscar_mensajes_descarte(uint32_t pId, t_cola* cola, t_cola* descarte){
	if(descarte->mensajes->head != NULL){
		for(int i = 0; i < descarte->mensajes->elements_count; i++){
			t_mensaje* mensaje;
			mensaje = list_get(colaDescartesNew->mensajes,i);
					if(sub_presente(pId, mensaje) == 0){
						t_mensaje* mensajeViejo;
						mensajeViejo = list_remove(descarte->mensajes, i);
						list_add(cola->mensajes, mensajeViejo);
			}
		}
	}
}

int sub_presente(uint32_t pId, t_mensaje* mensaje){
	if(mensaje->subs->head != NULL){
		for(int j = 0; j < mensaje->subs->elements_count; j++){
			t_sub* sub;
			sub = list_get(mensaje->subs,j);
			if(sub->pID == pId){
				return 1;
			}
		}
	}
	return 0;
}

//manda todos mensajes sin leer de una cola, si no hay mensajes no hace nada
void mandar_mensajes_broker(t_cola* cola){
	t_sub* sub;
	int contadorSubs;
	if(cola->mensajes->head != NULL && cola->subs->head != NULL){
		for(int i = 0; i < cola->mensajes->elements_count; i++){ //avanza hasta el final de la cola de mensajes
			t_mensaje* mensaje;
			mensaje = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
			if(sacar_mensaje_de_Cache(CACHE, hoja_de_particiones, mensaje->mensaje ,mensaje->id , cola->tipoCola, &NUMERO_VICTIMA, ALGOR_REEMPLAZO, semNumeroVictima, semCache) == 0)
			{
				mensaje = list_remove(cola->mensajes,i);
				liberar_estructuras(mensaje->mensaje, cola->tipoCola);

				contadorSubs = (mensaje->subs->elements_count) - 1;
				while(contadorSubs >= 0){
					sub = list_remove(mensaje->subs,contadorSubs);
					//todo problema aca



					//free(sub);
					contadorSubs--;
				}
				free(mensaje);
				i--;
			}
			else //si no lo acabo de borrar de memoria, trato de mandarlo
			{
				if(mensaje->subs->head != NULL){
					for(int j = 0; j < mensaje->subs->elements_count; j++){ //avanza hasta el final de la cola de subs
						sub = list_get(mensaje->subs,j); // busca el j elemento de la lista subs
						if(sub->recibido != 1 && sub->suscripto == 1){
							sem_wait(semLog);
							log_info(logger, "Envio un mensaje a uno de los suscriptores");
							sem_post(semLog);
							mandar_mensaje(mensaje->mensaje,cola->tipoCola,sub->elSocket);
						}
						else
						{
							log_info(logger,"Este mensaje no se envia porque fue confirmado");
						}
					}
				borrar_datos(cola,mensaje);
				}
			}
		}
	}
}

void borrar_datos(t_cola* cola, t_mensaje* mensaje){
	switch(cola->tipoCola){
	case NEW:
		borrar_datos_new(mensaje->mensaje);
		break;
	case APPEARED:
		borrar_datos_appeared(mensaje->mensaje);
		break;
	case GET:
		borrar_datos_get(mensaje->mensaje);
		break;
	case LOCALIZED:
		borrar_datos_localized(mensaje->mensaje);
		break;
	case CATCH:
		borrar_datos_catch(mensaje->mensaje);
		break;
	case CAUGHT:
		borrar_datos_caught(mensaje->mensaje);
		break;
	case TEST: //Estos 6 están solo para que no sale el WARNING, no sirven para nada aca
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
	default:
		break;
	}
}

void borrar_datos_new(New* mensaje){
	mensaje->cantPokemon = 0;
	mensaje->largoNombre = 100;
	mensaje->nombrePokemon = "aca no hay nada papu";
	mensaje->posPokemon.x = 0;
	mensaje->posPokemon.y = 0;
}

void borrar_datos_appeared(Appeared* mensaje){
	mensaje->largoNombre = 100;
	mensaje->nombrePokemon = "aca no hay nada papu";
	mensaje->posPokemon.x = 100;
	mensaje->posPokemon.y = 100;
}

void borrar_datos_get(Get* mensaje){
	mensaje->largoNombre = 100;
	mensaje->nombrePokemon = "aca no hay nada papu";
}

void borrar_datos_localized(Localized* mensaje){
	mensaje->largoNombre = 100;
	mensaje->nombrePokemon = "aca no hay nada papu";

	if(mensaje->cantPosciciones == 0)
	{
		mensaje->coords[0] = 0;
		mensaje->coords[1] = 0;
	}

	else
	{
		uint32_t iterador = 0;
		for(; iterador <= (mensaje->cantPosciciones * 2)-1 ;iterador++)
		{
			mensaje->coords[iterador] = 0;
		}
	}
	mensaje->cantPosciciones = 0;
}

void borrar_datos_catch(Catch* mensaje){
	mensaje->largoNombre = 100;
	mensaje->nombrePokemon = "aca no hay nada papu";
	mensaje->posPokemon.x = 100;
	mensaje->posPokemon.y = 100;
}

void borrar_datos_caught(Caught* mensaje){
	mensaje->largoNombre = 100;
	mensaje->nombrePokemon = "aca no hay nada papu";
	mensaje->pudoAtrapar = -100;
}

// avanza en la cola de mensajes hasta encontrar el mensaje con el id deseado, despues busca en la lista de
// suscriptores de ese mensaje hasta encontrar el socket adecuado y pone en visto el mensaje
int modificar_cola(t_cola* cola, int32_t id_mensaje, uint32_t pID){
	for(int i = 0; i < cola->mensajes->elements_count; i++){
		t_mensaje* mensaje;
		mensaje = list_get(cola->mensajes,i);

		if(mensaje->id == id_mensaje){
			for(int j = 0; j < mensaje->subs->elements_count; j++){

				t_sub* sub;
				sub = list_get(mensaje->subs,j);

				if(sub->pID == pID){
					sub->recibido = 1;
					return 1;
				}
			}
		}
	}
	return 0;
}

// usa modificar_cola para confirmar el mensaje deseado (confirmar_cola esta arriba)
void confirmar_mensaje(confirmacionMensaje* mensaje){
	int confirmador;
	switch(mensaje->colaMensajes){
	case NEW:
		sem_wait(semNew);
		confirmador = modificar_cola(colaNew,mensaje->id_mensaje,mensaje->pId);
		sem_wait(semLog);
		if(confirmador == 1){
			log_info(logger, "Se confirmo un mensaje de la cola New");
		}
		sem_post(semLog);
		sem_post(semNew);
		break;
	case APPEARED:
		sem_wait(semAppeared);
		confirmador = modificar_cola(colaAppeared,mensaje->id_mensaje,mensaje->pId);
		sem_wait(semLog);
		if(confirmador == 1){
		log_info(logger, "Se confirmo un mensaje de la cola Appeared");
		}
		sem_post(semLog);
		sem_post(semAppeared);
		break;
	case GET:
		sem_wait(semGet);
		confirmador = modificar_cola(colaGet,mensaje->id_mensaje,mensaje->pId);
		sem_wait(semLog);
		if(confirmador == 1){
		log_info(logger, "Se confirmo un mensaje de la cola Get");
		}
		sem_post(semLog);
		sem_post(semGet);
		break;
	case LOCALIZED:
		sem_wait(semLocalized);
		confirmador = modificar_cola(colaLocalized,mensaje->id_mensaje,mensaje->pId);
		sem_wait(semLog);
		if(confirmador == 1){
		log_info(logger, "Se confirmo un mensaje de la cola Localized");
		}
		sem_post(semLog);
		sem_post(semLocalized);
		break;
	case CATCH:
		sem_wait(semCatch);
		confirmador = modificar_cola(colaCatch,mensaje->id_mensaje,mensaje->pId);
		sem_wait(semLog);
		if(confirmador == 1){
		log_info(logger, "Se confirmo un mensaje de la cola Catch");
		}
		sem_post(semLog);
		sem_post(semCatch);
		break;
	case CAUGHT:
		sem_wait(semCaught);
		confirmador = modificar_cola(colaCaught,mensaje->id_mensaje,mensaje->pId);
		sem_wait(semLog);
		if(confirmador == 1){
		log_info(logger, "Se confirmo un mensaje de la cola Caught");
		}
		sem_post(semLog);
		sem_post(semCaught);
		break;
	case TEST: //Estos 6 están solo para que no sale el WARNING, no sirven para nada aca
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
	default:
		break;
	}
}

// revisar cuando hay que borrar un mensaje
void borrar_mensajes(t_cola* cola){
	int32_t subsTotales = 0, yaRecibido = 0;
		if(cola->mensajes->head != NULL)
		{
			for(int i = 0; i < cola->mensajes->elements_count; i++)
			{ //avanza hasta el final de la cola de mensajes
				t_mensaje* mensaje;
				mensaje = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
				if(mensaje->subs->head != NULL)
				{
					for(int j = 0; j < mensaje->subs->elements_count; j++)
					{ //avanza hasta el final de la cola de subs
						t_sub* sub;
						sub = list_get(mensaje->subs,j); // busca el j elemento de la lista subs
						if(sub->recibido == 1)
						{
							yaRecibido++;
						}
						subsTotales++;
					}
				}
				if((subsTotales == yaRecibido) && (subsTotales != 0) && (yaRecibido != 0))
				{
					t_mensaje* mensajeBorrado;
					mensajeBorrado = list_remove(cola->mensajes, i);
					agregar_descarte(cola,mensajeBorrado);
				}
			}
		}
}

// agrega los mesnajes borrados de la cola a la cola de descartes
void agregar_descarte (t_cola* cola, t_mensaje* descarte){
	switch(cola->tipoCola){
	case NEW:
		list_add(colaDescartesNew->mensajes,descarte);
		break;
	case APPEARED:
		list_add(colaDescartesAppeared->mensajes,descarte);
		break;
	case GET:
		list_add(colaDescartesGet->mensajes,descarte);
		break;
	case LOCALIZED:
		list_add(colaDescartesLocalized->mensajes,descarte);
		break;
	case CATCH:
		list_add(colaDescartesCatch->mensajes,descarte);
		break;
	case CAUGHT:
		list_add(colaDescartesCaught->mensajes,descarte);
		break;
	default:
		break;
	}
}

// primero recorre la lista de mensajes hasta encontrar el sub deseado, lo elimina de la lista y sigue buscando en los
// demas mensajes, cuando termina con eso elimina al sub de la lista de subs de la cola
void desuscribir(uint32_t pID, t_cola* cola){
	t_sub* sub;
	for(int i = 0; i < cola->mensajes->elements_count; i++){ //avanza hasta el final de la cola de mensajes
		t_mensaje* mensaje;// = malloc(sizeof(t_mensaje));
		mensaje = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
		for(int j = 0; j < mensaje->subs->elements_count; j++){ //avanza hasta el final de la cola de subs
			sub = list_get(mensaje->subs,j); // busca el j elemento de la lista subs
			if(sub->pID == pID){
				sub->suscripto = 0;
			}
		}
	}
	for(int j = 0; j < cola->subs->elements_count; j++ ){ //avanza hasta el final de la cola subs
		sub = list_get(cola->subs,j); // busca el j elemento de la lista subs
		if(sub->pID == pID){
			sub->suscripto = 0;
		}
	}
}

//esto es para que arranque el server y se quede escuchando mensajes.

/* dependiendo del codigo de operacion hace diferentes cosas
 * si es el numero de las colas es que estas mandando un mensaje a esas colas y tiene que agregarse
 * si es un tipo SUSCRIPCION te suscribe a la cola que se desea
 */

void process_request(codigo_operacion cod_op, int32_t socket_cliente, uint32_t sizeAAllocar) {
	uint32_t sizeMensajeParaCache;
	idMensaje* idCatch;
	New* mensajeNew;
	Appeared* mensajeAppeared;
	Get* mensajeGet;
	Localized* mensajeLocalized;
	Catch* mensajeCatch;
	Caught* mensajeCaught;
	Suscripcion* mensajeSuscrip;
	Dessuscripcion* mensajeDessuscrip;
	confirmacionMensaje* mensajeConfirmacion;
		switch (cod_op) {
		case NEW:
			mensajeNew = malloc(sizeAAllocar);
			recibir_mensaje(mensajeNew, cod_op, socket_cliente);
			sem_wait(semNew);
			sizeMensajeParaCache = calcular_bytes_utiles_de_mensaje(mensajeNew, cod_op);
			agregar_mensaje_new(mensajeNew,sizeAAllocar);
			agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, TAMANIO_MIN_PART, ADMIN_MEMORIA, hoja_de_particiones, ALGOR_ASIGN_PARTICION, mensajeNew, sizeMensajeParaCache, cod_op, &NUMERO_VICTIMA, FRECUEN_COMPACT, &PARTICIONES_ELIMINADAS, logger, semLog, semCache, semParticiones, semNumeroVictima, semParticionesEliminadas);
			mandar_mensajes_broker(colaNew);
			sem_post(semNew);
			break;
		case APPEARED:
			mensajeAppeared = malloc(sizeAAllocar);
			recibir_mensaje(mensajeAppeared, cod_op, socket_cliente);
			sem_wait(semAppeared);
			sizeMensajeParaCache = calcular_bytes_utiles_de_mensaje(mensajeAppeared, cod_op);
			agregar_mensaje_appeared(mensajeAppeared,sizeAAllocar);
			agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, TAMANIO_MIN_PART, ADMIN_MEMORIA, hoja_de_particiones, ALGOR_ASIGN_PARTICION, mensajeAppeared, sizeMensajeParaCache, cod_op, &NUMERO_VICTIMA, FRECUEN_COMPACT, &PARTICIONES_ELIMINADAS, logger, semLog, semCache, semParticiones, semNumeroVictima, semParticionesEliminadas);
			mandar_mensajes_broker(colaAppeared);
			sem_post(semAppeared);
			break;
		case GET:
			mensajeGet = malloc(sizeAAllocar);
			recibir_mensaje(mensajeGet, cod_op, socket_cliente);
			sem_wait(semGet);
			sizeMensajeParaCache = calcular_bytes_utiles_de_mensaje(mensajeGet, cod_op);
			agregar_mensaje_get(mensajeGet,sizeAAllocar);
			agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, TAMANIO_MIN_PART, ADMIN_MEMORIA, hoja_de_particiones, ALGOR_ASIGN_PARTICION, mensajeGet, sizeMensajeParaCache, cod_op, &NUMERO_VICTIMA, FRECUEN_COMPACT, &PARTICIONES_ELIMINADAS, logger, semLog, semCache, semParticiones, semNumeroVictima, semParticionesEliminadas);
			mandar_mensajes_broker(colaGet);
			sem_post(semGet);
			break;
		case LOCALIZED:
			mensajeLocalized = malloc(sizeAAllocar);
			recibir_mensaje(mensajeLocalized, cod_op, socket_cliente);
			sem_wait(semLocalized);
			sizeMensajeParaCache = calcular_bytes_utiles_de_mensaje(mensajeLocalized, cod_op);
			agregar_mensaje_localized(mensajeLocalized,sizeAAllocar);
			agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, TAMANIO_MIN_PART, ADMIN_MEMORIA, hoja_de_particiones, ALGOR_ASIGN_PARTICION, mensajeLocalized, sizeMensajeParaCache, cod_op, &NUMERO_VICTIMA, FRECUEN_COMPACT, &PARTICIONES_ELIMINADAS, logger, semLog, semCache, semParticiones, semNumeroVictima, semParticionesEliminadas);
			mandar_mensajes_broker(colaLocalized);
			sem_post(semLocalized);
			break;
		case CATCH:
			mensajeCatch = malloc(sizeAAllocar);
			idCatch = malloc(sizeof(idMensaje));
			recibir_mensaje(mensajeCatch, cod_op, socket_cliente);
			sem_wait(semCatch);
			sizeMensajeParaCache = calcular_bytes_utiles_de_mensaje(mensajeCatch, cod_op);
			agregar_mensaje_catch(mensajeCatch,sizeAAllocar,idCatch);
			agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, TAMANIO_MIN_PART, ADMIN_MEMORIA, hoja_de_particiones, ALGOR_ASIGN_PARTICION, mensajeCatch, sizeMensajeParaCache, cod_op, &NUMERO_VICTIMA, FRECUEN_COMPACT, &PARTICIONES_ELIMINADAS, logger, semLog, semCache, semParticiones, semNumeroVictima, semParticionesEliminadas);
			mandar_mensaje(idCatch,IDMENSAJE,socket_cliente);
			mandar_mensajes_broker(colaCatch);
			sem_post(semCatch);
			break;
		case CAUGHT:
			mensajeCaught = malloc(sizeAAllocar);
			recibir_mensaje(mensajeCaught, cod_op, socket_cliente);
			sem_wait(semCaught);
			sizeMensajeParaCache = calcular_bytes_utiles_de_mensaje(mensajeCaught, cod_op);
			agregar_mensaje_caught(mensajeCaught,sizeAAllocar);
			agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, TAMANIO_MIN_PART, ADMIN_MEMORIA, hoja_de_particiones, ALGOR_ASIGN_PARTICION, mensajeCaught, sizeMensajeParaCache, cod_op, &NUMERO_VICTIMA, FRECUEN_COMPACT, &PARTICIONES_ELIMINADAS, logger, semLog, semCache, semParticiones, semNumeroVictima, semParticionesEliminadas);
			mandar_mensajes_broker(colaCaught);
			sem_post(semCaught);
			break;
		case SUSCRIPCION:
			mensajeSuscrip= malloc(sizeAAllocar);
			recibir_mensaje(mensajeSuscrip, cod_op, socket_cliente);
			printf("La PID que me llego en suscripcion: %u\n",mensajeSuscrip->pId);
			switch(mensajeSuscrip->numeroCola){
			case NEW:
				sem_wait(semLog);
				log_info(logger, "Se suscribio a la cola New");
				sem_post(semLog);
				sem_wait(semNew);
				agregar_sub(mensajeSuscrip->pId, colaNew, socket_cliente);
				sem_post(semNew);
				break;
			case APPEARED:
				sem_wait(semLog);
				log_info(logger, "Se suscribio a la cola Appeared");
				sem_post(semLog);
				sem_wait(semAppeared);
				agregar_sub(mensajeSuscrip->pId, colaAppeared, socket_cliente);
				sem_post(semAppeared);
				break;
			case GET:
				sem_wait(semLog);
				log_info(logger, "Se suscribio a la cola Get");
				sem_post(semLog);
				sem_wait(semGet);
				agregar_sub(mensajeSuscrip->pId, colaGet, socket_cliente);
				sem_post(semGet);
				break;
			case LOCALIZED:
				sem_wait(semLog);
				log_info(logger, "Se suscribio a la cola Localized");
				sem_post(semLog);
				sem_wait(semLocalized);
				agregar_sub(mensajeSuscrip->pId, colaLocalized, socket_cliente);
				sem_post(semLocalized);
				break;
			case CATCH:
				sem_wait(semLog);
				log_info(logger, "Se suscribio a la cola Catch");
				sem_post(semLog);
				sem_wait(semCatch);
				agregar_sub(mensajeSuscrip->pId, colaCatch, socket_cliente);
				sem_post(semCatch);
				break;
			case CAUGHT:
				sem_wait(semLog);
				log_info(logger, "Se suscribio a la cola Caught");
				sem_post(semLog);
				sem_wait(semCaught);
				agregar_sub(mensajeSuscrip->pId, colaCaught, socket_cliente);
				sem_post(semCaught);
				break;
			default:
				break;
			}
			free(mensajeSuscrip);
			break;
		case DESSUSCRIPCION:
			mensajeDessuscrip = malloc(sizeAAllocar);
			recibir_mensaje(mensajeDessuscrip, cod_op, socket_cliente);
			switch(mensajeDessuscrip->numeroCola){
			case NEW:
				sem_wait(semNew);
				desuscribir(mensajeDessuscrip->pId, colaNew);
				sem_post(semNew);
				sem_wait(semLog);
				log_info(logger, "Se desuscribio de la cola New");
				sem_post(semLog);
				break;
			case APPEARED:
				sem_wait(semAppeared);
				desuscribir(mensajeDessuscrip->pId, colaAppeared);
				sem_post(semAppeared);
				sem_wait(semLog);
				log_info(logger, "Se desuscribio de la cola Appeared");
				sem_post(semLog);
				break;
			case GET:
				sem_wait(semGet);
				desuscribir(mensajeDessuscrip->pId, colaGet);
				sem_post(semGet);
				sem_wait(semLog);
				log_info(logger, "Se desuscribio de la cola Get");
				sem_post(semLog);
				break;
			case LOCALIZED:
				sem_wait(semLocalized);
				desuscribir(mensajeDessuscrip->pId, colaLocalized);
				sem_post(semLocalized);
				sem_wait(semLog);
				log_info(logger, "Se desuscribio de la cola Localized");
				sem_post(semLog);
				break;
			case CATCH:
				sem_wait(semCatch);
				desuscribir(mensajeDessuscrip->pId, colaCatch);
				sem_post(semCatch);
				sem_wait(semLog);
				log_info(logger, "Se desuscribio de la cola Catch");
				sem_post(semLog);
				break;
			case CAUGHT:
				sem_wait(semCaught);
				desuscribir(mensajeDessuscrip->pId, colaCaught);
				sem_post(semCaught);
				sem_wait(semLog);
				log_info(logger, "Se desuscribio de la cola Caught");
				sem_post(semLog);
				break;
			default:
				break;
			}
			free(mensajeDessuscrip);
			break;
		case CONFIRMACION:
			mensajeConfirmacion = malloc(sizeAAllocar);
			recibir_mensaje(mensajeConfirmacion, cod_op, socket_cliente);
			printf("La ID que me llego en confirmacion: %u\n",mensajeConfirmacion->pId);
			confirmar_mensaje(mensajeConfirmacion);// los semaforos estan aca
			break;
		case TEST:
			break;
		case DESCONEXION:
			pthread_exit(NULL);
		case ERROR:
			pthread_exit(NULL);
		default:
			pthread_exit(NULL);
			break;
		}
}

void serve_client(int32_t* socket)
{
	while(1){
		int32_t sizeAAllocar;
		codigo_operacion cod_op;
		int32_t recibidosSize = 0;

		int32_t recibidos = recv(*socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
		bytesRecibidos(recibidos);

		if(recibidos >= 1)
		{
			sem_wait(semRecibirMensajes);//todo prueba

			recibidosSize = recv(*socket, &sizeAAllocar, sizeof(sizeAAllocar), MSG_WAITALL); //saca el tamaño de lo que sigue en el buffer
			bytesRecibidos(recibidosSize);
			printf("Tamaño de lo que sigue en el buffer: %u.\n", sizeAAllocar);

			process_request(cod_op, *socket, sizeAAllocar);

			sem_post(semRecibirMensajes);//todo Prueba
		}


		else
		{
			pthread_exit(NULL);
		}

		recibidosSize = 0;
		recibidos = 0;
	}
}

void esperar_cliente(int32_t socket_servidor)
{
	struct sockaddr_in dir_cliente;

	socklen_t tam_direccion = sizeof(struct sockaddr_in);

	int32_t socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);
	sem_wait(semLog);
	log_info(logger, "Un proceso se conecto");
	sem_post(semLog);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);
}

void iniciar_server(char* ip, char* puerto)
{
	int socket_servidor;
	int activo = 1;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(ip, puerto, &hints, &servinfo);
    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        //para que pueda reusar el socket si se cae
        setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &activo,sizeof(activo));

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);
    freeaddrinfo(servinfo);

    while(1)
    {
    	esperar_cliente(socket_servidor);
    }

}

void mostrameMemoria(int32_t senial)
{
	if(senial == SIGUSR1 || senial == SIGUSR2)
	{
		sem_wait(semLog);
		log_info(logger, "Dump de la Cache solicitada\n");
		sem_post(semLog);
		revision_lista_particiones(CACHE, hoja_de_particiones, TAMANIO_MEM, dumpCache);
	}
	else
	{
		puts("Señal incorrecta, abortando...");
		abort();
	}
}
