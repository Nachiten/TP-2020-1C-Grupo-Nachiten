#include "Broker.h"


typedef struct Entrenador{

	// Temporal | Num Pokedex
	int* listaPokemones;

	struct Entrenador* siguiente;
	struct Entrenador* anterior;

}Entrenador;

// conexion de clientes -----------------------------------------------------------------
void recibir_mensaje_cliente(Hilo* estructura){
	while(1){
	recibir_mensaje(estructura->mensaje,estructura->conexion);
	//guardar estructura en una variable global para que la levante otro hilo
	}
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
	t_log* logger;
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
		printf("La ip fue leido correctamente\n");
	}

	PUERTO_BROKER = config_get_string_value(config,"PUERTO_BROKER");

	if (PUERTO_BROKER == NULL){
		printf("No se pudo leer el puerto de la config\n");
		return EXIT_FAILURE;
	} else {
		printf("El puerto fue leido correctamente\n");
	}

	// lo comente porque no andaba el codigo con lo de memoria
//desde aca tengo lo de memoria (ToDo)**************************************************************************************************************
/*
	//va para declaraciones
	uint32_t TAMANIO_MEM;
	uint32_t* TAMANIO_MIN_PART;
	char* ADMIN_MEMORIA;
	char* ALGOR_REEMPLAZO;
	char* ALGOR_ASIGN_PARTICION;
	char* FRECUEN_COMPACT;

	//aca cargo las los valores del config para manejar la memoria
	//esta funcion falla pero feo
	//cargar_config_memoria(config, TAMANIO_MEM, TAMANIO_MIN_PART, ADMIN_MEMORIA, ALGOR_REEMPLAZO, ALGOR_ASIGN_PARTICION, FRECUEN_COMPACT);
	TAMANIO_MEM = config_get_int_value(config,"TAMANO_MEMORIA");
	TAMANIO_MIN_PART = config_get_int_value(config,"TAMANO_MINIMO_PARTICION");
	ADMIN_MEMORIA = config_get_string_value(config,"ALGORITMO_MEMORIA");
	ALGOR_REEMPLAZO = config_get_string_value(config,"ALGORITMO_REEMPLAZO");
	ALGOR_ASIGN_PARTICION = config_get_string_value(config,"ALGORITMO_PARTICION_LIBRE");
	FRECUEN_COMPACT = config_get_string_value(config,"FRECUENCIA_COMPACTACION");

	//preparo el espacio de memoria cache
	//IMPORTANTE: el tamaño de memoria que le asignemos viene del archivo de config, en bytes, hay que actualizarlo segun vayamos avanzando con esto
	CACHE = malloc(cambia_a_int(TAMANIO_MEM));

	//Preparo la lista de referencias a las particiones dentro de CACHE
	inicializar_lista_particiones(hoja_de_particiones);

	//borramos toda referencia a particiones dentro de CACHE
	matar_lista_particiones(hoja_de_particiones);

	//no olvidarse de reventar la memoria reservada para CACHE
	free(CACHE);*/
//hasta aca lo de memoria***************************************************************************************************************************

	//Arranco el Broker como servidor.
	iniciar_server(IP_BROKER, PUERTO_BROKER);

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
	nuevo.mensaje = malloc(sizeof(mensaje));
	nuevo.mensaje = mensaje;
	nuevo.subs = list_create();
	free(nuevo.mensaje);
	return nuevo;
}

//agrega el sub a todos los mensajes de la cola, si no hay mensajes no hace nada
void suscribir(t_sub* sub,t_cola* cola){
	if(cola->mensajes != NULL){
		for(int i = 0;i < cola->mensajes->elements_count; i++) {
			t_mensaje* aux = malloc(sizeof(t_mensaje));
			aux = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
			list_add(aux->subs,sub);
			free(aux);
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
				free(mensaje);
				return -1;
			}
			free(mensaje);
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
		free(new);
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
		free(new);
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
		free(new);
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
		free(new);
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
		free(new);
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
		}
		mensaje->ID = id;
		mensaje->corrID = idCorr;
		borrar_mensajes(colaCaught);
		*new = crear_mensaje(id,idCorr,mensaje);
		new->subs = colaCaught->subs;
		list_add(colaCaught->mensajes,new);
		mandar_mensajes_broker(colaCaught);
		free(new);
	}
}

void agregar_sub(int32_t socket, t_cola* cola){
	t_sub* new = malloc(sizeof(t_sub));
	*new = crear_sub(socket);
	list_add(cola->subs,new);
	suscribir(new,cola);
	free(new);
}

//manda todos mensajes sin leer de una cola, si no hay mensajes no hace nada
void mandar_mensajes_broker(t_cola* cola){
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
				free(sub);
			}
			free(mensaje);
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
				free(sub);
			}
		}
		free(mensaje);
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
					free(sub);
				}
				if(subsTotales == yaRecibido){
					// cuando este agregado memoria el mensaje eliminado deberia agregarse ahi
					borrado = malloc(sizeof(t_mensaje));
					borrado = list_remove(cola->mensajes, i);
					//probar
					//agregar_mensaje_a_Cache(CACHE, TAMANIO_MEM, hoja_de_particiones, ALGOR_ASIGN_PARTICION, borrado->mensaje, cola->tipoCola);
					free(mensaje);
				}
				free(mensaje);
			}
		}
}


// primero recorre la lista de mensajes hasta encontrar el sub deseado, lo elimina de la lista y sigue buscando en los
// demas mensajes, cuando termina con eso elimina al sub de la lista de subs de la cola
void desuscribir(int32_t socket, t_cola* cola){
	t_sub* aux = malloc(sizeof(t_sub));
	for(int i = 0; i < cola->mensajes->elements_count; i++){ //avanza hasta el final de la cola de mensajes
		t_mensaje* mensaje = malloc(sizeof(t_mensaje));
		mensaje = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
		for(int j = 0; j < mensaje->subs->elements_count; j++){ //avanza hasta el final de la cola de subs
			t_sub* sub = malloc(sizeof(t_sub));
			sub = list_get(mensaje->subs,j); // busca el j elemento de la lista subs
			if(sub->socket == socket){
				sub->suscripto = 0;
			}
			free(sub);
		}
		free(mensaje);
	}
	for(int j = 0; j < cola->subs->elements_count; j++ ){ //avanza hasta el final de la cola subs
		t_sub* sub = malloc(sizeof(t_sub));
		sub = list_get(cola->subs,j); // busca el j elemento de la lista subs
		if(sub->socket == socket){
			sub->suscripto = 0;
		}
		free(sub);
	}
}

//Todo esto es para que arranque el server y se quede escuchando mensajes.

void devolver_mensaje(void* mensaje_recibido, uint32_t size, int32_t socket_cliente, codigo_operacion tipoMensaje)
{
	char* mensaje = malloc(sizeof(char)); //esto es para almacenar el mensaje
	memcpy(mensaje, mensaje_recibido,size);//lo almaceno para ya devolverlo como char*

	mandar_mensaje(mensaje, tipoMensaje, socket_cliente);
	free(mensaje);
}

/* dependiendo del codigo de operacion hace diferentes cosas
 * si es el numero de las colas es que estas mandando un mensaje a esas colas y tiene que agregarse
 * si es un tipo SUSCRIPCION te suscribe a la cola que se desea
 */

//todo /agregar un desuscribir despues
void process_request(codigo_operacion cod_op, int32_t socket_cliente) {
	int32_t numeroCola;
	void* mensaje;
		switch (cod_op) {
		case NEW:
			mensaje = malloc(sizeof(New));
			recibir_mensaje(&mensaje, socket_cliente);
			sem_wait(semNew);
			agregar_mensaje_new(mensaje);
			sem_post(semNew);
			free(mensaje);
			break;
		case APPEARED:
			mensaje = malloc(sizeof(Appeared));
			recibir_mensaje(&mensaje, socket_cliente);
			sem_wait(semAppeared);
			agregar_mensaje_appeared(mensaje);
			sem_post(semAppeared);
			free(mensaje);
			break;
		case GET:
			mensaje = malloc(sizeof(Get));
			recibir_mensaje(&mensaje, socket_cliente);
			sem_wait(semGet);
			agregar_mensaje_get(mensaje);
			sem_post(semGet);
			free(mensaje);
			break;
		case LOCALIZED:
			mensaje = malloc(sizeof(Localized));
			recibir_mensaje(&mensaje, socket_cliente);
			sem_wait(semLocalized);
			agregar_mensaje_localized(mensaje);
			sem_post(semLocalized);
			free(mensaje);
			break;
		case CATCH:
			mensaje = malloc(sizeof(Catch));
			recibir_mensaje(&mensaje, socket_cliente);
			sem_wait(semCatch);
			agregar_mensaje_catch(mensaje);
			sem_post(semCatch);
			free(mensaje);
			break;
		case CAUGHT:
			mensaje = malloc(sizeof(Caught));
			recibir_mensaje(&mensaje, socket_cliente);
			sem_wait(semCaught);
			agregar_mensaje_caught(mensaje);
			sem_post(semCaught);
			free(mensaje);
			break;
		case SUSCRIPCION:
			mensaje = malloc(sizeof(Suscripcion));
			recibir_mensaje(&mensaje, socket_cliente);

			numeroCola = a_suscribir(mensaje);

			switch(numeroCola){
			case NEW:
				sem_wait(semNew);
				agregar_sub(socket_cliente, colaNew);
				sem_post(semNew);
				break;
			case APPEARED:
				sem_wait(semAppeared);
				agregar_sub(socket_cliente, colaAppeared);
				sem_post(semAppeared);
				break;
			case GET:
				sem_wait(semGet);
				agregar_sub(socket_cliente, colaGet);
				sem_post(semGet);
				break;
			case LOCALIZED:
				sem_wait(semLocalized);
				agregar_sub(socket_cliente, colaLocalized);
				sem_post(semLocalized);
				break;
			case CATCH:
				sem_wait(semCatch);
				agregar_sub(socket_cliente, colaCatch);
				sem_post(semCatch);
				break;
			case CAUGHT:
				sem_wait(semCaught);
				agregar_sub(socket_cliente, colaCaught);
				sem_post(semCaught);
				break;
			}
			free(mensaje);
			break;
		case DESSUSCRIPCION:
			mensaje = malloc(sizeof(Suscripcion));
			recibir_mensaje(&mensaje, socket_cliente);

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
			recibir_mensaje(&mensaje, socket_cliente);
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
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request(cod_op, *socket);
}

void esperar_cliente(int32_t socket_servidor)
{
	struct sockaddr_in dir_cliente;

	// Entero lindo para el socket (es un int)
	socklen_t tam_direccion = sizeof(struct sockaddr_in);

	int32_t socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

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
