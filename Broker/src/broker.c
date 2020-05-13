#include "Broker.h"


typedef struct Entrenador{

	// Temporal | Num Pokedex
	int* listaPokemones;

	struct Entrenador* siguiente;
	struct Entrenador* anterior;

}Entrenador;

t_sub crear_sub(int socket){
	t_sub nuevo;
	nuevo.sub = socket;
	nuevo.recibido = 0;
	return nuevo;
}

t_mensaje crear_mensaje(int id,char* mensaje){
	t_mensaje nuevo;
	nuevo.id = id;
	nuevo.mensaje = malloc(sizeof(char));
	nuevo.mensaje = mensaje;
	nuevo.subs = list_create();
	return nuevo;
}

t_cola crear_cola(char* nombre){
	t_cola nuevo;
	nuevo.nombre = nombre;
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
void agregar_mensaje(char* mensaje, t_cola cola){
	t_mensaje* new = malloc(sizeof(t_mensaje));
	*new = crear_mensaje(TEST_ID,mensaje);
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

int main(void) {

	t_log* logger;
	t_config* config;
	int socket;
	char* IP_BROKER;
	char* PUERTO_BROKER;

	//Dejo cargado un logger para loguear los eventos.
	logger = cargarUnLog("Logs/broker.log", "broker");

	//Cargo las configuraciones del .config
	config = leerConfiguracion("../Configs/Broker.config");

	IP_BROKER = config_get_string_value(config,"IP_BROKER");
	PUERTO_BROKER = config_get_string_value(config,"PUERTO_BROKER");

	//Arranco el Broker como servidor.
	iniciar_server(IP_BROKER, PUERTO_BROKER);

	printf("hola");

	return EXIT_SUCCESS;
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


