#include "mensajes.h"

void mensajeNew(char* pokemon, int posX, int posY, int cantidad, int IDMensaje){

	// Checkeo de variables
	if (pokemon == NULL){
		printf("ERROR | No hay ningun pokemon");
		return;
	}

	if (posX < 0 || posY < 0){
		printf("ERROR | La posicion no puede ser negativa");
		return;
	}

	if (cantidad < 1){
		printf("ERROR | La cantidad no puede ser 0 ni negativa");
		return;
	}

	crearPokemonSiNoExiste(pokemon);

	// Realizar lo que corresponde para abrir el archivo metadata (marcarlo open=y)
	abrirArchivoPokemon(pokemon);

	if (hayAlgunBloque(pathFiles, pokemon)){
		//printf("NEW | Ya habia bloques, leyendo...\n");

		char** bloques = leerBloquesPokemon(pokemon);

		int cantidadBytesALeer = leerSizePokemon(pokemon);

		char* lineasLeidas = leerContenidoBloquesPokemon(bloques, cantidadBytesALeer);

		// Lineas incluyendo la linea nueva
		char* lineasNuevasMasPokemon = NULL;

		// Cantidad de bloques finales luego de agregar la linea
		int cantidadBloquesRequeridos;

		// Cantidad de bloques anterior
		int cantidadBloquesActual;

		t_list* listaBloques = list_create();

		listaBloques = convertirAListaDeEnterosDesdeChars(bloques);

		// La linea buscada no es encontrada dentro del archivo, se debe agregar una linea nueva
		if (encontrarCoords(posX, posY, lineasLeidas) == -1){

			//printf("NEW | La linea NO fue encontrada... pegando al final\n");

			lineasNuevasMasPokemon = agregarNuevoPokemonALineas(posX, posY, cantidad, lineasLeidas);

			log_info(logger, "NEW | Se agregara la linea %i-%i=%i al pokemon %s", posX, posY, cantidad, pokemon);

		} else {

			lineasNuevasMasPokemon = sumarALineaPokemon(lineasLeidas, posX, posY, cantidad);

			log_info(logger, "NEW | Se modificara la linea existente %i-%i sumando %i al pokemon %s", posX, posY, cantidad, pokemon);
		}

		cantidadBloquesRequeridos = cantidadDeBloquesQueOcupa(strlen(lineasNuevasMasPokemon));

		cantidadBloquesActual = cantidadDeElementosEnArray(bloques);

		// No necesito bloques extra. solo escribir los que hay
		if (cantidadBloquesRequeridos == cantidadBloquesActual){

			// La cantidad se mantiene igual, solo escribir los bloques
			//printf("NEW | No se necesitan bloques extra... solo escribir\n");

			// Generar lista con los datos a escribir en los bloques
			t_list* listaDatos = separarStringEnBloques(lineasNuevasMasPokemon, cantidadBloquesRequeridos);

			// Escribir los datos en los bloques correspondientes
			escribirLineasEnBloques(listaBloques, listaDatos);

			// Borrar elementos de la lista antes de borrar lista
			int i;
			int cantidadElementosLista = list_size(listaDatos);
			for ( i = cantidadElementosLista - 1; i >= 0; i--){
				char* elemento = list_remove(listaDatos, i);
				free(elemento);
			}

			list_destroy(listaDatos);


			// Fijo el SIZE=60
			fijarSizeA(pokemon, strlen(lineasNuevasMasPokemon));

		// Debo pedir bloques nuevos
		} else if (cantidadBloquesRequeridos > cantidadBloquesActual){

			//printf("NEW | Se necesitan mas bloques... pidiendo\n");

			// Cantidad de bloques extra que se deben pedir
			int cantidadBloquesExtra = cantidadBloquesRequeridos - cantidadBloquesActual;

			// Sumo los bloques extra a la listaBloques original
			sumarBloquesExtraALista(listaBloques, cantidadBloquesExtra);

			// Generar lista con los datos a escribir en los bloques
			t_list* listaDatos = separarStringEnBloques(lineasNuevasMasPokemon, cantidadBloquesRequeridos);

			// Escribir los datos en los bloques correspondientes
			escribirLineasEnBloques(listaBloques, listaDatos);

			// Fijo el string BLOCKS=[1,2,3]
			fijarBloquesA(pokemon, listaBloques);

			// TODO | Hacer bien
			// Borrar elementos de la lista antes de borrar lista
			//int i;
//			int cantidadElementosLista = list_size(listaDatos);
//			for (i=0; i < cantidadElementosLista; i++){
//				char* elemento = list_remove(listaDatos, i);
//				free(elemento);
//			}

			list_destroy(listaDatos);

			// Fijo el SIZE=60
			fijarSizeA(pokemon, strlen(lineasNuevasMasPokemon));


		} else {
			printf("ERROR | La cantidad de bloques requeridos no puede ser menor al agregar un pokemon nuevo");
		}

		list_destroy(listaBloques);

		if (lineasNuevasMasPokemon != NULL) free(lineasNuevasMasPokemon);

		free(lineasLeidas);

	} else {
		escribirLineaNuevaPokemon(pokemon, posX, posY, cantidad);
		log_info(logger, "NEW | Escribiendo nueva linea %i-%i=%i en pokemon vacio %s", posX, posY, cantidad, pokemon);
	}



	//sleep(TIEM_REIN_OPERACION);

	// Cerrar el archivo luego de usarlo
	cerrarArchivoPokemon(pokemon);

	enviarMensajeAppeared(pokemon, posX, posY, IDMensaje); // TODO Num 3 HARDCODEADO | En realidad toma el ID como parametro esta funcion
}

void mensajeCatch(char* pokemon, int posX, int posY, int IDMensaje){

	int resultado = 0;

	if (existeCarpetaPokemon(pokemon) == 1){
		// Abrir el archivo

		abrirArchivoPokemon(pokemon);

		char** bloques = leerBloquesPokemon(pokemon);

		int pesoEnBytes = leerSizePokemon(pokemon);

		char* lineasLeidas = leerContenidoBloquesPokemon(bloques, pesoEnBytes);

		int lineaEncontrada = encontrarCoords(posX, posY, lineasLeidas);

		if (lineaEncontrada != -1){
			//printf("CATCH | Se encontro la linea\n");
			log_info(logger, "CATCH | Se le restara 1 a la coordenada %i-%i dentro del archivo %s", posX, posY, pokemon);

			char* lineasModificadas = restarALineaPokemon(lineasLeidas, posX, posY);

			int cantidadBloquesActual = cantidadDeElementosEnArray(bloques);

			int cantidadBloquesRequeridos = cantidadDeBloquesQueOcupa(strlen(lineasModificadas));

			// Importante: Strlen de "" es 0

			t_list* listaBloques = convertirAListaDeEnterosDesdeChars(bloques);

//			printf("Printeando lista antes de liberar:");
//			printearListaDeEnteros(listaBloques);

			// No debo liberar ningun bloque, ocupa lo mismo
			if (cantidadBloquesActual == cantidadBloquesRequeridos){
				//printf("CATCH | La cantidad de bloques se mantiene igual");


			} else if (cantidadBloquesRequeridos < cantidadBloquesActual){
				// TODO | Terminar
				/* Luego de liberar los bloques y modificar la lista se debe
				 * 1) Separar el array en bloques
				 * 2) Escribir los datos de bloques
				 * 3) Escribir el size en metadata.bin
				 * 4) Escribir los nuevos bloques en metadata.bin
				 */
				//printf("CATCH | Se deben liberar bloques\n");

				int cantidadDeBloquesALiberar = cantidadBloquesActual - cantidadBloquesRequeridos;

				//liberarNBloques(listaBloques, cantidadDeBloquesALiberar);

				// Tamanio: 2 | BloquesALiberar: 1

				liberarNBloques(listaBloques, cantidadDeBloquesALiberar);

//				printf("Printeando lista despues de liberar:");
//				printearListaDeEnteros(listaBloques);

				fijarBloquesA(pokemon, listaBloques);

			} else {
				printf("ERROR | La cantidad de bloques al caputrar un pokemon no puede ser mayor a la anterior");
			}

			t_list* listaDatosBloques = separarStringEnBloques(lineasModificadas, cantidadBloquesRequeridos);

			escribirLineasEnBloques(listaBloques, listaDatosBloques);

			// Sacar
			int i;
			int cantidadElementosLista = list_size(listaDatosBloques);
			for (i = cantidadElementosLista - 1; i >= 0; i--)
			{
				char* elemento = list_remove(listaDatosBloques, i);
				free(elemento);
			}

			list_destroy(listaDatosBloques);
			list_destroy(listaBloques);

			fijarSizeA(pokemon, strlen(lineasModificadas));

			resultado = 1;

			free(lineasModificadas);
		} else {
			//printf("CATCH | Las coordenadas buscadas no existen dentro del archivo\n");
			log_warning(logger, "CATCH | Las coordenadas %i-%i no existen dentro del archivo %s", posX, posY, pokemon);
		}

		cerrarArchivoPokemon(pokemon);

		free(lineasLeidas);
	} else {
		//printf("CATCH | No existe el pokemon buscado\n");
		log_warning(logger, "CATCH | El pokemon %s no existe", pokemon);
	}

	enviarMensajeCaught(pokemon, resultado, IDMensaje);

}

void mensajeGet(char* pokemon, int IDMensaje){

	printf("Pokemon buscado: %s\n", pokemon);
	printf("Id Mensaje: %i", IDMensaje);

	t_list* listaCoords = list_create();

	if (existeCarpetaPokemon(pokemon)){

		abrirArchivoPokemon(pokemon);

		char** bloquesLeidos = leerBloquesPokemon(pokemon);

		int cantBytes = leerSizePokemon(pokemon);

		if (cantBytes > 0)
		{
			// Leer lineas del archivo
			char* lineasLeidas = leerContenidoBloquesPokemon(bloquesLeidos, cantBytes);

			// Generar una lista con todas las coordenadas del archivo
			listaCoords = convertirAListaDeCoords(lineasLeidas);

			log_info(logger, "GET | Se encontraron coordenadas del pokemon %s", pokemon);
		} else {
			log_warning(logger, "GET | El pokemon %s está vacio", pokemon);
		}

		cerrarArchivoPokemon(pokemon);

	} else {
		//printf("GET | El pokemon %s no existe\n", pokemon);
		log_warning(logger, "GET | El pokemon %s no existe", pokemon);
	}

	Localized* miStruct = generarStructLocalized(pokemon, listaCoords, IDMensaje);

	// Liberar lista de enteros

	int i;
	int cantidadElementosLista = list_size(listaCoords);
	for (i = cantidadElementosLista - 1; i >= 0; i--){
		int* elemento = list_remove(listaCoords, i);
		free(elemento);
	}

	list_destroy(listaCoords);

	enviarMensajeLocalized(pokemon, miStruct, IDMensaje);

	//free(miStruct->nombrePokemon);
	free(miStruct);


}

void enviarMensajeAppeared(char* pokemon, int posX, int posY, int IDMensaje){
	printf("Se enviara el siguiente mensaje al broker (cola appeared):\n");
	printf("Pokemon: %s\n", pokemon);
	printf("PosX: %i\n", posX);
	printf("PosY: %i\n", posY);
	printf("ID Corrlativo: %i\n", IDMensaje);

	Appeared* structAEnviar = malloc(sizeof(Appeared) + strlen(pokemon) + 1);

	structAEnviar->ID = 0;
	structAEnviar->corrID = IDMensaje;

	structAEnviar->largoNombre = strlen(pokemon);
	structAEnviar->nombrePokemon = pokemon;

	structAEnviar->posPokemon.x = posX;
	structAEnviar->posPokemon.y = posY;

	//mandar_mensaje(structAEnviar, APPEARED, );

	// TODO | Liberar nombre pokemon
	free(structAEnviar);
}

void enviarMensajeCaught(char* pokemon, int resultado, int IDMensaje){
	Caught* structCaught = malloc(sizeof(Caught) + strlen(pokemon) + 1);

	structCaught->largoNombre = strlen(pokemon);
	structCaught->nombrePokemon = pokemon;

	structCaught->pudoAtrapar = resultado;

	structCaught->corrID = IDMensaje;
	structCaught->ID = 0;

	// TODO | Falta terminar
	//mandar_mensaje()

	free(structCaught);
}

void enviarMensajeLocalized(char* pokemon, Localized* structAEnviar, int IDMensaje){
	printf("Se enviara el siguiente mensaje al broker (cola appeared):\n");
	printf("Pokemon: %s\n", pokemon);
	printf("ID Mensaje: %i\n", IDMensaje);

	int i;
	//Mostrar la lista coords del struct
	printf ("ListaCoords:\n");
	for (i = 0; i < structAEnviar->cantPosciciones * 2; i+=2){

		int coordX = structAEnviar->coords[i];
		int coordY = structAEnviar->coords[i+1];

		printf("Coord X: %i ", coordX);
		printf("Coord Y: %i\n", coordY);
	}

	//Todo | falta enviar el mensaje

	//int socketLocalized = establecer_conexion(IP_BROKER, PUERTO_BROKER);

	//mandar_mensaje(structAEnviar, LOCALIZED , socketLocalized );
}

Localized* generarStructLocalized(char* pokemon, t_list* listaCoords, int IDMensaje){

	int cantidadCoords =  list_size(listaCoords) / 2;
	int tamanioArray = list_size(listaCoords);

	Localized* structLocalized = malloc(sizeof(struct Localized) + tamanioArray * sizeof(uint32_t) + strlen(pokemon) + 1);

	int i;

	for (i = 0; i< list_size(listaCoords); i++){
		int* coordenadaActual = list_get(listaCoords, i);
		structLocalized->coords[i] = *coordenadaActual;

	}

	structLocalized->cantPosciciones = cantidadCoords;

	structLocalized->largoNombre = strlen(pokemon);
	structLocalized->nombrePokemon = pokemon;

	structLocalized->ID = IDMensaje;
	structLocalized->corrID = -2;

	return structLocalized;
}
