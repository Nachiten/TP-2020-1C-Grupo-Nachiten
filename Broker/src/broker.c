#include "Broker.h"


typedef struct Entrenador{

	// Temporal | Num Pokedex
	int* listaPokemones;

	struct Entrenador* siguiente;
	struct Entrenador* anterior;

}Entrenador;

// conexion de clientes -----------------------------------------------------------------
void recibir_mensaje_cliente(Hilo* estructura){
	//while(1){
	recibir_mensaje(estructura->mensaje,NEW,estructura->conexion);
	//guardar estructura en una variable global para que la levante otro hilo
	//}
}

void a (){
	t_config* config;
	char* IP_BROKER, PUERTO_BROKER;
	int32_t conexion;
	void* mensaje;
	pthread_t thread;
	Hilo estructura;

	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/Broker.config");

	IP_BROKER = config_get_string_value(config,"IP_BROKER");

	PUERTO_BROKER = config_get_string_value(config,"PUERTO_BROKER");

	conexion = establecer_conexion(IP_BROKER, PUERTO_BROKER);
	estructura.conexion = conexion;

	pthread_create(&thread, NULL, (void*)recibir_mensaje_cliente, &estructura);
	pthread_join(thread,NULL);

	mensaje = estructura.mensaje;
}
//-----------------------------------------------------------------------------------

// *************************************************
int main(void) {

	t_config* config;
	//t_log* logger;
	char* IP_BROKER;
	char* PUERTO_BROKER;

	id_inicial = 0;
	inicializar_colas();
	inicializar_semaforos();

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

	if (pathLogs == NULL){
		printf("No se pudo leer el path del log de la config\n");
		return EXIT_FAILURE;
	} else {
		printf("El path del log fue leido correctamente\n");
	}

	//Dejo cargado un logger para loguear los eventos.
	logger = cargarUnLog(pathLogs, "Broker");

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

//desde aca tengo lo de memoria (ToDo)

	//va para declaraciones
	uint32_t TAMANIO_MEM;
	uint32_t TAMANIO_MIN_PART;
	char* ADMIN_MEMORIA;
	char* ALGOR_REEMPLAZO;
	char* ALGOR_ASIGN_PARTICION;
	uint32_t FRECUEN_COMPACT;
	hoja_de_particiones = malloc(sizeof(lista_particiones));

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
	//IMPORTANTE: el tamaño de memoria que le asignemos viene del archivo de config, en bytes, hay que actualizarlo segun vayamos avanzando con esto
	CACHE = malloc(TAMANIO_MEM);
	puts("Malloc de memoria completo");

	//Preparo la lista de referencias a las particiones dentro de CACHE
	inicializar_lista_particiones(hoja_de_particiones);

	puts("Lista de particiones inicializada.");


	// *************************************************
	//TESTING AGREGAR MENSAJE A CACHE
	codigo_operacion codigoPrueba = 1; //poner aca el tipo de mensaje a probar

	New* pokemonNew = malloc(sizeof(New));

	pokemonNew->nombrePokemon = "pikachu";
	pokemonNew->largoNombre = strlen(pokemonNew->nombrePokemon);
	pokemonNew->posPokemon.x = 1;
	pokemonNew->posPokemon.y = 1;
	pokemonNew->cantPokemon = 1;
	pokemonNew->ID = 30;
	pokemonNew->corrID = 4;


	t_mensaje* mensajePrueba = malloc(sizeof(t_mensaje));
	mensajePrueba->id = pokemonNew->ID;
	mensajePrueba->id_correlativo = pokemonNew->corrID;
	mensajePrueba->mensaje = pokemonNew;

	t_mensaje* mensajePrueba2 = malloc(sizeof(t_mensaje));
	mensajePrueba2->id = pokemonNew->ID;
	mensajePrueba2->id_correlativo = pokemonNew->corrID;
	mensajePrueba2->mensaje = pokemonNew;


	//ToDo atencion, necesito el tamaño del mensaje para asignarle un espacio, hablar con Nico. Harcodeado por ahora
	agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, hoja_de_particiones, ALGOR_ASIGN_PARTICION, mensajePrueba->mensaje, codigoPrueba);






	agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, hoja_de_particiones, ALGOR_ASIGN_PARTICION, mensajePrueba->mensaje, codigoPrueba);
	agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, hoja_de_particiones, ALGOR_ASIGN_PARTICION, mensajePrueba->mensaje, codigoPrueba);


	// *************************************************


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

	sem_init(semNew, 0, 1);
	sem_init(semAppeared, 0, 1);
	sem_init(semGet, 0, 1);
	sem_init(semLocalized, 0, 1);
	sem_init(semCatch, 0, 1);
	sem_init(semCaught, 0, 1);
}

int32_t crear_id(){
	return id_inicial++;
}

void loggear_propio(char* aLogear){
	t_log* logger;
	logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Broker/Logs/brokerPropio.log","BROKER");
	log_info(logger, aLogear);
}

void loggear_obligatorio(char* aLogear){
	t_log* logger;
	logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Broker/Logs/broker.log","BROKER");
	log_info(logger, aLogear);
}

t_sub crear_sub(int32_t socket){
	t_sub nuevo;
	nuevo.socket = socket;
	nuevo.recibido = 0;
	nuevo.suscripto = 1;
	return nuevo;
}

t_mensaje crear_mensaje(int32_t id, int32_t id_correlativo, void* mensaje){
	t_mensaje nuevo;
	nuevo.id = id;
	nuevo.id_correlativo = id_correlativo;
	nuevo.mensaje = mensaje;
	nuevo.subs = list_create();
	return nuevo;
}

//agrega el sub a todos los mensajes de la cola, si no hay mensajes no hace nada
void suscribir(t_sub* sub,t_cola* cola){
	if(cola->mensajes != NULL){
		for(int i = 0;i < cola->mensajes->elements_count; i++) {
			t_mensaje* aux = malloc(sizeof(t_mensaje));
			aux = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
			list_add(aux->subs,sub);
			//free(aux);
		}
	}
}

// devuelve un -1 si encuentra un mensaje con el mismo id correlativo en esa cola
int32_t buscar_en_cola(int32_t id_correlativo, t_cola* cola){
	if(cola->mensajes != NULL){
		for(int i = 0; i < cola->mensajes->elements_count; i++){
			t_mensaje* mensaje = malloc(sizeof(t_mensaje));
			mensaje = list_get(cola->mensajes,i);
			if(mensaje->id_correlativo == id_correlativo){
				//free(mensaje);
				return -1;
			}
			//free(mensaje);
		}
	}
	return 1;
}

// crea un mensaje con los datos que le pasan y lo agrega a la cola, el mensajes tiene los mismos suscriptores que la cola
void agregar_mensaje_new(New* mensaje){
	if(buscar_en_cola(mensaje->corrID, colaNew) != -1){
		t_mensaje* new = malloc(sizeof(t_mensaje));
		int32_t id = crear_id(),idCorr;
		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaNew);
		*new = crear_mensaje(id,idCorr,mensaje);
		new->subs = colaNew->subs;
		list_add(colaNew->mensajes,new);
		mandar_mensajes_broker(colaNew);
	}
}

void agregar_mensaje_appeared(Appeared* mensaje){
	if(buscar_en_cola(mensaje->corrID, colaAppeared) != -1){
		t_mensaje* new = malloc(sizeof(t_mensaje));
		int32_t id = crear_id(),idCorr;
		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaAppeared);
		*new = crear_mensaje(id,idCorr,mensaje);
		new->subs = colaAppeared->subs;
		list_add(colaAppeared->mensajes,new);
		mandar_mensajes_broker(colaAppeared);
	}
}

void agregar_mensaje_get(Get* mensaje){
	if(buscar_en_cola(mensaje->corrID, colaGet) != -1){
		t_mensaje* new = malloc(sizeof(t_mensaje));
		int32_t id = crear_id(),idCorr;
		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaGet);
		*new = crear_mensaje(id,idCorr,mensaje);
		new->subs = colaGet->subs;
		list_add(colaGet->mensajes,new);
		mandar_mensajes_broker(colaGet);
	}
}

void agregar_mensaje_localized(Localized* mensaje){
	if(buscar_en_cola(mensaje->corrID, colaLocalized) != -1){
		t_mensaje* new = malloc(sizeof(t_mensaje));
		int32_t id = crear_id(),idCorr;
		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaLocalized);
		*new = crear_mensaje(id,idCorr,mensaje);
		new->subs = colaLocalized->subs;
		list_add(colaLocalized->mensajes,new);
		mandar_mensajes_broker(colaGet);
	}
}

void agregar_mensaje_catch(Catch* mensaje){
	if(buscar_en_cola(mensaje->corrID, colaCatch) != -1){
		t_mensaje* new = malloc(sizeof(t_mensaje));
		int32_t id = crear_id(),idCorr;
		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaCatch);
		*new = crear_mensaje(id,idCorr,mensaje);
		new->subs = colaCatch->subs;
		list_add(colaCatch->mensajes,new);
		mandar_mensajes_broker(colaCatch);
	}
}

void agregar_mensaje_caught(Caught* mensaje){
	if(buscar_en_cola(mensaje->corrID, colaCaught) != -1){
		t_mensaje* new = malloc(sizeof(t_mensaje));
		int32_t id = crear_id(),idCorr;
		if(mensaje->corrID == -2){
			idCorr = id;
		}else{
			idCorr = mensaje->corrID;
			printf("adentro de agregar mensaje, ID correlativo: %i\n",mensaje->corrID);
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaCaught);
		*new = crear_mensaje(id,idCorr,mensaje);
		new->subs = colaCaught->subs;
		list_add(colaCaught->mensajes,new);
		mandar_mensajes_broker(colaCaught);
	}
}

void agregar_sub(int32_t socket, t_cola* cola){
	t_sub* new = malloc(sizeof(t_sub));
	*new = crear_sub(socket);
	list_add(cola->subs,new);
	suscribir(new,cola);
	mandar_mensajes_broker(cola);
}

//manda todos mensajes sin leer de una cola, si no hay mensajes no hace nada
void mandar_mensajes_broker(t_cola* cola){
	log_info(logger,"estoy mandando mensajes");
	if(cola->mensajes != NULL){
		for(int i = 0; i < cola->mensajes->elements_count; i++){ //avanza hasta el final de la cola de mensajes
			t_mensaje* mensaje = malloc(sizeof(t_mensaje));
			mensaje = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
			for(int j = 0; j < mensaje->subs->elements_count; j++){ //avanza hasta el final de la cola de subs
				t_sub* sub = malloc(sizeof(t_sub));
				sub = list_get(mensaje->subs,j); // busca el j elemento de la lista subs
				if(sub->recibido != 1 && sub->suscripto == 1){
					mandar_mensaje(mensaje->mensaje,cola->tipoCola,sub->socket);
				}
				//free(sub);
			}
//			free(mensaje->mensaje);
//			free(mensaje);
		}
	}
}

// avanza en la cola de mensajes hasta encontrar el mensaje con el id deseado, despues busca en la lista de
// suscriptores de ese mensaje hasta encontrar el socket adecuado y pone en visto el mensaje
void modificar_cola(t_cola* cola, int32_t id_mensaje, int32_t socket){
	for(int i = 0; i < cola->mensajes->elements_count; i++){

		t_mensaje* mensaje = malloc(sizeof(t_mensaje));
		mensaje = list_get(cola->mensajes,i);

		if(mensaje->id == id_mensaje){
			for(int j = 0; j < mensaje->subs->elements_count; j++){

				t_sub* sub = malloc(sizeof(t_sub));
				sub = list_get(mensaje->subs,j);

				if(sub->socket == socket){
					sub->recibido = 1;
					exit(EXIT_SUCCESS);
				}
				//free(sub);
			}
		}
		//free(mensaje);
	}
}

// usa modificar_cola para confirmar el mensaje deseado (confirmar_cola esta arriba)
void confirmar_mensaje(int32_t socket, confirmacionMensaje* mensaje){
	switch(mensaje->colaMensajes){
	case NEW:
		sem_wait(semNew);
		modificar_cola(colaNew,mensaje->id_mensaje,socket);
		sem_post(semNew);
		break;
	case APPEARED:
		sem_wait(semAppeared);
		modificar_cola(colaAppeared,mensaje->id_mensaje,socket);
		sem_post(semAppeared);
		break;
	case GET:
		sem_wait(semGet);
		modificar_cola(colaGet,mensaje->id_mensaje,socket);
		sem_post(semGet);
		break;
	case LOCALIZED:
		sem_wait(semLocalized);
		modificar_cola(colaLocalized,mensaje->id_mensaje,socket);
		sem_post(semLocalized);
		break;
	case CATCH:
		sem_wait(semCatch);
		modificar_cola(colaCatch,mensaje->id_mensaje,socket);
		sem_post(semCatch);
		break;
	case CAUGHT:
		sem_wait(semCaught);
		modificar_cola(colaCaught,mensaje->id_mensaje,socket);
		sem_post(semCaught);
		break;
	}
}

// te devuelve el numero de cola al que se quiere suscribir el cliente
int32_t a_suscribir(Suscripcion* mensaje){
	return mensaje->numeroCola;
}

// te devuelve el numero de cola al que se quiere desuscribir el cliente
int32_t a_desuscribir(Dessuscripcion* mensaje){
	return mensaje->numeroCola;
	printf("Me des suscribi de la cola %i", mensaje->numeroCola);
}

// revisar cuando hay que borrar un mensaje

void borrar_mensajes(t_cola* cola){
	int32_t subsTotales = 0, yaRecibido = 0;
	t_mensaje* borrado;
		if(cola->mensajes != NULL){
			for(int i = 0; i < cola->mensajes->elements_count; i++){ //avanza hasta el final de la cola de mensajes
				t_mensaje* mensaje = malloc(sizeof(t_mensaje));
				mensaje = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
				for(int j = 0; j < mensaje->subs->elements_count; j++){ //avanza hasta el final de la cola de subs
					t_sub* sub = malloc(sizeof(t_sub));
					sub = list_get(mensaje->subs,j); // busca el j elemento de la lista subs
					if(sub->recibido == 1){
						yaRecibido++;
					}
					subsTotales++;
					//free(sub);
				}
				if(subsTotales == yaRecibido){
					// cuando este agregado memoria el mensaje eliminado deberia agregarse ahi
					borrado = malloc(sizeof(t_mensaje));
					borrado = list_remove(cola->mensajes, i);
					//probar
					//agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, hoja_de_particiones, ALGOR_ASIGN_PARTICION, borrado->mensaje, cola->tipoCola);
					//free(mensaje);
				}
				//free(mensaje);
			}
		}
}


// primero recorre la lista de mensajes hasta encontrar el sub deseado, lo elimina de la lista y sigue buscando en los
// demas mensajes, cuando termina con eso elimina al sub de la lista de subs de la cola
void desuscribir(int32_t socket, t_cola* cola){
	//t_sub* aux = malloc(sizeof(t_sub));
	for(int i = 0; i < cola->mensajes->elements_count; i++){ //avanza hasta el final de la cola de mensajes
		t_mensaje* mensaje = malloc(sizeof(t_mensaje));
		mensaje = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
		for(int j = 0; j < mensaje->subs->elements_count; j++){ //avanza hasta el final de la cola de subs
			t_sub* sub = malloc(sizeof(t_sub));
			sub = list_get(mensaje->subs,j); // busca el j elemento de la lista subs
			if(sub->socket == socket){
				sub->suscripto = 0;
			}
			//free(sub);
		}
		//free(mensaje);
	}
	for(int j = 0; j < cola->subs->elements_count; j++ ){ //avanza hasta el final de la cola subs
		t_sub* sub = malloc(sizeof(t_sub));
		sub = list_get(cola->subs,j); // busca el j elemento de la lista subs
		if(sub->socket == socket){
			sub->suscripto = 0;
		}
		//free(sub);
	}
}

//esto es para que arranque el server y se quede escuchando mensajes.

/* dependiendo del codigo de operacion hace diferentes cosas
 * si es el numero de las colas es que estas mandando un mensaje a esas colas y tiene que agregarse
 * si es un tipo SUSCRIPCION te suscribe a la cola que se desea
 */

//todo /agregar un desuscribir despues
void process_request(codigo_operacion cod_op, int32_t socket_cliente) {
	int32_t numeroCola;
	char* aLogear;
	void* mensaje;
	New* mensajeNew;
	Appeared* mensajeAppeared;
	Get* mensajeGet;
	Localized* mensajeLocalized;
	Catch* mensajeCatch;
	Caught* mensajeCaught;
		switch (cod_op) {
		case NEW:
			mensajeNew  = malloc(sizeof(New));
			recibir_mensaje(mensajeNew, cod_op, socket_cliente);
			sem_wait(semNew);
			agregar_mensaje_new(mensajeNew);
			sem_post(semNew);
			//free(mensajeNew); // cuando libero esto????
			break;
		case APPEARED:
			mensajeAppeared = malloc(sizeof(Appeared));
			recibir_mensaje(mensajeAppeared, cod_op, socket_cliente);
			puts("Sali de recibir mensaje"); //agregado para ver si completaba recibir mensaje correctamente
			sem_wait(semAppeared);
			agregar_mensaje_appeared(mensajeAppeared);
			sem_post(semAppeared);
			//free(mensaje);
			break;
		case GET:
			mensajeGet = malloc(sizeof(Get));
			recibir_mensaje(mensajeGet, cod_op, socket_cliente);
			sem_wait(semGet);
			agregar_mensaje_get(mensajeGet);
			puts("Sali de recibir mensaje"); //agregado para ver si completaba recibir mensaje correctamente
			sem_post(semGet);
			//free(mensaje);
			break;
		case LOCALIZED:
			mensajeLocalized = malloc(sizeof(Localized));
			recibir_mensaje(mensajeLocalized, cod_op, socket_cliente);
			sem_wait(semLocalized);
			agregar_mensaje_localized(mensajeLocalized);
			puts("Sali de recibir mensaje"); //agregado para ver si completaba recibir mensaje correctamente
			sem_post(semLocalized);
			//free(mensaje);
			break;
		case CATCH:
			mensajeCatch = malloc(sizeof(Catch));
			recibir_mensaje(mensajeCatch, cod_op, socket_cliente);
			puts("Sali de recibir mensaje"); //agregado para ver si completaba recibir mensaje correctamente
			sem_wait(semCatch);
			agregar_mensaje_catch(mensajeCatch);
			sem_post(semCatch);
			//free(mensaje);
			break;
		case CAUGHT:
			mensajeCaught = malloc(sizeof(Caught));
			recibir_mensaje(mensajeCaught, cod_op, socket_cliente);
			puts("Sali de recibir mensaje"); //agregado para ver si completaba recibir mensaje correctamente
			sem_wait(semCaught);
			agregar_mensaje_caught(mensajeCaught);
			sem_post(semCaught);
			//free(mensaje);
			break;
		case SUSCRIPCION:
			mensaje = malloc(sizeof(Suscripcion));
			recibir_mensaje(mensaje, cod_op, socket_cliente);
			numeroCola = a_suscribir(mensaje);

			switch(numeroCola){
			case NEW:
				sem_wait(semNew);
				agregar_sub(socket_cliente, colaNew);
				sem_post(semNew);
				aLogear = "Se suscribio a la cola New";
				log_info(logger, aLogear);
				break;
			case APPEARED:
				sem_wait(semAppeared);
				agregar_sub(socket_cliente, colaAppeared);
				sem_post(semAppeared);
				aLogear = "Se suscribio a la cola Appeared";
				log_info(logger, aLogear);
				break;
			case GET:
				sem_wait(semGet);
				agregar_sub(socket_cliente, colaGet);
				sem_post(semGet);
				aLogear = "Se suscribio a la cola Get";
				log_info(logger, aLogear);
				break;
			case LOCALIZED:
				sem_wait(semLocalized);
				agregar_sub(socket_cliente, colaLocalized);
				sem_post(semLocalized);
				aLogear = "Se suscribio a la cola Localized";
				log_info(logger, aLogear);
				break;
			case CATCH:
				sem_wait(semCatch);
				agregar_sub(socket_cliente, colaCatch);
				sem_post(semCatch);
				aLogear = "Se suscribio a la cola Catch";
				log_info(logger, aLogear);
				break;
			case CAUGHT:
				sem_wait(semCaught);
				agregar_sub(socket_cliente, colaCaught);
				sem_post(semCaught);
				aLogear = "Se suscribio a la cola Caught";
				log_info(logger, aLogear);
				break;
			}
			free(mensaje);
			break;
		case DESSUSCRIPCION:
			log_info(logger, "llege a desuscribirme");
			mensaje = malloc(sizeof(Dessuscripcion));
			recibir_mensaje(mensaje, cod_op, socket_cliente);

			numeroCola = a_desuscribir(mensaje);

			switch(numeroCola){
			case NEW:
				sem_wait(semNew);
				desuscribir(socket_cliente, colaNew);
				sem_post(semNew);
				break;
			case APPEARED:
				sem_wait(semAppeared);
				desuscribir(socket_cliente, colaAppeared);
				sem_post(semAppeared);
				break;
			case GET:
				sem_wait(semGet);
				desuscribir(socket_cliente, colaGet);
				sem_post(semGet);
				break;
			case LOCALIZED:
				sem_wait(semLocalized);
				desuscribir(socket_cliente, colaLocalized);
				sem_post(semLocalized);
				break;
			case CATCH:
				sem_wait(semCatch);
				desuscribir(socket_cliente, colaCatch);
				sem_post(semCatch);
				break;
			case CAUGHT:
				sem_wait(semCaught);
				desuscribir(socket_cliente, colaCaught);
				sem_post(semCaught);
				break;
			}
			free(mensaje);
			break;
		case CONFIRMACION:
			mensaje = malloc(sizeof(confirmacionMensaje));
			recibir_mensaje(mensaje, cod_op, socket_cliente);
			confirmar_mensaje(socket_cliente, mensaje);// los semaforos estan aca
			break;
		case TEST:
			break;
		case DESCONEXION:
			pthread_exit(NULL);
		case ERROR:
			pthread_exit(NULL);
		}
}

void serve_client(int32_t* socket)
{
	codigo_operacion cod_op;
	int32_t recibidos = recv(*socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
	bytesRecibidos(recibidos);
	if(recibidos == -1)
		cod_op = -1;

	process_request(cod_op, *socket);
}

void esperar_cliente(int32_t socket_servidor)
{
	struct sockaddr_in dir_cliente;

	// Entero lindo para el socket (es un int)
	socklen_t tam_direccion = sizeof(struct sockaddr_in);

	int32_t socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);
	//accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);
}

void iniciar_server(char* ip, char* puerto)
{
	int socket_servidor;

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

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    while(1)
    	esperar_cliente(socket_servidor);
}
