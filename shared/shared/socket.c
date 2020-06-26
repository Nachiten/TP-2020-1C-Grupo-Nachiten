/*
 * socket.c
 *
 *  Created on: 17 abr. 2020
 *      Author: utnso
 */

#include "socket.h"

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

	//ToDo cambiar la manera en que funciona esto?
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
			paquete->buffer->stream = malloc(sizeof(Localized) + 26); //+ 25 posiciones para el nombre + el \n
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
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->nombrePokemon), pokemon->largoNombre+1);
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

	puts("los size:");
	printf("el size de largonombre es: %u\n",sizeof(pokemon->largoNombre));
	printf("el largonombre +1 es: %u\n",(pokemon->largoNombre+1));
	printf("el size de POS X es: %u\n",sizeof(pokemon->posPokemon.x));
	printf("el size de POS Y es: %u\n",sizeof(pokemon->posPokemon.y));
	printf("el size de cantidad de pokemon es: %u\n",sizeof(pokemon->cantPokemon));
	printf("el size de ID es: %u\n",sizeof(pokemon->ID));
	printf("el size de ID correl es: %u\n",sizeof(pokemon->corrID));

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
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->nombrePokemon), pokemon->largoNombre+1);
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
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->nombrePokemon), pokemon->largoNombre+1);
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

uint32_t serializar_paquete_catch(t_paquete* paquete, Catch* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto el largo del nombre del pokemon
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->largoNombre), sizeof(pokemon->largoNombre));
	desplazamiento += sizeof(pokemon->largoNombre);

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->nombrePokemon), pokemon->largoNombre+1);
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
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->nombrePokemon), pokemon->largoNombre+1);
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

void recibir_mensaje(void* estructura, codigo_operacion tipoMensaje, int32_t socket_cliente)
{
	uint32_t size;

	bytesRecibidos(recv(socket_cliente, &size, sizeof(size), MSG_WAITALL)); //saca el tamaño de lo que sigue en el buffer
	printf("Tamaño de lo que sigue en el buffer: %u.\n",size);

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
			free(estructura);
			break;

		case GET:
			desserializar_get(estructura, socket_cliente);
			free(estructura);
			break;

		case LOCALIZED://esto no lo puedo hacer todavia porque la estructura no esta completa
			break;

		case CATCH:
			desserializar_catch(estructura, socket_cliente);
			free(estructura);
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

		case DESCONEXION://Estos 3 están solo para que no sale el WARNING, no sirven para nada aca
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
	printf("el largo del nombre del pokemon es: %i\n", estructura->largoNombre);

	//char* saveMe = malloc(estructura->largoNombre+1);
	char* saveMe[estructura->largoNombre];

	//saco el nombre del pokemon
	//bytesRecibidos(recv(socket_cliente, &(estructura->nombrePokemon), estructura->largoNombre+1, MSG_WAITALL));
	//printf("el nombre del pokemon es: %s\n", estructura->nombrePokemon);

	bytesRecibidos(recv(socket_cliente, &saveMe, estructura->largoNombre+1, MSG_WAITALL));

	estructura->nombrePokemon = "pikachu";

	//printf("el nombre del pokemon es: %s\n", saveMe);

	//strcpy(saveMe,estructura->nombrePokemon);
	//printf("el nombre del pokemon es: %s\n", estructura->nombrePokemon);



	//free(saveMe);


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

	printf("la pos X es: %u\n", estructura->posPokemon.x);
	printf("la pos Y es: %u\n", estructura->posPokemon.y);
	printf("la cantidad de pokemones es: %u\n", estructura->cantPokemon);
	printf("la ID del mensaje es: %u\n", estructura->ID);
	printf("la ID correlativa del mensaje es: %i\n", estructura->corrID);
}

void desserializar_appeared(Appeared* estructura, int32_t socket_cliente)
{
	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon), MSG_WAITALL));

	//saco pos X
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x), MSG_WAITALL));

	//saco pos Y
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y), MSG_WAITALL));
}

void desserializar_get(Get* estructura, int32_t socket_cliente)
{
	estructura->nombrePokemon = malloc(sizeof(char*));

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon), MSG_WAITALL));

	//saco ID del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->ID), sizeof(estructura->ID), MSG_WAITALL));
}

void desserializar_catch(Catch* estructura, int32_t socket_cliente)
{
	estructura->nombrePokemon = malloc(sizeof(char*));

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon), MSG_WAITALL));

	//saco pos X
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x), MSG_WAITALL));

	//saco pos Y
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y), MSG_WAITALL));

	//saco ID del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->ID), sizeof(estructura->ID), MSG_WAITALL));
}

void desserializar_caught(Caught* estructura, int32_t socket_cliente)
{
	//saco ID CORRELATIVO del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->corrID), sizeof(estructura->corrID), MSG_WAITALL));

	//saco el resultado del intento de atrapar al pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->pudoAtrapar), sizeof(estructura->pudoAtrapar), MSG_WAITALL));
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

void recibir_mensaje_cliente(Hilo* estructura){
	//while(1){
	recibir_mensaje(estructura->mensaje,NEW,estructura->conexion);
	//guardar estructura en una variable global para que la levante otro hilo
	//}
}
