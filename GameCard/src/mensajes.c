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
		printf("NEW | Ya habia bloques, leyendo...\n");

		char** bloques = leerBloquesPokemon(pokemon);

		int cantidadBytesALeer = leerSizePokemon(pokemon);

		char* lineasLeidas = leerContenidoBloquesPokemon(bloques, cantidadBytesALeer);

		// Lineas incluyendo la linea nueva
		char* lineasNuevasMasPokemon;

		// Cantidad de bloques finales luego de agregar la linea
		int cantidadBloquesRequeridos;

		// Cantidad de bloques anterior
		int cantidadBloquesActual;

		t_list* listaBloques = list_create();

		listaBloques = convertirAListaDeEnterosDesdeChars(bloques);

		// La linea buscada no es encontrada dentro del archivo, se debe agregar una linea nueva
		if (encontrarCoords(posX, posY, lineasLeidas) == -1){

			printf("NEW | La linea NO fue encontrada... pegando al final\n");

			lineasNuevasMasPokemon = agregarNuevoPokemonALineas(posX, posY, cantidad, lineasLeidas);

		} else {
			printf("NEW | La linea fue encontrada, se la debe modificar... [No hecho todavia]");
			//printf("Lineas con linea reemplazada: %s", lineasConLineaReemplazada);

			lineasNuevasMasPokemon = sumarALineaPokemon(lineasLeidas, posX, posY, cantidad);
		}

		cantidadBloquesRequeridos = cantidadDeBloquesQueOcupa(strlen(lineasNuevasMasPokemon));

		cantidadBloquesActual = cantidadDeElementosEnArray(bloques);

		// No necesito bloques extra. solo escribir los que hay
		if (cantidadBloquesRequeridos == cantidadBloquesActual){

			// La cantidad se mantiene igual, solo escribir los bloques
			printf("NEW | No se necesitan bloques extra... solo escribir\n");

			// Generar lista con los datos a escribir en los bloques
			t_list* listaDatos = separarStringEnBloques(lineasNuevasMasPokemon, cantidadBloquesRequeridos);

			// Escribir los datos en los bloques correspondientes
			escribirLineasEnBloques(listaBloques, listaDatos);

			// Fijo el SIZE=60
			fijarSizeA(pokemon, strlen(lineasNuevasMasPokemon));

		// Debo pedir bloques nuevos
		} else if (cantidadBloquesRequeridos > cantidadBloquesActual){

			printf("NEW | Se necesitan mas bloques... pidiendo\n");

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

			// Fijo el SIZE=60
			fijarSizeA(pokemon, strlen(lineasNuevasMasPokemon));


		} else {
			printf("ERROR | La cantidad de bloques requeridos no puede ser menor al agregar un pokemon nuevo");
		}


	} else {
		printf("NEW | No habia bloques... generando de 0\n");
		escribirLineaNuevaPokemon(pokemon, posX, posY, cantidad);
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
			printf("CATCH | Se encontro la linea\n");

			char* lineasModificadas = restarALineaPokemon(lineasLeidas, posX, posY);

			int cantidadBloquesActual = cantidadDeElementosEnArray(bloques);

			int cantidadBloquesRequeridos = cantidadDeBloquesQueOcupa(strlen(lineasModificadas));

			// Importante: Strlen de "" es 0

			t_list* listaBloques = convertirAListaDeEnterosDesdeChars(bloques);

//			printf("Printeando lista antes de liberar:");
//			printearListaDeEnteros(listaBloques);

			// No debo liberar ningun bloque, ocupa lo mismo
			if (cantidadBloquesActual == cantidadBloquesRequeridos){
				printf("CATCH | La cantidad de bloques se mantiene igual");


			} else if (cantidadBloquesRequeridos < cantidadBloquesActual){
				// TODO | Terminar
				/* Luego de liberar los bloques y modificar la lista se debe
				 * 1) Separar el array en bloques
				 * 2) Escribir los datos de bloques
				 * 3) Escribir el size en metadata.bin
				 * 4) Escribir los nuevos bloques en metadata.bin
				 */
				printf("CATCH | Se deben liberar bloques\n");

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

			fijarSizeA(pokemon, strlen(lineasModificadas));

			resultado = 1;
		} else {
			printf("CATCH | Las coordenadas buscadas no existen dentro del archivo\n");
		}

		cerrarArchivoPokemon(pokemon);
	} else {
		printf("CATCH | No existe el pokemon buscado\n");
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

		// Leer lineas del archivo
		char* lineasLeidas = leerContenidoBloquesPokemon(bloquesLeidos, cantBytes);

		// Generar una lista con todas las coordenadas del archivo
		listaCoords = convertirAListaDeCoords(lineasLeidas);

		cerrarArchivoPokemon(pokemon);

	} else {
		printf("GET | El pokemon %s no existe\n", pokemon);
	}

	Localized* miStruct = generarStructLocalized(pokemon, listaCoords, IDMensaje);

	enviarMensajeLocalized(pokemon, miStruct, IDMensaje);


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

	int socketLocalized = establecer_conexion(IP_BROKER, PUERTO_BROKER);

	mandar_mensaje(structAEnviar, LOCALIZED , socketLocalized );

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
