/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	int size_serializado = (sizeof(int) + sizeof(op_code) + paquete->buffer->size);

	void *buffer = malloc(size_serializado);

	int offset = 0;

	memcpy(buffer + offset, &(paquete->codigo_operacion), sizeof(paquete->codigo_operacion));

	offset += sizeof(paquete->codigo_operacion);

	memcpy(buffer + offset, &(paquete->buffer->size), sizeof(paquete->buffer->size));

	offset += sizeof(paquete->buffer->size);

	memcpy(buffer + offset, paquete->buffer->stream, paquete->buffer->size);

	*bytes = size_serializado;

	return buffer;

}

int crear_conexion(char *ip, char* puerto)
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

void enviar_mensaje(char* mensaje, int socket_cliente)
{

	t_paquete* unPaquete = malloc(sizeof(t_paquete));

	unPaquete->codigo_operacion = MENSAJE;
	unPaquete->buffer = malloc(sizeof(t_buffer));
	unPaquete->buffer->stream = mensaje;
	unPaquete->buffer->size = strlen(mensaje) + 1;

	int tamanioStream;

	void* aEnviar = serializar_paquete( unPaquete , &tamanioStream);

	send(socket_cliente, aEnviar , tamanioStream , 0);

	free(aEnviar);

	free(unPaquete->buffer);
	free(unPaquete);
}

char* recibir_mensaje(int socket_cliente)
{
	// devolver el string que el servidor me mando (el mismo q yo mande)

	op_code operacion;

	recv(socket_cliente, &operacion,sizeof(operacion), 0);
	int buffer_size;

	recv(socket_cliente, &buffer_size,sizeof(buffer_size), 0);

	char *buffer = malloc(buffer_size);
	recv(socket_cliente, buffer,buffer_size, 0);

	if(buffer[buffer_size - 1] != '\0'){
		printf("WARN - El buffer recibido no es un string D:\n");
	}

	return buffer;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
