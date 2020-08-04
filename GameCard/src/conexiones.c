#include "conexiones.h"

// Hilo de escucha de gameboy
void comenzarEscuchaGameBoy(){

	printf("Puerto Gamecard:%s.\n", PUERTO_GAMECARD);

	// El PUERTO_GAMECARD se lee de la config
	int32_t socketoide = reservarSocket(PUERTO_GAMECARD);

	listen(socketoide, SOMAXCONN);

	while(1)
	{
		esperar_conexiones(socketoide);
	}
	close(socketoide);
}

// Hilo de escucha de broker
void comenzarConexionConBroker(datosHiloBroker* datos)
{
	// CONEXION Y ESPERAR MENSAJES DE BROKER
	//semaforo suscripcion y demas
	semConexionBroker= malloc(sizeof(sem_t));
	sem_init(semConexionBroker, 0, 1);

	int TIEM_REIN_CONEXION = datos->TIEM_REIN_CONEXION;

	t_log* logger = datos->logger;

	socketNew = conectarseABroker(logger, NEW);
	socketCatch = conectarseABroker(logger, CATCH);
	socketGet = conectarseABroker(logger, GET);

	while (socketNew <= 0)
	{
		sleep(TIEM_REIN_CONEXION);
		socketNew = conectarseABroker(logger, NEW);
	}

	while (socketCatch <= 0)
	{
		sleep(TIEM_REIN_CONEXION);
		socketCatch = conectarseABroker(logger, CATCH);
	}

	while(socketGet <= 0)
	{
		sleep(TIEM_REIN_CONEXION);
		socketGet = conectarseABroker(logger, GET);
	}

	printf("Socket New: %i\n", socketNew);
	printf("Socket Catch: %i\n", socketCatch);
	printf("Socket Get: %i\n", socketGet);

	pthread_t hiloGet;
	pthread_t hiloNew;
	pthread_t hiloCatch;

	datosHiloColas datosGet = {socketGet, GET};
	datosHiloColas datosNew = {socketNew, NEW};
	datosHiloColas datosCatch = {socketCatch, CATCH};

	// Levanto hilos para escuchar colas (cada uno tiene un while)
	pthread_create(&hiloGet  , NULL, (void*)esperarMensajes, &datosGet);
	pthread_create(&hiloNew  , NULL, (void*)esperarMensajes, &datosNew);
	pthread_create(&hiloCatch, NULL, (void*)esperarMensajes, &datosCatch);

	pthread_join(hiloGet, NULL);
	pthread_join(hiloNew, NULL);
	pthread_join(hiloCatch, NULL);

	puts("sali de esperar mensaje (No tengo que llegar nunca)");
}

void serve_client(int32_t* socket)
{
	int32_t sizeAAllocar = 0;
	codigo_operacion cod_op;
	int32_t recibidosSize = 0;
	int32_t recibidos = 0;

	recibidos = recv(*socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
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

void esperarMensajes(datosHiloColas* datosHiloColas){

	codigo_operacion nombreCola = datosHiloColas->cola;
	int socketCola = datosHiloColas->socket;

	codigo_operacion cod_op;
	uint32_t desconexion = 1;
	int32_t sizeAAllocar;
	int32_t recibidosCodOP = 0;
	int32_t recibidosSize = 0;
	uint32_t pID = getpid();
	confirmacionMensaje* mensajeConfirm;
	int32_t socketAck = 0;

	while(1)
	{
		printf("Esperando mensajes de la cola %i.\n", nombreCola);
		recibidosCodOP = recv(socketCola, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
		bytesRecibidos(recibidosCodOP);
		printf("Recibi un mensaje de la cola %i.\n", nombreCola);

		//en caso de que haya fallado la conexion del COD OP
		while((recibidosCodOP == -1) || (recibidosCodOP == 0) || (desconexion == -1))
		{
			sleep(TIEM_REIN_CONEXION);

			sem_wait(semConexionBroker);
			desconexion = conectarseABroker(logger, nombreCola);
			sem_post(semConexionBroker);
			socketCola = desconexion;
			if((desconexion != -1) || (desconexion != 0))
			{
				recibidosCodOP = recv(socketCola, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
				bytesRecibidos(recibidosCodOP);
			}
		}

		recibidosSize = recv(socketCola, &sizeAAllocar, sizeof(sizeAAllocar), MSG_WAITALL); //saca el tamaño de lo que sigue en el buffer
		bytesRecibidos(recibidosSize);

		printf("Tamaño de lo que sigue en el buffer: %u.\n", sizeAAllocar);

		//en caso de que haya fallado la conexion de la variable SIZE
		while((recibidosSize == -1) || (desconexion == -1))
		{
			sleep(TIEM_REIN_CONEXION);
			sem_wait(semConexionBroker);
			desconexion = conectarseABroker(logger, nombreCola);
			sem_post(semConexionBroker);
			socketCola = desconexion;
			if((desconexion != -1) || (desconexion != 0))
			{
				recibidosSize = recv(socketCola, &sizeAAllocar, sizeof(sizeAAllocar), MSG_WAITALL); //saca el tamaño de lo que sigue en el buffer
				bytesRecibidos(recibidosSize);
			}
		}

		switch (cod_op)
		{
			case NEW: ;
				New* mensajeNewRecibido = malloc(sizeAAllocar);
				recibir_mensaje(mensajeNewRecibido, cod_op, socketCola);

				//mandamos confirmacion para no volver a recibir este mensaje
				mensajeConfirm = malloc(sizeof(confirmacionMensaje));
				mensajeConfirm->id_mensaje = mensajeNewRecibido->ID;
				mensajeConfirm->colaMensajes = cod_op;
				mensajeConfirm->pId = pID;
				socketAck = establecer_conexion(IP_BROKER, PUERTO_BROKER);
				mandar_mensaje(mensajeConfirm, CONFIRMACION, socketAck);
				cerrar_conexion(socketAck);
				free(mensajeConfirm);
				sleep(1);

				char* pokemon = mensajeNewRecibido->nombrePokemon;
				int posX = mensajeNewRecibido->posPokemon.x;
				int posY = mensajeNewRecibido->posPokemon.y;
				int cantidad = mensajeNewRecibido->cantPokemon;

				int IDMensaje = mensajeNewRecibido->ID;

				mensajeNew( pokemon , posX, posY, cantidad, IDMensaje );

				free(mensajeNewRecibido->nombrePokemon);
				free(mensajeNewRecibido);

				break;

			case GET: ;
				Get* mensajeGetRecibido = malloc(sizeAAllocar);
				recibir_mensaje(mensajeGetRecibido, cod_op, socketCola);

				//mandamos confirmacion para no volver a recibir este mensaje
				mensajeConfirm = malloc(sizeof(confirmacionMensaje));
				mensajeConfirm->id_mensaje = mensajeGetRecibido->ID;
				mensajeConfirm->colaMensajes = cod_op;
				mensajeConfirm->pId = pID;
				socketAck = establecer_conexion(IP_BROKER, PUERTO_BROKER);
				mandar_mensaje(mensajeConfirm, CONFIRMACION, socketAck);
				cerrar_conexion(socketAck);
				free(mensajeConfirm);
				sleep(1);

				mensajeGet(mensajeGetRecibido->nombrePokemon, mensajeGetRecibido->ID);

				free(mensajeGetRecibido->nombrePokemon);
				free(mensajeGetRecibido);

			break;
				case CATCH: ;
				Catch* mensajeCatchRecibido = malloc(sizeAAllocar);
				recibir_mensaje(mensajeCatchRecibido, cod_op, socketCola);

				//mandamos confirmacion para no volver a recibir este mensaje
				mensajeConfirm = malloc(sizeof(confirmacionMensaje));
				mensajeConfirm->id_mensaje = mensajeCatchRecibido->ID;
				mensajeConfirm->colaMensajes = cod_op;
				mensajeConfirm->pId = pID;
				socketAck = establecer_conexion(IP_BROKER, PUERTO_BROKER);
				mandar_mensaje(mensajeConfirm, CONFIRMACION, socketAck);
				cerrar_conexion(socketAck);
				free(mensajeConfirm);
				sleep(1);

				// Corrigiendo... Todo lo corregiste?
				mensajeCatch(mensajeCatchRecibido->nombrePokemon, mensajeCatchRecibido->posPokemon.x, mensajeCatchRecibido->posPokemon.y, mensajeCatchRecibido->ID);

				free(mensajeCatchRecibido->nombrePokemon);
				free(mensajeCatchRecibido);
			break;
			default:
				printf("No deberia entrar aca D:");
				break;
		}
	}
}

void esperar_conexiones(int32_t socket_servidor)
{
	struct sockaddr_in dir_cliente;

	//es un int
	socklen_t tam_direccion = sizeof(struct sockaddr_in);
	//espera una conexion
	int32_t socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	if(socket_cliente >= 1)
	{
		serve_client(&socket_cliente);
	}
}

int conectarseABroker(t_log* logger, codigo_operacion nombreCola){
	int socket = establecer_conexion(IP_BROKER, PUERTO_BROKER);//creo conexión con el Broker.

	resultado_de_conexion(socket, logger, "BROKER");

	if ((socket != -1) && (socket != 0))
	{
		//sem_wait(semConexionBroker);
		sleep(1);
		suscribirseAUnaCola(socket, nombreCola);
		//sem_post(semConexionBroker);
	}

	return socket;
}

// Cuando llega mensaje de gameboy
void process_request(codigo_operacion cod_op, int32_t socket_cliente, int32_t tamanioDatos)
{

	New* mensajeNewRecibido;
	Get* mensajeGetRecibido;
	Catch* mensajeCatchRecibido;

	switch (cod_op)
	{
	case NEW:
		mensajeNewRecibido = malloc(tamanioDatos);
		recibir_mensaje(mensajeNewRecibido, cod_op, socket_cliente);

		//ya te llegaron los datos y llamas a tus funciones ToDO (esto no lo tenias echo Nacho, te lo agregue yo, lo tenes que revisar)
		mensajeNew(mensajeNewRecibido->nombrePokemon, mensajeNewRecibido->posPokemon.x, mensajeNewRecibido->posPokemon.y, mensajeNewRecibido->cantPokemon, mensajeNewRecibido->ID);

		free(mensajeNewRecibido->nombrePokemon);
		free(mensajeNewRecibido);
		break;

	case GET:
		mensajeGetRecibido = malloc(tamanioDatos);
		recibir_mensaje(mensajeGetRecibido, cod_op, socket_cliente);

		//ya te llegaron los datos y llamas a tus funciones toDo(esto no lo tenias echo Nacho, te lo agregue yo, lo tenes que revisar)
		mensajeGet(mensajeGetRecibido->nombrePokemon, mensajeGetRecibido->ID);

		free(mensajeGetRecibido->nombrePokemon);
		free(mensajeGetRecibido);
		break;

	case CATCH:
		mensajeCatchRecibido = malloc(tamanioDatos);
		recibir_mensaje(mensajeCatchRecibido, cod_op, socket_cliente);

		//ya te llegaron los datos y llamas a tus funciones toDo(esto no lo tenias echo Nacho, te lo agregue yo, lo tenes que revisar)
		mensajeCatch(mensajeCatchRecibido->nombrePokemon, mensajeCatchRecibido->posPokemon.x, mensajeCatchRecibido->posPokemon.y, mensajeCatchRecibido->ID);

		free(mensajeCatchRecibido->nombrePokemon);
		free(mensajeCatchRecibido);
		break;

	default:
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
	sleep(1);

	free(estructuraSuscribirse);
}

int escucharGameBoy(char* IP_GAMECARD, char* PUERTO_GAMECARD, t_log* logger){
	int socket = establecer_conexion(IP_GAMECARD, PUERTO_GAMECARD);//creo conexión con el Broker.

	resultado_de_conexion(socket, logger, "SOCKET_ESCUCHA");

	return socket;
}
