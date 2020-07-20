/*
 * socket.c
 *
 *  Created on: 17 abr. 2020
 *      Author: utnso
 */

#include "socket.h"

int32_t reservarSocket(char* miPuerto)
{
	uint32_t miPuertoInt = cambia_a_int(miPuerto); //pasa a int la key del config

	//preparo una direccion con estas configuraciones
	struct sockaddr_in direccionMiSocket;
	direccionMiSocket.sin_family = AF_INET;
	direccionMiSocket.sin_addr.s_addr = INADDR_ANY;
	direccionMiSocket.sin_port = htons(miPuertoInt); //puerto en el que queremos que escuche

	//me armo el socket en el que quiero escuchar
	int32_t miSocket = socket(AF_INET, SOCK_STREAM, 0); //con estos parametros le decimos que use el protocolo TCP/IP

	//este cacho de codigo es para que si se cierra mal el programa, se pueda reutilizar el socket sin problema
	uint32_t verdadero = 1;
	setsockopt(miSocket, SOL_SOCKET, SO_REUSEADDR, &verdadero, sizeof(verdadero));

	//asocio el socket que creamos con la direccion(el puerto) que arme antes
	if(bind(miSocket, (void*) &direccionMiSocket, sizeof(direccionMiSocket)) != 0)
	{
		puts("El socket no se pudo asociar correctamente (Es posible que esté en uso).");
		return 1;
	}

	puts("Socket reservado correctamente.");
	listen(miSocket, SOMAXCONN); //SOMAXCONN podria ser un numero (Nº max de conexiones en la cola), pero el parametro significa que tiene el tamaño maximo de cola

	return miSocket;
}

int32_t establecer_conexion(char* ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int32_t socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
	{
		printf("Error al intentar conectarse.\n");
		socket_cliente = -1;
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

void resultado_de_conexion(int32_t socket, t_log* logger, char* modulo)
{
	if(socket == -1)
	{
	log_warning(logger, "Conexión fallida con socket de %s", modulo);
	}
	else
	{
	log_info(logger, "Conexión establecida con socket de %s", modulo);
	}
}

void cerrar_conexion(int32_t socket)
{
	close(socket);
}

void mandar_mensaje(void* mensaje, codigo_operacion tipoMensaje, int32_t socket)
{
	t_paquete* paquete_por_armar = malloc (sizeof(t_paquete));
	paquete_por_armar->buffer = malloc(sizeof(t_buffer));
	uint32_t size_serializado = 0;

	printf("Voy a mandar un mensaje del tipo %i\n", tipoMensaje);

	//preparo el paquete para mandar
	void* paquete_serializado = serializar_paquete(paquete_por_armar, mensaje, tipoMensaje, &size_serializado);

	//mando el mensaje
	bytesEnviados(send(socket, paquete_serializado, size_serializado, 0));

	//libero los malloc utilizados
	eliminar_paquete(paquete_por_armar);
	free(paquete_serializado);
}

void* serializar_paquete(t_paquete* paquete, void* mensaje, codigo_operacion tipoMensaje, uint32_t* size_serializado)
{
	uint32_t size_ya_armado = 0;
	paquete->codigo_op = tipoMensaje;
	printf("cod op: %i",paquete->codigo_op);

	void* buffer_serializar;//aca se va a guardar el choclo ya armado

	switch(tipoMensaje){
		case NEW:
				paquete->buffer->stream = malloc(sizeof(New) + 26); //+ 25 posiciones para el nombre + el \n
				size_ya_armado = serializar_paquete_new(paquete, mensaje);
			break;

		case APPEARED:
				paquete->buffer->stream = malloc(sizeof(Appeared) + 26); //+ 25 posiciones para el nombre + el \n
				size_ya_armado = serializar_paquete_appeared(paquete, mensaje);
			break;

		case GET:
				paquete->buffer->stream = malloc(sizeof(Get) + 26); //+ 25 posiciones para el nombre + el \n
				size_ya_armado = serializar_paquete_get(paquete, mensaje);
			break;

		case LOCALIZED://esto no lo puedo hacer todavia porque la estructura no esta completa
				paquete->buffer->stream = malloc(sizeof(Localized) + 126); //+ 100 espacios para las posibles coordenadas + 25 posiciones para el nombre + el \n
				size_ya_armado = serializar_paquete_localized(paquete, mensaje);
			break;

		case CATCH:
				paquete->buffer->stream = malloc(sizeof(Catch) + 26); //+ 25 posiciones para el nombre + el \n
				size_ya_armado = serializar_paquete_catch(paquete, mensaje);
			break;

		case CAUGHT:
				paquete->buffer->stream = malloc(sizeof(Caught) + 26); //+ 25 posiciones para el nombre + el \n
				size_ya_armado = serializar_paquete_caught(paquete, mensaje);
			break;

		case TEST:
				size_ya_armado = serializar_paquete_prueba(paquete, mensaje);
			break;

		case SUSCRIPCION:
			paquete->buffer->stream = malloc(sizeof(Suscripcion));
			size_ya_armado = serializar_paquete_suscripcion(paquete, mensaje);
			break;

		case DESSUSCRIPCION:
			paquete->buffer->stream = malloc(sizeof(Dessuscripcion));
			size_ya_armado = serializar_paquete_dessuscripcion(paquete, mensaje);
			break;

		case DESCONEXION://Estos 3 están solo para que no salga el WARNING, no sirven para nada aca
			break;

		case ERROR:
			break;

		case CONFIRMACION:
			break;
	}

	//ahora me preparo para meter en el buffer "posta", el choclo que va a enviar mandar_mensaje
	printf("el size que tiene armado es: %i \n", size_ya_armado);
	buffer_serializar = malloc(size_ya_armado);
	uint32_t desplazamiento = 0;

	//meto en el buffer el tipo de mensaje que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->codigo_op), sizeof(paquete->codigo_op));
	desplazamiento += sizeof(paquete->codigo_op);

	//meto en el buffer el tamaño de lo que meti en el buffer del paquete que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->buffer->size), sizeof(paquete->buffer->size));
	desplazamiento += sizeof(paquete->buffer->size);
	printf("lo que sigue en buffer es: %i \n", paquete->buffer->size);

	//por ultimo meto en el buffer lo que meti en el buffer del paquete
	memcpy(buffer_serializar  + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento += paquete->buffer->size;

	(*size_serializado) = size_ya_armado;
	return buffer_serializar; //devuelvo el mensaje listo para enviar
}

uint32_t serializar_paquete_new(t_paquete* paquete, New* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto el largo del nombre del pokemon
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->largoNombre), sizeof(pokemon->largoNombre));
	desplazamiento += sizeof(pokemon->largoNombre);

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, pokemon->nombrePokemon, pokemon->largoNombre+1);
	desplazamiento += pokemon->largoNombre+1;

	//meto coordenada X de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.x), sizeof(pokemon->posPokemon.x));
	desplazamiento += sizeof(pokemon->posPokemon.x);

	//meto coordenada Y de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.y), sizeof(pokemon->posPokemon.y));
	desplazamiento += sizeof(pokemon->posPokemon.y);

	//meto cantidad de pokemons en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->cantPokemon), sizeof(pokemon->cantPokemon));
	desplazamiento += sizeof(pokemon->cantPokemon);

	//meto la ID de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->ID), sizeof(pokemon->ID));
	desplazamiento += sizeof(pokemon->ID);

	//meto la ID CORRELATIVA de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->corrID), sizeof(pokemon->corrID));
	desplazamiento += sizeof(pokemon->corrID);

	//le meto al size del buffer el tamaño de lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->largoNombre) + (pokemon->largoNombre +1) + sizeof(pokemon->posPokemon.x) + sizeof(pokemon->posPokemon.y) + sizeof(pokemon->cantPokemon) + sizeof(pokemon->ID) + sizeof(pokemon->corrID);

	//el tamaño del mensaje entero es el codigo de operacion + la variable donde me guarde el size del buffer + lo que pesa el buffer
	size = sizeof(codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_appeared(t_paquete* paquete, Appeared* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto el largo del nombre del pokemon
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->largoNombre), sizeof(pokemon->largoNombre));
	desplazamiento += sizeof(pokemon->largoNombre);

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, (pokemon->nombrePokemon), pokemon->largoNombre+1);
	desplazamiento += pokemon->largoNombre+1;

	//meto coordenada X de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.x), sizeof(pokemon->posPokemon.x));
	desplazamiento += sizeof(pokemon->posPokemon.x);

	//meto coordenada Y de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.y), sizeof(pokemon->posPokemon.y));
	desplazamiento += sizeof(pokemon->posPokemon.y);

	//meto la ID de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->ID), sizeof(pokemon->ID));
	desplazamiento += sizeof(pokemon->ID);

	//meto la ID CORRELATIVA de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->corrID), sizeof(pokemon->corrID));
	desplazamiento += sizeof(pokemon->corrID);

	//le meto al size del buffer el tamaño de lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->largoNombre) + (pokemon->largoNombre +1) + sizeof(pokemon->posPokemon.x) + sizeof(pokemon->posPokemon.y) + sizeof(pokemon->ID) + sizeof(pokemon->corrID);

	//el tamaño del mensaje entero es el codigo de operacion + la variable donde me guarde el size del buffer + lo que pesa el buffer
	size = sizeof(codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_get(t_paquete* paquete, Get* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto el largo del nombre del pokemon
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->largoNombre), sizeof(pokemon->largoNombre));
	desplazamiento += sizeof(pokemon->largoNombre);

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, (pokemon->nombrePokemon), pokemon->largoNombre+1);
	desplazamiento += pokemon->largoNombre+1;

	//meto la ID de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->ID), sizeof(pokemon->ID));
	desplazamiento += sizeof(pokemon->ID);

	//meto la ID CORRELATIVA de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->corrID), sizeof(pokemon->corrID));
	desplazamiento += sizeof(pokemon->corrID);

	//le meto al size del buffer el tamaño de lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->largoNombre) + (pokemon->largoNombre +1) + sizeof(pokemon->ID) + sizeof(pokemon->corrID);

	//el tamaño del mensaje entero es el codigo de operacion + la variable donde me guarde el size del buffer + lo que pesa el buffer
	size = sizeof(codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_localized(t_paquete* paquete, Localized* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;
	uint32_t paresDeCoordenadas = 2 * pokemon->cantPosciciones;
	uint32_t iterador = 0;

	//meto cantidad de posiciones donde hay pokemons en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->cantPosciciones), sizeof(pokemon->cantPosciciones));
	desplazamiento += sizeof(pokemon->cantPosciciones);

	//meto pares de coordenadas donde hay pokemons en el buffer del paquete
	while(iterador <= (paresDeCoordenadas - 1))
	{
		memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->coords[iterador]), sizeof(pokemon->coords[iterador]));
		desplazamiento += sizeof(pokemon->coords[iterador]);
		iterador++;
	}

	//meto el largo del nombre del pokemon
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->largoNombre), sizeof(pokemon->largoNombre));
	desplazamiento += sizeof(pokemon->largoNombre);

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, pokemon->nombrePokemon, pokemon->largoNombre+1);
	desplazamiento += pokemon->largoNombre+1;

	//meto la ID de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->ID), sizeof(pokemon->ID));
	desplazamiento += sizeof(pokemon->ID);

	//meto la ID CORRELATIVA de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->corrID), sizeof(pokemon->corrID));
	desplazamiento += sizeof(pokemon->corrID);

	//le meto al size del buffer el tamaño de lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->largoNombre) + (pokemon->largoNombre +1) + sizeof(pokemon->cantPosciciones) + (sizeof(pokemon->coords[0]) * 2 * pokemon->cantPosciciones) + sizeof(pokemon->ID) + sizeof(pokemon->corrID);

	//el tamaño del mensaje entero es el codigo de operacion + la variable donde me guarde el size del buffer + lo que pesa el buffer
	size = sizeof(codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	puts("---------------------------------------------------");
	printf("el largo del nombre del pokemon es: %i\n", pokemon->largoNombre);
	printf("el nombre del pokemon es: %s\n", pokemon->nombrePokemon);
	printf("la cantidad de posiciones es: %u\n", pokemon->cantPosciciones);
	iterador = 0;
	while(iterador < (pokemon->cantPosciciones * 2 - 1))
	{
		printf("posicion en X es: %u\n", pokemon->coords[iterador]);
		iterador++;
		printf("posicion en Y es: %u\n", pokemon->coords[iterador]);
		iterador++;
	}
	printf("la ID del mensaje es: %u\n", pokemon->ID);
	printf("la ID correlativa del mensaje es: %i\n", pokemon->corrID);
	puts("---------------------------------------------------");

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_catch(t_paquete* paquete, Catch* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto el largo del nombre del pokemon
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->largoNombre), sizeof(pokemon->largoNombre));
	desplazamiento += sizeof(pokemon->largoNombre);

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, (pokemon->nombrePokemon), pokemon->largoNombre+1);
	desplazamiento += pokemon->largoNombre+1;

	//meto coordenada X de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.x), sizeof(pokemon->posPokemon.x));
	desplazamiento += sizeof(pokemon->posPokemon.x);

	//meto coordenada Y de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.y), sizeof(pokemon->posPokemon.y));
	desplazamiento += sizeof(pokemon->posPokemon.y);

	//meto la ID de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->ID), sizeof(pokemon->ID));
	desplazamiento += sizeof(pokemon->ID);

	//meto la ID CORRELATIVA de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->corrID), sizeof(pokemon->corrID));
	desplazamiento += sizeof(pokemon->corrID);

	//le meto al size del buffer el tamaño de lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->largoNombre) + (pokemon->largoNombre +1) + sizeof(pokemon->posPokemon.x) + sizeof(pokemon->posPokemon.y) + sizeof(pokemon->ID) + sizeof(pokemon->corrID);

	//el tamaño del mensaje entero es el codigo de operacion + la variable donde me guarde el size del buffer + lo que pesa el buffer
	size = sizeof(codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_caught(t_paquete* paquete, Caught* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto el largo del nombre del pokemon
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->largoNombre), sizeof(pokemon->largoNombre));
	desplazamiento += sizeof(pokemon->largoNombre);

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, (pokemon->nombrePokemon), pokemon->largoNombre+1);
	desplazamiento += pokemon->largoNombre+1;

	//meto el resultado del intento de atrapar al pokemon
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->pudoAtrapar), sizeof(pokemon->pudoAtrapar));
	desplazamiento += sizeof(sizeof(pokemon->pudoAtrapar));

	//meto la ID de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->ID), sizeof(pokemon->ID));
	desplazamiento += sizeof(pokemon->ID);

	//meto la ID CORRELATIVA de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->corrID), sizeof(pokemon->corrID));
	desplazamiento += sizeof(pokemon->corrID);

//	printf("el largo del nombre del pokemon es: %i\n", pokemon->largoNombre); //ToDo borrar esto, estaba para controles
//	printf("el nombre del pokemon es: %s\n", pokemon->nombrePokemon);
//	printf("el resultado de intentar atrapar al pokemon es: %u\n", pokemon->pudoAtrapar);
//	printf("la ID del mensaje es: %u\n", pokemon->ID);
//	printf("la ID correlativa del mensaje es: %i\n", pokemon->corrID);

	//le meto al size del buffer el tamaño de lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->corrID) + sizeof(pokemon->pudoAtrapar);
	paquete->buffer->size = sizeof(pokemon->largoNombre) + (pokemon->largoNombre +1) + sizeof(pokemon->pudoAtrapar) + sizeof(pokemon->ID) + sizeof(pokemon->corrID);

	//el tamaño del mensaje entero es el codigo de operacion + la variable donde me guarde el size del buffer + lo que pesa el buffer
	size = sizeof(codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_prueba (t_paquete* paquete, char* mensaje)
{
	uint32_t size = 0;
	//siguiente linea es version vieja, esta mal?
	//paquete->buffer->stream = mensaje;

	//(version nueva) meto el mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream, &mensaje, sizeof(mensaje));

	//le meto al size del buffer el tamaño de lo que acabo de meter en el buffer
	paquete->buffer->size = strlen(mensaje)+1;

	//el tamaño del mensaje entero es el codigo de operacion + la variable donde me guarde el size del buffer + lo que pesa el buffer
	size = sizeof(codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}


uint32_t serializar_paquete_suscripcion(t_paquete* paquete, Suscripcion* cola)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto la COLA a la que suscribirse en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(cola->numeroCola), sizeof(cola->numeroCola));
	desplazamiento += sizeof(cola->numeroCola);

	//le meto al size del buffer el tamaño de lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(cola->numeroCola);

	//el tamaño del mensaje entero es el codigo de operacion + la variable donde me guarde el size del buffer + lo que pesa el buffer
	size = sizeof(codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_dessuscripcion(t_paquete* paquete, Dessuscripcion* cola)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto la COLA a de la que dessuscribirse en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(cola->numeroCola), sizeof(cola->numeroCola));
	desplazamiento += sizeof(cola->numeroCola);

	//le meto al size del buffer el tamaño de lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(cola->numeroCola);

	//el tamaño del mensaje entero es el codigo de operacion + la variable donde me guarde el size del buffer + lo que pesa el buffer
	size = sizeof(codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void recibir_mensaje(void* estructura, codigo_operacion tipoMensaje, int32_t socket_cliente, uint32_t* sizeDeLosDatos)
{
	int32_t size;
	bytesRecibidos(recv(socket_cliente, &size, sizeof(size), MSG_WAITALL)); //saca el tamaño de lo que sigue en el buffer

	printf("Tamaño de lo que sigue en el buffer: %u.\n", size);

	*sizeDeLosDatos = size;//Quizas te creas muy inteligente y pienses que esto está al pedo y podriamos pisar el valor de sizeDeLosDatos directamente. No sos tan inteligente y no esta al pedo

	desserializar_mensaje(estructura, tipoMensaje, socket_cliente);
}

void desserializar_mensaje (void* estructura, codigo_operacion tipoMensaje, int32_t socket_cliente)
{
	switch(tipoMensaje)
	{
		case NEW:
			desserializar_new(estructura, socket_cliente);
			break;

		case APPEARED:
			desserializar_appeared(estructura, socket_cliente);
			break;

		case GET:
			desserializar_get(estructura, socket_cliente);
			break;

		case LOCALIZED:
			desserializar_localized(estructura, socket_cliente);
			break;

		case CATCH:
			desserializar_catch(estructura, socket_cliente);
			break;

		case CAUGHT:
			desserializar_caught(estructura, socket_cliente);
			break;

		case TEST:
			break;

		case SUSCRIPCION:
			desserializar_suscripcion(estructura, socket_cliente);
			break;

		case DESSUSCRIPCION:
			desserializar_dessuscripcion(estructura, socket_cliente);
			break;

		case DESCONEXION://Estos 2 están solo para que no salte el WARNING, no sirven para nada aca
			break;

		case ERROR:
			break;

		case CONFIRMACION:

			break;
	}
}

void desserializar_new(New* estructura, int32_t socket_cliente)
{
	//saco el largo del nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->largoNombre), sizeof(estructura->largoNombre), MSG_WAITALL));

	estructura->nombrePokemon = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, estructura->nombrePokemon, estructura->largoNombre+1, MSG_WAITALL));

	//saco pos X
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x), MSG_WAITALL));

	//saco pos Y
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y), MSG_WAITALL));

	//saco cantidad de pokemones
	bytesRecibidos(recv(socket_cliente, &(estructura->cantPokemon), sizeof(estructura->cantPokemon), MSG_WAITALL));

	//saco ID del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->ID), sizeof(estructura->ID), MSG_WAITALL));

	//saco ID CORRELATIVO del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->corrID), sizeof(estructura->corrID), MSG_WAITALL));

	printf("el largo del nombre del pokemon es: %i\n", estructura->largoNombre);
	printf("el nombre del pokemon es: %s\n", estructura->nombrePokemon);
	printf("la pos X es: %u\n", estructura->posPokemon.x);
	printf("la pos Y es: %u\n", estructura->posPokemon.y);
	printf("la cantidad de pokemones es: %u\n", estructura->cantPokemon);
	printf("la ID del mensaje es: %u\n", estructura->ID);
	printf("la ID correlativa del mensaje es: %i\n", estructura->corrID);
}

void desserializar_appeared(Appeared* estructura, int32_t socket_cliente)
{
	//saco el largo del nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->largoNombre), sizeof(estructura->largoNombre), MSG_WAITALL));

	estructura->nombrePokemon = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, estructura->nombrePokemon, estructura->largoNombre+1, MSG_WAITALL));

	//saco pos X
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x), MSG_WAITALL));

	//saco pos Y
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y), MSG_WAITALL));

	//saco ID del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->ID), sizeof(estructura->ID), MSG_WAITALL));

	//saco ID CORRELATIVO del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->corrID), sizeof(estructura->corrID), MSG_WAITALL));

	printf("el largo del nombre del pokemon es: %u\n", estructura->largoNombre);
	printf("el nombre del pokemon es: %s\n", estructura->nombrePokemon);
	printf("la pos X es: %u\n", estructura->posPokemon.x);
	printf("la pos Y es: %u\n", estructura->posPokemon.y);
	printf("la ID del mensaje es: %u\n", estructura->ID);
	printf("la ID correlativa del mensaje es: %i\n", estructura->corrID);
}

void desserializar_get(Get* estructura, int32_t socket_cliente)
{
	//saco el largo del nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->largoNombre), sizeof(estructura->largoNombre), MSG_WAITALL));

	estructura->nombrePokemon = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, estructura->nombrePokemon, estructura->largoNombre+1, MSG_WAITALL));

	//saco ID del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->ID), sizeof(estructura->ID), MSG_WAITALL));

	//saco ID CORRELATIVO del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->corrID), sizeof(estructura->corrID), MSG_WAITALL));

	printf("el largo del nombre del pokemon es: %i\n", estructura->largoNombre);
	printf("el nombre del pokemon es: %s\n", estructura->nombrePokemon);
	printf("la ID del mensaje es: %u\n", estructura->ID);
	printf("la ID correlativa del mensaje es: %i\n", estructura->corrID);
}

void desserializar_localized(Localized* estructura, int32_t socket_cliente)
{
	uint32_t iterador = 0;

	//saco cantidad de posiciones donde hay pokemons
	bytesRecibidos(recv(socket_cliente, &(estructura->cantPosciciones), sizeof(estructura->cantPosciciones), MSG_WAITALL));

	//estructura->coords = malloc(sizeof(estructura->coords[0]) * 2 * estructura->cantPosciciones);
	//estructura->coords[estructura->cantPosciciones];

	//saco pares de coordenadas donde hay pokemons
		while(iterador <= (estructura->cantPosciciones * 2 - 1))
		{
			bytesRecibidos(recv(socket_cliente, &(estructura->coords[iterador]), sizeof(estructura->coords[iterador]), MSG_WAITALL));
			iterador++;
		}

	//saco el largo del nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->largoNombre), sizeof(estructura->largoNombre), MSG_WAITALL));

	estructura->nombrePokemon = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, estructura->nombrePokemon, estructura->largoNombre+1, MSG_WAITALL));

	//saco ID del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->ID), sizeof(estructura->ID), MSG_WAITALL));

	//saco ID CORRELATIVO del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->corrID), sizeof(estructura->corrID), MSG_WAITALL));

	printf("el largo del nombre del pokemon es: %i\n", estructura->largoNombre);
	printf("el nombre del pokemon es: %s\n", estructura->nombrePokemon);
	printf("la cantidad de posiciones es: %u\n", estructura->cantPosciciones);
	iterador = 0;
	while(iterador < (estructura->cantPosciciones * 2 - 1))
	{
		printf("posicion en X es: %u\n", estructura->coords[iterador]);
		iterador++;
		printf("posicion en Y es: %u\n", estructura->coords[iterador]);
		iterador++;
	}
	printf("la ID del mensaje es: %u\n", estructura->ID);
	printf("la ID correlativa del mensaje es: %i\n", estructura->corrID);
}

void desserializar_catch(Catch* estructura, int32_t socket_cliente)
{
	//saco el largo del nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->largoNombre), sizeof(estructura->largoNombre), MSG_WAITALL));

	estructura->nombrePokemon = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, estructura->nombrePokemon, estructura->largoNombre+1, MSG_WAITALL));

	//saco pos X
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x), MSG_WAITALL));

	//saco pos Y
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y), MSG_WAITALL));

	//saco ID del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->ID), sizeof(estructura->ID), MSG_WAITALL));

	//saco ID CORRELATIVO del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->corrID), sizeof(estructura->corrID), MSG_WAITALL));

	printf("el largo del nombre del pokemon es: %i\n", estructura->largoNombre);
	printf("el nombre del pokemon es: %s\n", estructura->nombrePokemon);
	printf("la pos X es: %u\n", estructura->posPokemon.x);
	printf("la pos Y es: %u\n", estructura->posPokemon.y);
	printf("la ID del mensaje es: %u\n", estructura->ID);
	printf("la ID correlativa del mensaje es: %i\n", estructura->corrID);
}

void desserializar_caught(Caught* estructura, int32_t socket_cliente)
{
	//saco el largo del nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->largoNombre), sizeof(estructura->largoNombre), MSG_WAITALL));

	estructura->nombrePokemon = malloc(estructura->largoNombre+1);

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, estructura->nombrePokemon, estructura->largoNombre+1, MSG_WAITALL));

	//saco el resultado del intento de atrapar al pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->pudoAtrapar), sizeof(estructura->pudoAtrapar), MSG_WAITALL));

	//saco ID CORRELATIVO del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->corrID), sizeof(estructura->corrID), MSG_WAITALL));

	//saco ID CORRELATIVO del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->corrID), sizeof(estructura->corrID), MSG_WAITALL));

	printf("el largo del nombre del pokemon es: %i\n", estructura->largoNombre);
	printf("el nombre del pokemon es: %s\n", estructura->nombrePokemon);
	printf("el resultado de intentar atrapar al pokemon fue: %u\n", estructura->pudoAtrapar);
	printf("la ID del mensaje es: %u\n", estructura->ID);
	printf("la ID correlativa del mensaje es: %i\n", estructura->corrID);
}

void desserializar_suscripcion(Suscripcion* estructura, int32_t socket_cliente)
{
	//saco la COLA a la que suscribirse del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->numeroCola), sizeof(estructura->numeroCola), MSG_WAITALL));
}

void desserializar_dessuscripcion(Dessuscripcion* estructura, int32_t socket_cliente)
{
	//saco la COLA dela que dessuscribirse del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->numeroCola), sizeof(estructura->numeroCola), MSG_WAITALL));
}

