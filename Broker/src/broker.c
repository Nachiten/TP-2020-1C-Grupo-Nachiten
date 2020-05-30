#include "Broker.h"


typedef struct Entrenador{

	// Temporal | Num Pokedex
	int* listaPokemones;

	struct Entrenador* siguiente;
	struct Entrenador* anterior;

}Entrenador;



// *************************************************
int main(void) {

	t_config* config;
	int socket;
	char* IP_BROKER;
	char* PUERTO_BROKER;

	colaNew.tipoCola = NEW;
	colaAppeared.tipoCola = APPEARED;
	colaGet.tipoCola = GET;
	colaLocalized.tipoCola = LOCALIZED;
	colaCatch.tipoCola = CATCH;
	colaCaught.tipoCola = CAUGHT;

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



t_sub crear_sub(int socket){
	t_sub nuevo;
	nuevo.socket = socket;
	nuevo.recibido = 0;
	return nuevo;
}

t_mensaje crear_mensaje(int id,void* mensaje){
	t_mensaje nuevo;
	nuevo.id = id;
	nuevo.id_correlativo = 0;// a cambiar despues
	nuevo.mensaje = malloc(sizeof(mensaje));
	nuevo.mensaje = mensaje;
	nuevo.subs = list_create();
	return nuevo;
}

t_cola crear_cola(void){
	t_cola nuevo;
	nuevo.mensajes = list_create();
	nuevo.subs = list_create();
	return nuevo;
}

//agrega el sub a todos los mensajes de la cola, si no hay mensajes no hace nada
void suscribir(t_sub* sub,t_cola cola){
	int contador = 0;
	if(cola.mensajes != NULL){
		do {
			t_mensaje* aux = list_get(cola.mensajes,contador);
			list_add(aux->subs,sub);
			contador += 1;
			cola.mensajes->head = cola.mensajes->head->next;
		}while(cola.mensajes->head != NULL);
	}
}

//crea un mensaje de un string y lo agrega a una cola YA CREADA, el mensaje ya tiene los suscriptores que tenga esa cola
void agregar_mensaje(void* mensaje, codigo_operacion tipo_mensaje, t_cola cola){
	t_mensaje* new = malloc(sizeof(t_mensaje));
	*new = crear_mensaje(TEST_ID,mensaje); // cambiar id
	new->subs = cola.subs;
	list_add(cola.mensajes,new);
}

//agrega un suba una cola de mensajes y lo suscribe a los mensajes que tenga
void agregar_sub(int socket, t_cola cola){
	t_sub* new = malloc(sizeof(t_sub));
	*new = crear_sub(socket);
	list_add(cola.subs,new);
	suscribir(new,cola);
}

//manda todos mensajes sin leer de una cola, si no hay mensajes no hace nada
void mandar_mensaje_broker(t_cola cola){
	int n1 = 0, n2 = 0;
	if(cola.mensajes != NULL){
		while(cola.mensajes->head != NULL){ //avanza hasta el final de la cola de mensajes
			t_mensaje* mensaje = list_get(cola.mensajes,n1); // busca el n1 elemento de la lista mensajes
			while(mensaje->subs->head != NULL){ //avanza hasta el final de la cola de subs
				t_sub* sub = list_get(mensaje->subs,n2); // busca el n2 elemento de la lista subs
				if(sub->recibido != 1){
					mandar_mensaje(mensaje->mensaje,cola.tipoCola,sub->socket);
				}
				n2 += 1;
				mensaje->subs->head = mensaje->subs->head->next;
			}
			n1 += 1;
			cola.mensajes->head = cola.mensajes->head->next;
		}
	}
}

//Todo esto es para que arranque el server y se quede escuchando mensajes.

void devolver_mensaje(void* mensaje_recibido, int size, int socket_cliente, codigo_operacion tipoMensaje)
{
	char* mensaje; //esto es para almacenar el mensaje
	memcpy(mensaje, mensaje_recibido,size);//lo almaceno para ya devolverlo como char*

	mandar_mensaje(mensaje, tipoMensaje, socket_cliente);
}
// agregar logs al process_request
/*
t_log* logger;
logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Broker/Logs/broker.log","BROKER");
log_info(logger, "");
*/
void process_request(int cod_op, int socket_cliente) {
	int size;
	void* mensaje;
		switch (cod_op) {
		case APPEARED:
			mensaje = recibir_mensaje(socket_cliente, &size); // aca queda el mensaje sin el cod_op ni el size
			// poner funcion que desarme el paquete del resto del mensaje

			agregar_mensaje(mensaje, cod_op, colaAppeared);
			free(mensaje);
			break;
		case TEST:
			mensaje = recibir_mensaje(socket_cliente, &size); // aca queda el mensaje sin el cod_op ni el size
			devolver_mensaje(mensaje, size, socket_cliente, TEST);
			free(mensaje);
			break;
		case DESCONEXION:
			pthread_exit(NULL);
		case ERROR:
			pthread_exit(NULL);
		}
}

void serve_client(int* socket)
{
	codigo_operacion cod_op;
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request(cod_op, *socket);
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

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

/*
//prueba de agregar un sub a una lista
	int a = 11111;
	char mensaje[] = "hola que tal\0";
	t_sub* test = malloc(sizeof(t_sub));
	*test= crear_sub(a);
	t_list* subs = list_create();
	list_add(subs,test);
	t_sub* sub = list_get(subs,0);
	printf("nombre sub: %i",sub->sub);

//prueba para agregar un mensaje
	int a = 11111;
	char* mensaje = "hola que tal\0";
	t_mensaje test = crear_mensaje(a,mensaje);
	printf("nombre sub: %s",test.mensaje);

//prueba de cola y agregar mensaje
	char* nombre = "TEST";
	char* mensaje = "hola que tal\0";
	t_cola test = crear_cola(nombre);
	agregar_mensaje(mensaje,test);
	t_list* lista = test.mensajes;
	t_mensaje* prueba = list_get(lista,0);
	printf("nombre sub: %s",prueba->mensaje);
 */

/*
t_log* iniciar_logger(void)
{
	t_log *logger = log_create("Logs/broker.log", "Broker", 1, LOG_LEVEL_INFO);

	if(logger == NULL){
		printf("No pude crear el logger\n");
		exit(1);
	}

	return logger;
}
*/


