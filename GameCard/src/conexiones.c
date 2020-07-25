#include "conexiones.h"

// Hilo de escucha de gameboy
void comenzarEscuchaGameBoy(){
	int32_t socketoide = reservarSocket("5001"); //tirarle la key de la config

	escuchoSocket(socketoide); //escuchando al gameboy
	close(socketoide);
}

// Hilo de escucha de broker
void comenzarConexionConBroker(datosHiloBroker* datos){

	// CONEXION Y ESPERAR MENSAJES DE BROKER
	char* IP_BROKER = datos->IP_BROKER;
	char* PUERTO_BROKER = datos->PUERTO_BROKER;
	int TIEM_REIN_CONEXION = datos->TIEM_REIN_CONEXION;

	t_log* logger = datos->logger;

	socketNew = conectarseABroker(logger, NEW);
	socketCatch = conectarseABroker(logger, CATCH);
	socketGet = conectarseABroker(logger, GET);

	while (socketNew == -1 || socketCatch == -1 || socketGet == -1){
		sleep(TIEM_REIN_CONEXION);
		socketNew = conectarseABroker(logger, NEW);
		socketCatch = conectarseABroker(logger, CATCH);
		socketGet = conectarseABroker(logger, GET);
	}

	printf("Socket New: %i\n", socketNew);
	printf("Socket Catch: %i\n", socketCatch);
	printf("Socket Get: %i\n", socketGet);

	while(1)
	{
	esperarMensajes(socketGet, IP_BROKER, PUERTO_BROKER, logger, TIEM_REIN_CONEXION, GET);
	}

	puts("sali de esperar mensaje");

}

void serve_client(int32_t* socket)
{
//	codigo_operacion cod_op;
//	int32_t recibidos = recv(*socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
//	bytesRecibidos(recibidos);
//	if(recibidos == -1)
//	{
//		cod_op = -1;
//	}
//
//	process_request(cod_op, *socket);

	while(1){
		int32_t sizeAAllocar;
		codigo_operacion cod_op;
		int32_t recibidosSize = 0;

		int32_t recibidos = recv(*socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
		bytesRecibidos(recibidos);

		if(recibidos >= 1)
		{
			recibidosSize = recv(*socket, &sizeAAllocar, sizeof(sizeAAllocar), MSG_WAITALL); //saca el tamaño de lo que sigue en el buffer
			bytesRecibidos(recibidosSize);
			printf("Tamaño de lo que sigue en el buffer: %u.\n", sizeAAllocar);
		}

		if(recibidos < 1  || recibidosSize < 1)
		{
			cod_op = -1;
			sizeAAllocar = 0;
		}

		process_request(cod_op, *socket, sizeAAllocar);
		recibidosSize = 0;
		recibidos = 0;
	}
}

void esperarMensajes(int socket, char* IP_BROKER, char* PUERTO_BROKER, t_log* logger, int TIEM_REIN_CONEXION, codigo_operacion nombreCola){

	codigo_operacion cod_op;
	uint32_t desconexion = 1;

	int32_t recibidosCodOP = recv(socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
	bytesRecibidos(recibidosCodOP);

	int32_t sizeAAllocar;

	//en caso de que haya fallado la conexion del COD OP
	while( (recibidosCodOP == -1) || (desconexion == -1) )
	{
		sleep(TIEM_REIN_CONEXION);
		desconexion = conectarseABroker(logger, nombreCola);
		if(desconexion != -1)
		{
			recibidosCodOP = recv(socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
			bytesRecibidos(recibidosCodOP);
		}
	}

	int32_t recibidosSize = recv(socket, &sizeAAllocar, sizeof(sizeAAllocar), MSG_WAITALL); //saca el tamaño de lo que sigue en el buffer
	bytesRecibidos(recibidosSize);

	printf("Tamaño de lo que sigue en el buffer: %u.\n", sizeAAllocar);

	//en caso de que haya fallado la conexion de la variable SIZE
	while((recibidosSize == -1) || (desconexion == -1))
	{
		sleep(TIEM_REIN_CONEXION);
		desconexion = conectarseABroker(logger, nombreCola);
		if(desconexion != -1)
		{
			recibidosSize = recv(socket, &sizeAAllocar, sizeof(sizeAAllocar), MSG_WAITALL); //saca el tamaño de lo que sigue en el buffer
			bytesRecibidos(recibidosSize);
		}
	}

	switch (cod_op)
	{
	 	case NEW: ;
			New* mensajeNewRecibido = malloc(sizeAAllocar);
			recibir_mensaje(mensajeNewRecibido, cod_op, socket);

			printf("Termine de recibir mensaje new sin explotar\n");

			// cola = 1;

//			typedef struct {
//				int32_t id_mensaje;
//				int colaMensajes;
//			}confirmacionMensaje;

//			confirmacionMensaje* ackBroker = malloc(sizeAAllocar);
//
//			ackBroker->colaMensajes = NEW;
//			ackBroker->id_mensaje = mensajeNewRecibido->ID;
//
//			// TODO : Explota al tratar de mandar este mensaje
//			mandar_mensaje(ackBroker, CONFIRMACION, socket);

			//printf("Termine de mandar ack sin explotar\n");

			char* pokemon = mensajeNewRecibido->nombrePokemon;
			int posX = mensajeNewRecibido->posPokemon.x;
			int posY = mensajeNewRecibido->posPokemon.y;
			int cantidad = mensajeNewRecibido->cantPokemon;

			int IDMensaje = mensajeNewRecibido->ID;

			mensajeNew( pokemon , posX, posY, cantidad, IDMensaje );

			break;
		case GET: ;
			Get* mensajeGetRecibido = malloc(sizeAAllocar);
			recibir_mensaje(mensajeGetRecibido, cod_op, socket);

			printf("Termine de recibir mensaje new sin explotar\n");

			mensajeGet(mensajeGetRecibido->nombrePokemon, mensajeGetRecibido->ID);

			break;
		case CATCH: ;
			Catch* mensajeCatchRecibido = malloc(sizeAAllocar);
			recibir_mensaje(mensajeCatchRecibido, cod_op, socket);

			printf("Termine de recibir mensaje new sin explotar\n");

			break;
		default:
			printf("No deberia entrar aca D:");
			break;
	}

}

void escuchoSocket(int32_t miSocket)
{
	// Acepto conexiones entrantes
	//struct sockaddr_in direccionConexionEntrante;
	//uint32_t tamanioConexionEntrante;
	//int32_t conexionEntrante = accept(miSocket, (void*) &direccionConexionEntrante, &tamanioConexionEntrante);

	//printf ("Me llego una conexion (GameBoy): %i", conexionEntrante);

	while(1)
	{
		esperar_conexiones(miSocket);
	}
}

void esperar_conexiones(int32_t socket_servidor)
{
	struct sockaddr_in dir_cliente;

	// Entero lindo para el socket (es un int)
	socklen_t tam_direccion = sizeof(struct sockaddr_in);

	int32_t socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);//espera una conexion

	serve_client(&socket_cliente);
}

int conectarseABroker(t_log* logger, codigo_operacion nombreCola){
	int socket = establecer_conexion(IP_BROKER, PUERTO_BROKER);//creo conexión con el Broker.

	resultado_de_conexion(socket, logger, "BROKER");

	if (socket != -1)
	{
		suscribirseAUnaCola(socket, nombreCola);
	}

	return socket;
}

// Cuando llega mensaje de gameboy
void process_request(codigo_operacion cod_op, int32_t socket_cliente, int32_t tamanioDatos) {
	New* mensajeNew;
	Get* mensajeGet;
	Catch* mensajeCatch;

		switch (cod_op) {
		case NEW:
			mensajeNew  = malloc(tamanioDatos);
			recibir_mensaje(mensajeNew, cod_op, socket_cliente);

			//ya te llegaron los datos y llamas a tus funciones

			printf("Termine de recibir un mensaje NEW\n");

			break;
		case GET:
			mensajeGet = malloc(tamanioDatos);
			recibir_mensaje(mensajeGet, cod_op, socket_cliente);
			printf("Termine de recibir un mensaje GET\n");

			//ya te llegaron los datos y llamas a tus funciones

			break;
		case CATCH:
			mensajeCatch = malloc(tamanioDatos);
			recibir_mensaje(mensajeCatch, cod_op, socket_cliente);

			printf("Termine de recibir un mensaje CATCH\n");

			//ya te llegaron los datos y llamas a tus funciones

			break;
		default:
			printf("Nunca deberia estar recibiendo un mensaje que no sea New Get o Catch");
			break;
		}
}

void suscribirseAUnaCola(int32_t socket, codigo_operacion nombreCola){

	//Uso una estructura para guardar el numero de cola al que me quiero subscribir y mandarlo a la funcion mandar_mensaje
	Suscripcion* estructuraSuscribirse = malloc(sizeof(Suscripcion));

	estructuraSuscribirse->numeroCola = nombreCola;
	estructuraSuscribirse->pId = getpid();

	//mandamos el mensaje pidiendo suscribirse a la cola
	mandar_mensaje(estructuraSuscribirse, SUSCRIPCION, socket);

	free(estructuraSuscribirse);
}

int escucharGameBoy(char* IP_GAMECARD, char* PUERTO_GAMECARD, t_log* logger){
	int socket = establecer_conexion(IP_GAMECARD, PUERTO_GAMECARD);//creo conexión con el Broker.

	resultado_de_conexion(socket, logger, "SOCKET_ESCUCHA");

	return socket;
}
