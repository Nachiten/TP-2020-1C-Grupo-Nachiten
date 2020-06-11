#include "Broker.h"


typedef struct Entrenador{

	// Temporal | Num Pokedex
	int* listaPokemones;

	struct Entrenador* siguiente;
	struct Entrenador* anterior;

}Entrenador;



// *************************************************
int32_t main(void) {

	t_config* config;
	//int socket;
	char* IP_BROKER;
	char* PUERTO_BROKER;

	id_inicial = 0;
	inicializar_colas();

	//Cargo las configuraciones del .config
	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/Broker.config");

	if (config == NULL){
		printf("No se pudo leer la config\n");
	} else {
		printf("La config fue leida correctamente\n");
	}

	// Leer path del log
	char* pathLogs = config_get_string_value(config,"LOG_FILE");

	if (pathLogs == NULL){
		printf("No se pudo leer el path del log de la config\n");
	} else {
		printf("El path del log fue leido correctamente\n");
	}

	//Dejo cargado un logger para loguear los eventos.
	//logger = cargarUnLog(pathLogs, "broker");

	IP_BROKER = config_get_string_value(config,"IP_BROKER");
	PUERTO_BROKER = config_get_string_value(config,"PUERTO_BROKER");

	//Arranco el Broker como servidor.
	iniciar_server(IP_BROKER, PUERTO_BROKER);

	return EXIT_SUCCESS;
}
// *************************************************

void inicializar_colas(){
	colaNew->tipoCola = NEW;
	colaNew->mensajes = list_create();
	colaNew->subs = list_create();

	colaAppeared->tipoCola = APPEARED;
	colaAppeared->mensajes = list_create();
	colaAppeared->subs = list_create();

	colaGet->tipoCola = GET;
	colaGet->mensajes = list_create();
	colaGet->subs = list_create();

	colaLocalized->tipoCola = LOCALIZED;
	colaLocalized->mensajes = list_create();
	colaLocalized->subs = list_create();

	colaCatch->tipoCola = CATCH;
	colaCatch->mensajes = list_create();
	colaCatch->subs = list_create();

	colaCaught->tipoCola = CAUGHT;
	colaCaught->mensajes = list_create();
	colaCaught->subs = list_create();
}

int32_t crear_id(){
	return 1;
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

t_cola crear_cola(codigo_operacion codigo){
	t_cola nuevo;
	nuevo.tipoCola = codigo;
	nuevo.mensajes = list_create();
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
			free(aux);
		}
	}
}

// devuelve un -1 si encuentra un mensaje con el mismo id correlativo en esa cola
int32_t buscar_en_cola(int32_t id_correlativo, t_cola cola){
	if(cola.mensajes != NULL){
		for(int i = 0; i < cola.mensajes->elements_count; i++){
			t_mensaje* mensaje = malloc(sizeof(t_mensaje));
			mensaje = list_get(cola.mensajes,i);
			if(mensaje->id_correlativo == id_correlativo){
				free(mensaje);
				return -1;
			}
			free(mensaje);
		}
	}
	return 1;
}

//crea un mensaje de un string y lo agrega a una cola YA CREADA, el mensaje ya tiene los suscriptores que tenga esa cola
void agregar_mensaje(void* mensaje, codigo_operacion tipo_mensaje, t_cola* cola){
	//if(){
		t_mensaje* new = malloc(sizeof(t_mensaje));
		*new = crear_mensaje(TEST_ID,TEST_ID,mensaje); // cambiar id
		new->subs = cola->subs;
		list_add(cola->mensajes,new);
		free(new);
	//}
}

void agregar_sub(int32_t socket, t_cola* cola){
	t_sub* new = malloc(sizeof(t_sub));
	*new = crear_sub(socket);
	list_add(cola->subs,new);
	suscribir(new,cola);
	free(new);
}

//manda todos mensajes sin leer de una cola, si no hay mensajes no hace nada
void mandar_mensaje_broker(t_cola cola){
	if(cola.mensajes != NULL){
		for(int i = 0; i < cola.mensajes->elements_count; i++){ //avanza hasta el final de la cola de mensajes
			t_mensaje* mensaje = malloc(sizeof(t_mensaje));
			mensaje = list_get(cola.mensajes,i); // busca el i elemento de la lista mensajes
			for(int j = 0; j < mensaje->subs->elements_count; j++){ //avanza hasta el final de la cola de subs
				t_sub* sub = malloc(sizeof(t_sub));
				sub = list_get(mensaje->subs,j); // busca el j elemento de la lista subs
				if(sub->recibido != 1){
					mandar_mensaje(mensaje->mensaje,cola.tipoCola,sub->socket);
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
		modificar_cola(colaNew,mensaje->id_mensaje,socket);
		break;
	case APPEARED:
		modificar_cola(colaAppeared,mensaje->id_mensaje,socket);
		break;
	case GET:
		modificar_cola(colaGet,mensaje->id_mensaje,socket);
		break;
	case LOCALIZED:
		modificar_cola(colaLocalized,mensaje->id_mensaje,socket);
		break;
	case CATCH:
		modificar_cola(colaCatch,mensaje->id_mensaje,socket);
		break;
	case CAUGHT:
		modificar_cola(colaCaught,mensaje->id_mensaje,socket);
		break;
	}
}

// te devuelve el numero de cola al que se quiere suscribir el cliente
int32_t a_suscribir(Suscripcion* mensaje){
	return mensaje->numeroCola;
}

// falta terminar, pero deberia funcionar para las pruebas de conexion
// a revisar cuando hay que borrar un mensaje
/*
void borrar_mensajes(t_cola cola){
	int32_t subsTotales = 0, yaRecibido = 0,n = 0, aBorrar[];
		if(cola.mensajes != NULL){
			for(int i = 0; i < cola.mensajes->elements_count; i++){ //avanza hasta el final de la cola de mensajes
				t_mensaje* mensaje = malloc(sizeof(t_mensaje));
				mensaje = list_get(cola.mensajes,i); // busca el i elemento de la lista mensajes
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
					n++;
					aBorrar[n] = i;
				}
				free(mensaje);
			}
		}
}
*/

// primero recorre la lista de mensajes hasta encontrar el sub deseado, lo elimina de la lista y sigue buscando en los
// demas mensajes, cuando termina con eso elimina al sub de la lista de subs de la cola
void desuscribir(int32_t socket, t_cola* cola){
	int numeroSub = -1;
	t_sub* aux = malloc(sizeof(t_sub));
	for(int i = 0; i < cola->mensajes->elements_count; i++){ //avanza hasta el final de la cola de mensajes
		t_mensaje* mensaje = malloc(sizeof(t_mensaje));
		mensaje = list_get(cola->mensajes,i); // busca el i elemento de la lista mensajes
		for(int j = 0; j < mensaje->subs->elements_count; j++){ //avanza hasta el final de la cola de subs
			t_sub* sub = malloc(sizeof(t_sub));
			sub = list_get(mensaje->subs,j); // busca el j elemento de la lista subs
			if(sub->socket == socket){
				numeroSub = j;
			}
			free(sub);
		}
		if(numeroSub != -1){
			aux = list_remove(mensaje->subs, numeroSub);
			free(aux);
		}
		free(mensaje);
	}
	for(int j = 0; j < cola->subs->elements_count; j++ ){ //avanza hasta el final de la cola subs
		t_sub* sub = malloc(sizeof(t_sub));
		sub = list_get(cola->subs,j); // busca el j elemento de la lista subs
		if(sub->socket == socket){
			numeroSub = j;
		}
		free(sub);
	}
	if(numeroSub != -1){
		aux = list_remove(cola->subs, numeroSub);
		free(aux);
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
	uint32_t size;
	int32_t numeroCola;
	void* mensaje;
		switch (cod_op) {
		case NEW:
			mensaje = malloc(sizeof(New));
			recibir_mensaje(&mensaje, socket_cliente, &size);
			agregar_mensaje(mensaje, cod_op, colaNew);
			free(mensaje);
			break;
		case APPEARED:
			mensaje = malloc(sizeof(Appeared));
			recibir_mensaje(&mensaje, socket_cliente, &size);
			agregar_mensaje(mensaje, cod_op, colaAppeared);
			free(mensaje);
			break;
		case GET:
			mensaje = malloc(sizeof(Get));
			recibir_mensaje(&mensaje, socket_cliente, &size);
			agregar_mensaje(mensaje, cod_op, colaGet);
			free(mensaje);
			break;
		case LOCALIZED:
			mensaje = malloc(sizeof(Localized));
			recibir_mensaje(&mensaje, socket_cliente, &size);
			agregar_mensaje(mensaje, cod_op, colaLocalized);
			free(mensaje);
			break;
		case CATCH:
			mensaje = malloc(sizeof(Catch));
			recibir_mensaje(&mensaje, socket_cliente, &size);
			agregar_mensaje(mensaje, cod_op, colaCatch);
			free(mensaje);
			break;
		case CAUGHT:
			mensaje = malloc(sizeof(Caught));
			recibir_mensaje(&mensaje, socket_cliente, &size);
			agregar_mensaje(mensaje, cod_op, colaCaught);
			free(mensaje);
			break;
		case SUSCRIPCION:
			mensaje = malloc(sizeof(Suscripcion));
			recibir_mensaje(&mensaje, socket_cliente, &size);

			numeroCola = a_suscribir(mensaje);

			switch(numeroCola){
			case NEW:
				agregar_sub(socket_cliente, colaNew);
				break;
			case APPEARED:
				agregar_sub(socket_cliente, colaAppeared);
				break;
			case GET:
				agregar_sub(socket_cliente, colaGet);
				break;
			case LOCALIZED:
				agregar_sub(socket_cliente, colaLocalized);
				break;
			case CATCH:
				agregar_sub(socket_cliente, colaCatch);
				break;
			case CAUGHT:
				agregar_sub(socket_cliente, colaCaught);
				break;
			}
			free(mensaje);
			break;
		case DESSUSCRIPCION:
			mensaje = malloc(sizeof(Suscripcion));
			recibir_mensaje(&mensaje, socket_cliente, &size);

			numeroCola = a_suscribir(mensaje);

			switch(numeroCola){
			case NEW:
				desuscribir(socket_cliente, colaNew);
				break;
			case APPEARED:
				desuscribir(socket_cliente, colaAppeared);
				break;
			case GET:
				desuscribir(socket_cliente, colaGet);
				break;
			case LOCALIZED:
				desuscribir(socket_cliente, colaLocalized);
				break;
			case CATCH:
				desuscribir(socket_cliente, colaCatch);
				break;
			case CAUGHT:
				desuscribir(socket_cliente, colaCaught);
				break;
			}
			free(mensaje);
			break;
		case CONFIRMACION:
			mensaje = malloc(sizeof(confirmacionMensaje));
			recibir_mensaje(&mensaje, socket_cliente, &size);
			confirmar_mensaje(socket_cliente, mensaje);
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
