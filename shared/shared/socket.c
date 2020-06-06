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
	printf("error");

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
	uint32_t size_serializado;

	//preparo el paquete para mandar
	void* paquete_serializado = serializar_paquete(paquete_por_armar, mensaje, tipoMensaje, &size_serializado);

	//mando el mensaje
	send(socket, paquete_serializado, size_serializado, 0);

	//libero los malloc utilizados
	eliminar_paquete(paquete_por_armar);
	eliminar_paquete(paquete_serializado);
}

void* serializar_paquete(t_paquete* paquete, void* mensaje, codigo_operacion tipoMensaje, uint32_t *size_serializado)
{
	uint32_t size_ya_armado = 0;
	paquete->codigo_op = tipoMensaje;
	paquete->buffer = malloc(sizeof(t_buffer));

	void* buffer_serializar;//aca se va a guardar todo el choclo ya armado

	//TERMINAR EL SWITCH - ATENCION!! PARECE QUE HAY MAS TIPOS DE MENSAJE, VER A MEDIDA QUE LO VAYAMOS ARMANDO
	switch(tipoMensaje){
		case NEW:
				size_ya_armado = serializar_paquete_new(paquete, mensaje);
			break;

		case APPEARED:
				size_ya_armado = serializar_paquete_appeared(paquete, mensaje);
			break;

		case GET:
				size_ya_armado = serializar_paquete_get(paquete, mensaje);
			break;

		case LOCALIZED://esto no lo puedo hacer todavia porque no pertenece a Gameboy, no conozco formato del mensaje
			break;

		case CATCH:
				size_ya_armado = serializar_paquete_catch(paquete, mensaje);
			break;

		case CAUGHT:
				size_ya_armado = serializar_paquete_caught(paquete, mensaje);
			break;

		case TEST:
				size_ya_armado = serializar_paquete_prueba(paquete, mensaje);
			break;
	}

	//ahora me preparo para meter todo en el buffer "posta", el choclo que va a enviar mandar_mensaje
	buffer_serializar = malloc(size_ya_armado);
	uint32_t desplazamiento = 0;

	//meto en el buffer el tipo de mensaje que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->codigo_op), sizeof(paquete->codigo_op));
	desplazamiento += sizeof(paquete->codigo_op);

	//meto en el buffer el tamaño de todo lo que meti en el buffer del paquete que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->buffer->size), sizeof(paquete->buffer->size));
	desplazamiento += sizeof(paquete->buffer->size);

	//por ultimo meto en el buffer todo lo que meti en el buffer del paquete
	memcpy(buffer_serializar  + desplazamiento, paquete->buffer->stream, paquete->buffer->size);

	(*size_serializado) = size_ya_armado;
	return buffer_serializar; //devuelvo el mensaje listo para enviar
}

uint32_t serializar_paquete_new(t_paquete* paquete, New* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->nombrePokemon), sizeof(pokemon->nombrePokemon));
	desplazamiento += sizeof(pokemon->nombrePokemon);

	//meto coordenada X de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.x), sizeof(pokemon->posPokemon.x));
	desplazamiento += sizeof(pokemon->posPokemon.x);

	//meto coordenada Y de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.y), sizeof(pokemon->posPokemon.y));
	desplazamiento += sizeof(pokemon->posPokemon.y);

	//meto cantidad de pokemons en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->cantPokemon), sizeof(pokemon->cantPokemon));
	desplazamiento += sizeof(pokemon->cantPokemon);

	//le meto al size del buffer el tamaño de todo lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->nombrePokemon) +1 + sizeof(pokemon->posPokemon.x) + sizeof(pokemon->posPokemon.y) + sizeof(pokemon->cantPokemon);

	//el tamaño del mensaje entero es el codigo de operacion + lo que meti en el el buffer + la variable donde me guarde el size
	size = sizeof(codigo_operacion) + sizeof(uint32_t) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_appeared(t_paquete* paquete, Appeared* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->nombrePokemon), sizeof(pokemon->nombrePokemon));
	desplazamiento += sizeof(pokemon->nombrePokemon);

	//meto coordenada X de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.x), sizeof(pokemon->posPokemon.x));
	desplazamiento += sizeof(pokemon->posPokemon.x);

	//meto coordenada Y de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.y), sizeof(pokemon->posPokemon.y));
	desplazamiento += sizeof(pokemon->posPokemon.y);

	//meto la ID CORRELATIVA de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->corrID), sizeof(pokemon->corrID));
	desplazamiento += sizeof(pokemon->corrID);

	//le meto al size del buffer el tamaño de todo lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->nombrePokemon) +1 + sizeof(pokemon->posPokemon.x) + sizeof(pokemon->posPokemon.y) + sizeof(pokemon->corrID);

	//el tamaño del mensaje entero es el codigo de operacion + lo que meti en el el buffer + la variable donde me guarde el size
	size = sizeof(codigo_operacion) + sizeof(uint32_t) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_get(t_paquete* paquete, Get* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->nombrePokemon), sizeof(pokemon->nombrePokemon));
	desplazamiento += sizeof(pokemon->nombrePokemon);

	//meto la ID de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->ID), sizeof(pokemon->ID));
	desplazamiento += sizeof(pokemon->ID);

	//le meto al size del buffer el tamaño de todo lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->nombrePokemon) +1 + sizeof(pokemon->ID);

	//el tamaño del mensaje entero es el codigo de operacion + lo que meti en el el buffer + la variable donde me guarde el size
	size = sizeof(codigo_operacion) + sizeof(uint32_t) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_catch(t_paquete* paquete, Catch* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto nombre del pokemon en buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->nombrePokemon), sizeof(pokemon->nombrePokemon));
	desplazamiento += sizeof(pokemon->nombrePokemon);

	//meto coordenada X de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.x), sizeof(pokemon->posPokemon.x));
	desplazamiento += sizeof(pokemon->posPokemon.x);

	//meto coordenada Y de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.y), sizeof(pokemon->posPokemon.y));
	desplazamiento += sizeof(pokemon->posPokemon.y);

	//meto la ID de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->ID), sizeof(pokemon->ID));
	desplazamiento += sizeof(pokemon->ID);

	//le meto al size del buffer el tamaño de todo lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->nombrePokemon) +1 + sizeof(pokemon->posPokemon.x) + sizeof(pokemon->posPokemon.y) + sizeof(pokemon->ID);

	//el tamaño del mensaje entero es el codigo de operacion + lo que meti en el el buffer + la variable donde me guarde el size
	size = sizeof(codigo_operacion) + sizeof(uint32_t) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

uint32_t serializar_paquete_caught(t_paquete* paquete, Caught* pokemon)
{
	uint32_t size = 0;
	uint32_t desplazamiento = 0;

	//meto la ID CORRELATIVA de mensaje en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->corrID), sizeof(pokemon->corrID));
	desplazamiento += sizeof(pokemon->corrID);

	//meto el resultado del intento de atrapar al pokemon
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->pudoAtrapar), sizeof(pokemon->pudoAtrapar));
	desplazamiento += sizeof(sizeof(pokemon->pudoAtrapar));

	//le meto al size del buffer el tamaño de todo lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->corrID) + sizeof(pokemon->pudoAtrapar);

	//el tamaño del mensaje entero es el codigo de operacion + lo que meti en el el buffer + la variable donde me guarde el size
	size = sizeof(codigo_operacion) + sizeof(uint32_t) + paquete->buffer->size;

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
	size = sizeof(codigo_operacion) + sizeof(uint32_t) + paquete->buffer->size;

	//devuelvo el tamaño de lo que meti en el paquete para poder hacer el malloc
	return size;
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer);
	free(paquete);
}

void recibir_mensaje(void* estructura, int32_t socket_cliente, uint32_t* size)
{
	codigo_operacion codigo;

	bytesRecibidos(recv(socket_cliente, &codigo, sizeof(codigo), MSG_WAITALL)); //saca el codigo de operacion

	bytesRecibidos(recv(socket_cliente, &size, sizeof(size), MSG_WAITALL)); //saca el tamaño de todo lo que sigue en el buffer

	desserializar_mensaje(&estructura, codigo, size, socket_cliente);
}

void desserializar_mensaje (void* estructura, codigo_operacion tipoMensaje, uint32_t* size, int32_t socket_cliente)
//void* desserializar_mensaje (void* restoDelMensaje, codigo_operacion tipoMensaje, uint32_t* size)
{
	void* datosDeMensaje; //esto va a ser una estructura donde se guarda lo que tenga el mensaje

	switch(tipoMensaje)
	{
		case NEW:
			estructura = malloc (sizeof(New));
			desserializar_new(&estructura, size, socket_cliente);
			break;

		case APPEARED:
			estructura = malloc (sizeof(Appeared));
			//datosDeMensaje = malloc(sizeof(Appeared));
			//desserializar_appeared(restoDelMensaje, &datosDeMensaje, size);
			desserializar_appeared(&estructura, size, socket_cliente);
			break;

		case GET:
			estructura = malloc (sizeof(Get));
			desserializar_get(&estructura, size, socket_cliente);
			break;

		case LOCALIZED://esto no lo puedo hacer todavia porque no pertenece a Gameboy, no conozco formato del mensaje
			break;

		case CATCH:
			break;

		case CAUGHT:
			break;

		case TEST:
			datosDeMensaje = malloc(sizeof(char*));
			datosDeMensaje;
			break;
	}

	//return datosDeMensaje;
}

void desserializar_new(New* estructura, uint32_t* size, int32_t socket_cliente)
{
	estructura->nombrePokemon = malloc(sizeof(char*));

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon), MSG_WAITALL));

	//saco pos X
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x), MSG_WAITALL));

	//saco pos Y
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y), MSG_WAITALL));

	//saco cantidad
	bytesRecibidos(recv(socket_cliente, &(estructura->cantPokemon), sizeof(estructura->cantPokemon), MSG_WAITALL));

	//saco ID del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->ID), sizeof(estructura->ID), MSG_WAITALL));
}

//void desserializar_appeared(void* restoDelMensaje, Appeared* estructura, uint32_t* size)
void desserializar_appeared(Appeared* estructura, uint32_t* size, int32_t socket_cliente)
{
	//uint32_t desplazamiento = 0;

	estructura->nombrePokemon = malloc(sizeof(char*));

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon), MSG_WAITALL));

	//saco pos X
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.x), sizeof(estructura->posPokemon.x), MSG_WAITALL));

	//saco pos Y
	bytesRecibidos(recv(socket_cliente, &(estructura->posPokemon.y), sizeof(estructura->posPokemon.y), MSG_WAITALL));


	/*
	//saco el nombre del pokemon
	memcpy(estructura->nombrePokemon, (&restoDelMensaje + desplazamiento), sizeof(estructura->nombrePokemon));
	desplazamiento += sizeof(estructura->nombrePokemon);

	//saco la coordenada X
	memcpy(estructura->posPokemon.x, (&restoDelMensaje + desplazamiento), sizeof(estructura->posPokemon.x));
	desplazamiento += sizeof(estructura->posPokemon.x);

	//saco la coordenada Y
	memcpy(estructura->posPokemon.y, (&restoDelMensaje + desplazamiento), sizeof(estructura->posPokemon.y));
	desplazamiento += sizeof(estructura->posPokemon.y);
	*/
}

void desserializar_get(Get* estructura, uint32_t* size, int32_t socket_cliente)
{
	estructura->nombrePokemon = malloc(sizeof(char*));

	//saco el nombre del pokemon
	bytesRecibidos(recv(socket_cliente, &(estructura->nombrePokemon), sizeof(estructura->nombrePokemon), MSG_WAITALL));

	//saco ID del mensaje
	bytesRecibidos(recv(socket_cliente, &(estructura->ID), sizeof(estructura->ID), MSG_WAITALL));
}
