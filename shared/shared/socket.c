/*
 * socket.c
 *
 *  Created on: 17 abr. 2020
 *      Author: utnso
 */

#include "socket.h"

int establecer_conexion(char* ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
	printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

void resultado_de_conexion(int socket, t_log* logger, char* modulo)
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

void cerrar_conexion(int socket)
{
	close(socket);
}

void mandar_mensaje(char* mensaje, codigo_operacion tipoMensaje, int socket_cliente)
{
	t_paquete* paquete_por_armar = malloc (sizeof(t_paquete));
	uint32_t size_serializado;

	//preparo el paquete para mandar
	void* paquete_serializado = serializar_paquete(paquete_por_armar, mensaje, tipoMensaje, &size_serializado);

	//mando el mensaje
	send(socket_cliente, paquete_serializado, size_serializado, 0);

	//libero los malloc utilizados
	eliminar_paquete(paquete_por_armar);
	eliminar_paquete(paquete_serializado);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer);
	free(paquete);
}

void* serializar_paquete(t_paquete* paquete, char* mensaje, codigo_operacion tipoMensaje, uint32_t *size_serializado)
{
	paquete->codigo_op = tipoMensaje;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream = mensaje;
	paquete->buffer->size = strlen(mensaje)+1;

	uint32_t size = sizeof(codigo_operacion) + sizeof(uint32_t) + paquete->buffer->size;
	void* buffer_serializar = malloc(size);
	uint32_t desplazamiento = 0;

	//meto en el buffer el tipo de mensaje que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->codigo_op), sizeof(paquete->codigo_op));
	desplazamiento += sizeof(paquete->codigo_op);

	//meto en el buffer el tamaño del mensaje que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->buffer->size), sizeof(paquete->buffer->size));
	desplazamiento += sizeof(paquete->buffer->size);

	//por ultimo meto en el buffer el mensaje en si
	memcpy(buffer_serializar  + desplazamiento, paquete->buffer->stream, paquete->buffer->size);

	(*size_serializado) = size;
	return buffer_serializar; //devuelvo el mensaje listo para enviar
}

char* recibir_mensaje(int socket_cliente, int* size)
{
	codigo_operacion codigo;
	//esto por ahora queda asi porque todavia nos da igual el codigo de operacion, pero despues va a cambiar-------------------
	recv(socket_cliente, &codigo, sizeof(codigo), 0);
	//ignorar esa linea por ahora ---------------------------------------------------------------------------------------------
	recv(socket_cliente, &size, sizeof(size), MSG_WAITALL);
	char* buffer = malloc(size);
	recv(socket_cliente, &size, sizeof(size), MSG_WAITALL);

	/*if(buffer[size-1] != '\0')
	{
		printf("ERROR! mensaje no reconocido.");
	}
	*/
	return buffer;
}

