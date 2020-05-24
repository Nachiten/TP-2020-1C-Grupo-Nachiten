/*
 * socket.c
 *
 *  Created on: 17 abr. 2020
 *      Author: utnso
 */

#include "socket.h"

uint32_t establecer_conexion(char* ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	uint32_t socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
	printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

void resultado_de_conexion(uint32_t socket, t_log* logger, char* modulo)
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

void cerrar_conexion(uint32_t socket)
{
	close(socket);
}

void mandar_mensaje(void* mensaje, codigo_operacion tipoMensaje, uint32_t socket)
{
	t_paquete* paquete_por_armar = malloc (sizeof(t_paquete));
	uint32_t size_serializado;

	//preparo el paquete para mandar
	void* paquete_serializado = serializar_paquete(paquete_por_armar, mensaje, tipoMensaje, &size_serializado);

	puts("antes del mensaje"); //de referencia para ver cuando crashea
	//mando el mensaje
	send(socket, paquete_serializado, size_serializado, 0);
	puts("despues del mensaje"); //de referencia para ver cuando crashea

	//libero los malloc utilizados
	eliminar_paquete(paquete_por_armar);
	eliminar_paquete(paquete_serializado);
}

//---------------------------VERSION SERIALIZAR CON SWITCH CASE (LINEAS 80 - )-----------------------------------------------------------
void* serializar_paquete(t_paquete* paquete, void* mensaje, codigo_operacion tipoMensaje, uint32_t *size_serializado)
{
	paquete->codigo_op = tipoMensaje;
	paquete->buffer = malloc(sizeof(t_buffer));

	void* buffer_serializar;//aca se va a guardar todo el choclo ya armado


	switch(tipoMensaje){
		case 1:
				buffer_serializar = serializar_paquete_appeared(paquete, mensaje, *size_serializado);
			break;

		case 404:
				buffer_serializar = serializar_paquete_prueba(paquete, mensaje, *size_serializado);
			break;
	}

	/* NOTA: hay codigo que se repite, eso habria que traerlo para aca pero estoy quemado, es, a partir de que se guardo todo en el buffer
	 * del paquete, es decir cuando empiezo a meter todo a "buffer_serializar".
	*/
	return buffer_serializar; //devuelvo el mensaje armado para que lo envien
}

void* serializar_paquete_appeared(t_paquete* paquete, Appeared* pokemon, uint32_t *size_serializado)
{
	uint32_t desplazamiento = 0;
	//meto nombre de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->largoNombre), sizeof(pokemon->largoNombre));
	desplazamiento += sizeof(pokemon->largoNombre);

	//meto coordenada X de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.x), sizeof(pokemon->posPokemon.x));
	desplazamiento += sizeof(pokemon->posPokemon.x);

	//meto coordenada Y de pokemon en el buffer del paquete
	memcpy(paquete->buffer->stream + desplazamiento, &(pokemon->posPokemon.y), sizeof(pokemon->posPokemon.y));
	desplazamiento += sizeof(pokemon->posPokemon.y);

	//le meto al size del buffer el tamaño de todo lo que acabo de meter en el buffer
	paquete->buffer->size = sizeof(pokemon->largoNombre) + 1 + sizeof(pokemon->posPokemon.x) + sizeof(pokemon->posPokemon.y);

	//el tamaño del mensaje entero es el codigo de operacion + lo que meti en el el buffer + la variable donde me guarde el si
	uint32_t size = sizeof(codigo_operacion) + sizeof(uint32_t) + paquete->buffer->size;

	//ahora me preparo para meter todo en el buffer "posta", el choclo que va a enviar mandar_mensaje
	void* buffer_serializar = malloc(size);
	desplazamiento = 0; //reciclo variable

	//meto en el buffer el tipo de mensaje que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->codigo_op), sizeof(paquete->codigo_op));
	desplazamiento += sizeof(paquete->codigo_op);

	//meto en el buffer el tamaño de todo lo que meti en el buffer del paquete que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->buffer->size), sizeof(paquete->buffer->size));
	desplazamiento += sizeof(paquete->buffer->size);

	//por ultimo meto en el buffer todo lo que meti en el buffer del paquete
	memcpy(buffer_serializar  + desplazamiento, paquete->buffer->stream, paquete->buffer->size);

	(*size_serializado) = size;
	return buffer_serializar; //devuelvo el mensaje listo para enviar
}

void* serializar_paquete_prueba (t_paquete* paquete, char* mensaje, uint32_t *size_serializado)
{
	paquete->buffer->stream = mensaje;
	paquete->buffer->size = strlen(mensaje)+1;
	uint32_t size = sizeof(codigo_operacion) + sizeof(uint32_t) + paquete->buffer->size;

	void* buffer_serializar = malloc(size);
	uint32_t desplazamiento = 0;

	//meto en el buffer el tipo de mensaje que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->codigo_op), sizeof(paquete->codigo_op));
	desplazamiento += sizeof(paquete->codigo_op);

	//meto en el buffer el tamaño de todo lo que meti en el buffer del paquete que voy a mandar
	memcpy(buffer_serializar  + desplazamiento, &(paquete->buffer->size), sizeof(paquete->buffer->size));
	desplazamiento += sizeof(paquete->buffer->size);

	//por ultimo meto en el buffer todo lo que meti en el buffer del paquete
	memcpy(buffer_serializar  + desplazamiento, paquete->buffer->stream, paquete->buffer->size);

	(*size_serializado) = size;
	return buffer_serializar; //devuelvo el mensaje listo para enviar
}

//----------------------------------------------------------------------------------------------------------------------------------

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer);
	free(paquete);
}

/* ESTE ES EL ORIGINAL
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
*/

char* recibir_mensaje(uint32_t socket_cliente, uint32_t* size)
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

