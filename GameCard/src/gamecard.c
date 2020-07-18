#include "gamecard.h"

void leerConfig(int* TIEM_REIN_CONEXION, int* TIEM_REIN_OPERACION, char** PUNTO_MONTAJE, char** IP_BROKER, char** PUERTO_BROKER, t_config* config){

	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/GameCard.config");

	if (config == NULL){
		printf("No se pudo leer la config!!");
		exit(1);
	}

	*TIEM_REIN_CONEXION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
	*TIEM_REIN_OPERACION = config_get_int_value(config, "TIEMPO_DE_REINTENTO_OPERACION");
	*PUNTO_MONTAJE = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	*IP_BROKER = config_get_string_value(config, "IP_BROKER");
	*PUERTO_BROKER = config_get_string_value(config, "PUERTO_BROKER");

	if (TIEM_REIN_CONEXION == NULL){
		printf("No se ha podido leer el tiempo reintento conexion de la config");
	}
	if (TIEM_REIN_OPERACION == NULL){
		printf("No se ha podido leer el tiempo reintento operacion de la config");
	}
	if (PUNTO_MONTAJE == NULL){
		printf("No se ha podido leer el punto_montaje de la config");
	}
	if (IP_BROKER == NULL){
		printf("No se ha podido leer la ip_broker de la config");
	}
	if (PUERTO_BROKER == NULL){
		printf("No se ha podido leer el puerto_broker de la config");
	}

	//config_destroy(config);
}

void leerMetadataBin(char* pathMetadata, int* BLOCKS, int* BLOCK_SIZE, char** MAGIC_NUMBER, t_config* metadataBin){

	// Nombre del archivo metadata
	char* archivoMetadata = "/Metadata.bin";

	// Path de la carpeta {punto_montaje}/Metadata/Metadata.bin
	char* pathMetadataBin = malloc(strlen(pathMetadata) + strlen(archivoMetadata) + 1);

	// Le pego el path de metadata
	strcpy(pathMetadataBin, pathMetadata);
	// Le pego al final el path del archivo
	strcat(pathMetadataBin, archivoMetadata);

	// Leo el archivo
	metadataBin = leerConfiguracion(pathMetadataBin);

	if (metadataBin == NULL){
		printf("No se pudo leer el archivo Metadata/Metadata.bin");
		exit(2);
	}

	// Obtengo los valores (pasados por referencia)
	*BLOCKS = config_get_int_value(metadataBin, "BLOCKS");
	*BLOCK_SIZE = config_get_int_value(metadataBin, "BLOCK_SIZE");
	*MAGIC_NUMBER = config_get_string_value(metadataBin,"MAGIC_NUMBER" );

	free(pathMetadataBin);

	/*
	printf("%i\n", *BLOCKS);
	printf("%i\n", *BLOCK_SIZE);
	printf("%s\n", *MAGIC_NUMBER);
	*/
}

//TODO No terminado
void leerUnPokemon(char* pathFiles, char* pokemon){

	char* metadataBin = "/Metadata.bin";
	// + 2 por el \0 y la "/"
	char* pathPokemonMetadata = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(metadataBin) + 2);

	strcpy(pathPokemonMetadata, pathFiles);
	strcat(pathPokemonMetadata, "/");
	// TODO se debe chequear antes de esto q la carpeta del pokemon existe | si no existe se crea
	strcat(pathPokemonMetadata, pokemon);
	// TODO idem punto anterior. Si el metadata no existe debe crearse
	strcat(pathPokemonMetadata, metadataBin);

	t_config* metadataPokemon;

	metadataPokemon = leerConfiguracion(pathPokemonMetadata);

	printf("Path pokemonMetadata: %s\n", pathPokemonMetadata);

	config_set_value(metadataPokemon, "OPEN" , "N");
	//config_remove_key(metadataPokemon, "DIRECTORY");
	int numeroSize = config_get_int_value(metadataPokemon, "SIZE");
	// TODO : Hacer que se pueda leer y escribir los archivos metadata.bin de los pokemons

	printf("%i\n", numeroSize);

	// path: {puntoMontaje}/Files/Pikachu/Metadata.bin

}

// Checkear si existe un determinado pokemon dentro de la carpeta Files/
int existeCarpetaPokemon(char* pokemon){

	char* pathCarpetaPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + 2);

	strcpy(pathCarpetaPokemon, pathFiles);
	strcat(pathCarpetaPokemon, "/");
	strcat(pathCarpetaPokemon, pokemon);

	//printf("Path carpeta pokemon: %s\n", pathCarpetaPokemon);

	int retorno;

	DIR* dir = opendir(pathCarpetaPokemon);
	if (dir) {
		// La carpeta existe
		retorno = 1;
	} else if (ENOENT == errno) {
		// La carpeta no existe
		retorno = 0;
	} else {
		printf("Hubo un error inesperado al abrir una carpeta pokemon D:");
		retorno = -1;
	}

	closedir(dir);

	free(pathCarpetaPokemon);
	return retorno;
}

// Crear la carpeta de un nuevo pokemon dentro de Files/pokemon
void crearCarpetaPokemon(char* pokemon){

	char* pathCarpetaPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + 2);

	strcpy(pathCarpetaPokemon, pathFiles);
	strcat(pathCarpetaPokemon, "/");
	strcat(pathCarpetaPokemon, pokemon);

	//printf("Path carpeta pokemon: %s\n", pathCarpetaPokemon);

	mkdir(pathCarpetaPokemon, 0777);

	free(pathCarpetaPokemon);
}

// Crea el Metadata.bin default de una carpeta Pokemon ya creada
void crearMetadataPokemon(char* pokemon){
	// pathCompleto = pathFiles/pokemon/metadata.bin
	char* pathMetadata = "/Metadata.bin";

	char* pathCompleto = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(pathMetadata) + 2);

	strcpy(pathCompleto, pathFiles);
	strcat(pathCompleto, "/");
	strcat(pathCompleto, pokemon);
	strcat(pathCompleto, pathMetadata);

	printf("Path Metadata: %s\n",pathCompleto);

	FILE* archivoMetadata = fopen( pathCompleto , "w" );
	fclose(archivoMetadata);

	t_config* datosMetadata = config_create(pathCompleto);

	// Valores default del metadata.bin
	config_set_value(datosMetadata, "DIRECTORY", "N");
	config_set_value(datosMetadata, "SIZE", "0");
	config_set_value(datosMetadata, "BLOCKS", "[]");
	config_set_value(datosMetadata, "OPEN", "N");

	config_save(datosMetadata);

	config_destroy(datosMetadata);

	//free(pathMetadata);
	free(pathCompleto);

}

// Crea un metadata con DIRECTORY=Y en pathCarpeta
void crearMetadataCarpeta(char* pathCarpeta){
	char* pathMetadata = "/Metadata.bin";

	char* pathCompleto = malloc(strlen(pathCarpeta) + strlen(pathMetadata) + 1);

	strcpy(pathCompleto, pathCarpeta);
	strcat(pathCompleto, pathMetadata);

	FILE* archivoMetadata = fopen( pathCompleto , "w" );
	fclose(archivoMetadata);

	t_config* datosMetadata = config_create(pathCompleto);

	config_set_value(datosMetadata, "DIRECTORY", "Y");

	config_save(datosMetadata);

	config_destroy(datosMetadata);

	//free(pathMetadata);
	free(pathCompleto);
}

void crearPokemonSiNoExiste(char* pokemon){
	if (!existeCarpetaPokemon(pokemon)){
		printf("No existe la carpeta: %s. Creando...\n", pokemon);
		crearCarpetaPokemon(pokemon);
		crearMetadataPokemon(pokemon);
		crearSemaforoPokemon(pokemon);
	} else {
		printf("Existe la carpeta: %s\n", pokemon);
	}
}

// Tomar los caracteres de coordenadas: Por ejemplo: 2-3=10 => 2-3
char* separarCoord(char* unString){
	char* token = malloc(strlen(unString));

	char* stringModificado = strdup(unString);

	token = strsep(&stringModificado, "=");

	return token;
}

// Retorna el numero de linea donde se encontro el pokemon, en caso de no encontrarlo devuelve -1
int encontrarCoords(int posX, int posY, char* lineaABuscar){
	// Primera linea = 0

	//char* lineaDePrueba = "123-23=10\n10-20=3\n15-20=20";

	int numLinea = 0;

	char** arrayLineas = string_split(lineaABuscar, "\n");

	char* lineaActual;
	while( (lineaActual = arrayLineas[numLinea]) != NULL){

		//printf("La linea es: %s\n", lineaActual);

		char* lineaConvertidaABuscar;
		char* lineaActualConvertida = separarCoord(lineaActual);

		asprintf(&lineaConvertidaABuscar, "%i-%i", posX, posY);

		if (strcmp(lineaConvertidaABuscar, lineaActualConvertida) == 0){
			return numLinea;
		}

		numLinea++;
	}

	return -1;
}

// Lee los bloques del metadata.bin de un pokemon existente
char** leerBloques(char* pokemon){
	char* metadataBin = "/Metadata.bin";

	// Path esperado: {pathMetadata}/Files/Pikachu/Metadata.bin

	char* pathMetadataPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(metadataBin) + 2);

	strcpy(pathMetadataPokemon, pathFiles);
	strcat(pathMetadataPokemon, "/");
	strcat(pathMetadataPokemon, pokemon);
	strcat(pathMetadataPokemon, metadataBin);

	//printf("Path Metadata Pokemon: %s\n", pathMetadataPokemon);

	t_config* datosMetadata = config_create(pathMetadataPokemon);

	if (datosMetadata == NULL){
		printf("No se ha podido leer los bloques del pokemon: %s\n", pokemon);
		exit(6);
	}

	return config_get_array_value(datosMetadata, "BLOCKS");
}

// Retorna el SIZE de un pokemon existente
int leerSizePokemon(char* pokemon){
	char* metadataBin = "/Metadata.bin";

	// Path esperado: {pathMetadata}/Files/Pikachu/Metadata.bin

	char* pathMetadataPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(metadataBin) + 2);

	strcpy(pathMetadataPokemon, pathFiles);
	strcat(pathMetadataPokemon, "/");
	strcat(pathMetadataPokemon, pokemon);
	strcat(pathMetadataPokemon, metadataBin);

	//printf("Path Metadata Pokemon: %s\n", pathMetadataPokemon);

	t_config* datosMetadata = config_create(pathMetadataPokemon);

	if (datosMetadata == NULL){
		printf("No se ha podido leer los bloques del pokemon: %s\n", pokemon);
		exit(6);
	}

	return config_get_int_value(datosMetadata, "SIZE");
}

// Fijar los bloques del metadata.bin del pokemon a los dados
void fijarBloquesA(char* pokemon, t_list* listaBloques){
	char* metadataBin = "/Metadata.bin";

	// Path esperado: {pathMetadata}/Files/Pikachu/Metadata.bin

	char* pathMetadataPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(metadataBin) + 2);

	strcpy(pathMetadataPokemon, pathFiles);
	strcat(pathMetadataPokemon, "/");
	strcat(pathMetadataPokemon, pokemon);
	strcat(pathMetadataPokemon, metadataBin);

	//printf("Path Metadata Pokemon: %s\n", pathMetadataPokemon);

	t_config* datosMetadata = config_create(pathMetadataPokemon);

	char* arrayBloques = crearStringArrayBloques(listaBloques);

	config_set_value(datosMetadata, "BLOCKS", arrayBloques);

	config_save(datosMetadata);

	config_destroy(datosMetadata);
}

void fijarSizeA(char* pokemon, int sizeEnBytes){
	char* metadataBin = "/Metadata.bin";

	// Path esperado: {pathMetadata}/Files/Pikachu/Metadata.bin

	char* pathMetadataPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(metadataBin) + 2);

	strcpy(pathMetadataPokemon, pathFiles);
	strcat(pathMetadataPokemon, "/");
	strcat(pathMetadataPokemon, pokemon);
	strcat(pathMetadataPokemon, metadataBin);

	//printf("Path Metadata Pokemon: %s\n", pathMetadataPokemon);

	t_config* datosMetadata = config_create(pathMetadataPokemon);

	char* sizeEnString;

	asprintf(&sizeEnString, "%i", sizeEnBytes);

	config_set_value(datosMetadata, "SIZE", sizeEnString);

	config_save(datosMetadata);

	config_destroy(datosMetadata);
}

// Generar un array de la forma [1,2,3,4] con la lista de bloques
char* crearStringArrayBloques(t_list* listaBloques){

	int tamanioTotalBytes = 0;

	int tamanioListaBloques = list_size(listaBloques);

	int i;
	for (i = 0; i< tamanioListaBloques ; i++){
		char* numeroEnString;

		int* numeroActual = list_get(listaBloques, i);

		asprintf(&numeroEnString, "%i", *numeroActual);

		// El tamanio del numero a insertar
		tamanioTotalBytes+= strlen(numeroEnString);

		//printf("El string es: %s\n", numeroEnString);
	}

	// La cantidad de comas
	tamanioTotalBytes += (tamanioListaBloques - 1 );
	// El "[" + "]" + \0
	tamanioTotalBytes += 3;

	printf("El size total es: %i\n", tamanioTotalBytes);

	char* stringCompleto = malloc(tamanioTotalBytes);

	strcpy(stringCompleto, "[");

	int j;

	for (j = 0; j< tamanioListaBloques ; j++){
		char* numeroEnString;

		int* numeroActual = list_get(listaBloques, j);

		asprintf(&numeroEnString, "%i", *numeroActual);

		strcat(stringCompleto, numeroEnString);

		if (j < tamanioListaBloques - 1){
			strcat(stringCompleto, ",");
		}
	}

	strcat(stringCompleto, "]");

	printf("String completo: %s", stringCompleto);

	return stringCompleto;

}

int hayAlgunBloque(char* pathFiles , char* pokemon){

	char** bloques = leerBloques(pokemon);

	if (bloques[0] != NULL){
		// Retorna uno en caso de haber algun bloque
		return 1;
	}

	// Retorna 0 en caso de no haber ningun bloque
	return 0;
}

// Generar linea a escribir en bloque a partir de posX posY y cantidad
char* generarLineaCoordsPokemon(int posX, int posY, int cantidad){
	char* stringAEscribir; // No hago malloc porque asprintf lo hace :)

	asprintf(&stringAEscribir, "%i-%i=%i\n", posX, posY, cantidad);

	//printf("String a escribir: %s", stringAEscribir);

	return stringAEscribir;
}

// Devuelve la cantidad de bloques que necesito para un string a escribir
int cantidadDeBloquesQueOcupa(int pesoEnBytes){

	int cantBloques = 0;

	// Se calcula cuantos bloques ocupa en base al peso y al tamaño
	while (pesoEnBytes > 0){

		//printf("pesoEnBytes: %i, ", pesoEnBytes);
		//printf("blockSize: %i\n", BLOCK_SIZE);

		pesoEnBytes-= BLOCK_SIZE;

		cantBloques++;
	}

	printf("La cantidad de bloques es: %i\n", cantBloques);

	return cantBloques;

}

t_list* separarStringEnBloques(char* lineaAEscribir, int cantBloques){

	t_list* listaStrings = list_create();

	int i;

	for (i = 0; i < cantBloques; i++){
		// Recortar la longitud del bloque del string
		char* miString = string_substring(lineaAEscribir, i * BLOCK_SIZE, BLOCK_SIZE);

		// Agrego el bloque recortado a la lista de strings
		list_add(listaStrings, miString);

		//printf("String en posicion: %i | %s\n", i , miString);
	}

	return listaStrings;

}

// Escribe la primera linea de un pokemon
void escribirLineaNuevaPokemon(char* pokemon, int posX, int posY, int cantidad){
	char* lineaAEscribir = generarLineaCoordsPokemon(posX, posY, cantidad);

	int pesoEnBytes = strlen(lineaAEscribir);

	int cantBloques = cantidadDeBloquesQueOcupa(pesoEnBytes);

	// Obtengo los bloques que necesito para guardar la info
	t_list* listaBloquesAOcupar = obtenerPrimerosLibresDeBitmap(cantBloques);

	// Testing para ver que se asignen bien los bloques
	// printf("Bitarray despues: \n");
	// printearBitArray(pathMetadata, BLOCKS);

	t_list* listaDatosBloques = separarStringEnBloques(lineaAEscribir, cantBloques);

	escribirLineasEnBloques(listaBloquesAOcupar, listaDatosBloques);

	// Fijar el array BLOCKS a por ejemeplo [1,2,3]
	fijarBloquesA(pokemon, listaBloquesAOcupar);
	// Fijar el SIZE=35
	fijarSizeA(pokemon, pesoEnBytes);

}

// Escribir las lineas en listaDatosBloques en los bloques listaBloquesAOcupar
void escribirLineasEnBloques(t_list* listaBloquesAOcupar, t_list* listaDatosBloques){
	if (list_size(listaBloquesAOcupar) != list_size(listaDatosBloques)){
		printf("ERROR | La cantidad de bloques a escribir debe coincidir con la cantidad de datos a escribir");
	}

	int i;

	// Se recorre la lista de bloques que se deben ocupar
	for (i = 0 ; i < list_size(listaBloquesAOcupar) ; i++){
		char* datoAEscribir = list_get(listaDatosBloques, i);
		int* bloqueAOcupar = list_get(listaBloquesAOcupar, i);
		//printf("En el bloque %i se escribira el dato:\n%s\n", *bloqueAOcupar , datoAEscribir );

		escribirDatoEnBloque(datoAEscribir, *bloqueAOcupar);

		// escribirDatoEnBloque(dato, bloque, pathBloques);
	}
}

// Escribir un dato en un bloque determinado
void escribirDatoEnBloque(char* dato, int numBloque){
	// Array de chars para meter el int convertido a array
	char* enteroEnLetras;

	// Genero un array de chars que es el numero de bloque pasado a string
	asprintf(&enteroEnLetras, "%i", numBloque);

	char* extension = ".bin";

	// 5 = longitud del numero + 1 por la /
	char* nombreArchivo = malloc( strlen(enteroEnLetras) + strlen(extension) + 2 );

	// El nombre de archivo es de la forma /{num}.bin | Ejemplo: /35.bin
	strcpy(nombreArchivo, "/");
	strcat(nombreArchivo, enteroEnLetras);
	strcat(nombreArchivo, extension);

	// Creo una copia del path de /Blocks para no modificarlo
	char* pathBloque = malloc(strlen(pathBloques) + strlen(nombreArchivo) + 1);

	// Le pego el path de /Blocks
	strcpy(pathBloque, pathBloques);

	// Le pego el valor hardcodeado 1.bin
	strcat(pathBloque, nombreArchivo);

	FILE* bloque = fopen( pathBloque , "w" );

	fwrite(dato, strlen(dato) + 1, 1, bloque);

	fclose(bloque);

	//printf("Path bloque: %s", pathBloque);


}

void suscribirseAColas(int32_t socket){

	//Uso una estructura para guardar el numero de cola al que me quiero subscribir y mandarlo a la funcion mandar_mensaje
	Suscripcion* estructuraSuscribirse = malloc(sizeof(Suscripcion));

	estructuraSuscribirse->numeroCola = NEW;

	//mandamos el mensaje pidiendo suscribirse a la cola
	mandar_mensaje(estructuraSuscribirse, SUSCRIPCION, socket);

	estructuraSuscribirse->numeroCola = GET;
	//mandamos el mensaje pidiendo suscribirse a la cola
	mandar_mensaje(estructuraSuscribirse, SUSCRIPCION, socket);


	estructuraSuscribirse->numeroCola = CATCH;
	//mandamos el mensaje pidiendo suscribirse a la cola
	mandar_mensaje(estructuraSuscribirse, SUSCRIPCION, socket);

	free(estructuraSuscribirse);
}

int conectarseABroker(char* IP_BROKER, char* PUERTO_BROKER, t_log* logger){
	int socket = establecer_conexion(IP_BROKER, PUERTO_BROKER);//creo conexión con el Broker.

	resultado_de_conexion(socket, logger, "BROKER");

	if (socket != -1) suscribirseAColas(socket);

	return socket;
}

int escucharGameBoy(char* IP_GAMECARD, char* PUERTO_GAMECARD, t_log* logger){
	int socket = establecer_conexion(IP_GAMECARD, PUERTO_GAMECARD);//creo conexión con el Broker.

	resultado_de_conexion(socket, logger, "SOCKET_ESCUCHA");

	return socket;
}

void esperarMensajes(int socket, char* IP_BROKER, char* PUERTO_BROKER, t_log* logger, int TIEM_REIN_CONEXION){

	codigo_operacion cod_op;
	uint32_t desconexion = 1;

	int32_t recibidos = recv(socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
	bytesRecibidos(recibidos);


	while((recibidos == -1) || (desconexion == -1))

	{
		sleep(TIEM_REIN_CONEXION);
		desconexion = conectarseABroker(IP_BROKER, PUERTO_BROKER, logger);
		if(desconexion != -1)
		{
			recibidos = recv(socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
			bytesRecibidos(recibidos);
		}
	}

	uint32_t tamanioDatos;

	switch (cod_op)
	{
	 	case NEW: ;
			New* mensajeNew = malloc(sizeof(New));
			recibir_mensaje(mensajeNew, cod_op, socket, &tamanioDatos);

			break;
		case GET: ;
			Get* mensajeGet = malloc(sizeof(Get));
			recibir_mensaje(mensajeGet, cod_op, socket, &tamanioDatos);

			break;
		case CATCH: ;
			Catch* mensajeCatch = malloc(sizeof(Catch));
			recibir_mensaje(mensajeCatch, cod_op, socket, &tamanioDatos);

			break;
		default:
			printf("No deberia entrar aca D:");
			break;
	}

}



void process_request(codigo_operacion cod_op, int32_t socket_cliente) {
	New* mensajeNew;
	Get* mensajeGet;
	Catch* mensajeCatch;
	uint32_t tamanioDatos;

		switch (cod_op) {
		case NEW:
			mensajeNew  = malloc(sizeof(New));
			recibir_mensaje(mensajeNew, cod_op, socket_cliente, &tamanioDatos);

			//ya te llegaron los datos y llamas a tus funciones

			printf("Termine de recibir un mensaje NEW\n");

			break;
		case GET:
			mensajeGet = malloc(sizeof(Get));
			recibir_mensaje(mensajeGet, cod_op, socket_cliente, &tamanioDatos);
			printf("Termine de recibir un mensaje GET\n");

			//ya te llegaron los datos y llamas a tus funciones

			break;
		case CATCH:
			mensajeCatch = malloc(sizeof(Catch));
			recibir_mensaje(mensajeCatch, cod_op, socket_cliente, &tamanioDatos);

			printf("Termine de recibir un mensaje CATCH\n");

			//ya te llegaron los datos y llamas a tus funciones

			break;
		}
}


void serve_client(int32_t* socket)
{
	codigo_operacion cod_op;
	int32_t recibidos = recv(*socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
	bytesRecibidos(recibidos);
	if(recibidos == -1)
	{
		cod_op = -1;
	}

	process_request(cod_op, *socket);
}

void esperar_conexiones(int32_t socket_servidor)
{
	struct sockaddr_in dir_cliente;

	// Entero lindo para el socket (es un int)
	socklen_t tam_direccion = sizeof(struct sockaddr_in);

	int32_t socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);//espera una conexion

	serve_client(&socket_cliente);
}

void escuchoSocket(int32_t miSocket)
{
	//acepto conexiones entrantes
	//struct sockaddr_in direccionConexionEntrante;
	//uint32_t tamanioConexionEntrante;
//	int32_t conexionEntrante = accept(miSocket, (void*) &direccionConexionEntrante, &tamanioConexionEntrante);

	//printf ("Me llego una conexion (GameBoy): %i", conexionEntrante);

	while(1)
	{
		esperar_conexiones(miSocket);
	}
}

t_list* convertirAListaDeEnterosDesdeChars(char** listaDeChars){
	int cantidadNumeros = 0;
	t_list* miLista = list_create();

	while (listaDeChars[cantidadNumeros] != NULL){
		//printf("lista de chars: %s \n", listaDeChars[cantidadNumeros]);

		// Al hacer malloc me aseguro que esta direccion de memoria no va a ser pisada
		int* charConvertido = malloc(sizeof(int));

		int nombre = atoi(listaDeChars[cantidadNumeros]);

		memcpy(charConvertido, &nombre, sizeof(int));

		list_add(miLista, charConvertido);
		cantidadNumeros++;
	}

	return miLista;
}

char* leerContenidoDeUnBloque(char* bloqueALeer, int cantBytesALeer){

	char* pathDeArchivos = malloc(strlen(pathBloques) + strlen(bloqueALeer) + 6); // 1 del /, 4 del .bin, 1 del fin de string
	strcpy(pathDeArchivos, pathBloques);
	strcat(pathDeArchivos, "/");
	strcat(pathDeArchivos, bloqueALeer);
	strcat(pathDeArchivos, ".bin");

	char* datosLeidos = malloc(cantBytesALeer + 1);

	FILE* myFile = fopen(pathDeArchivos, "r");

	//printf("Bytes a leer: %i\n", cantBytesALeer);

	fread(datosLeidos, cantBytesALeer + 1, 1, myFile);

	fclose(myFile);

	//printf("Linea leida:\n %s\n", datosLeidos);

	return datosLeidos;


	// cantidad a leer esta definido asi ya que la iteracion, y por ende la lógica detras de cual es el último bloque, se debe hacer en el loop.
	// como el último bloque tiene menos cosas, este tiene que ser leido con un tamaño diferente.
}

int cantidadDeElementosEnArray(char** array){
	int i = 0;
	while(array[i] != NULL){
		i++;
	}
	return i;
}

// bloquesALeer = BLOCKS cantidadALeerEnBytes = SIZE (del metadata.bin)
char* leerContenidoBloquesPokemon(char** bloquesALeer, int cantidadALeerEnBytes) {

	char* stringARetornar = malloc(cantidadALeerEnBytes + 1);

	int bytesLeidos = 0;

	int cantidadDeBloquesALeer = cantidadDeElementosEnArray(bloquesALeer);

	int i;

	for(i = 0; i < cantidadDeBloquesALeer; i++) {

		int cantidadDeBytesALeer;

		if(bytesLeidos+BLOCK_SIZE <= cantidadALeerEnBytes) {
			cantidadDeBytesALeer = BLOCK_SIZE;
			}else{
			cantidadDeBytesALeer = cantidadALeerEnBytes - bytesLeidos;
		}

		char* aux = leerContenidoDeUnBloque( bloquesALeer[i], cantidadDeBytesALeer);

		if(i == 0){
			strcpy(stringARetornar, aux);
		}else{
			strcat(stringARetornar, aux);
		}

		bytesLeidos += cantidadDeBytesALeer;

		free(aux);
	}

	return stringARetornar;

}


void comenzarConexionConBroker(datosHiloBroker* datos){

	// CONEXION Y ESPERAR MENSAJES DE BROKER
	char* IP_BROKER = datos->IP_BROKER;
	char* PUERTO_BROKER = datos->PUERTO_BROKER;
	int TIEM_REIN_CONEXION = datos->TIEM_REIN_CONEXION;

	t_log* logger = datos->logger;

	int socketBroker = -1;

	socketBroker = conectarseABroker(IP_BROKER, PUERTO_BROKER, logger);

	while (socketBroker == -1){
		sleep(TIEM_REIN_CONEXION);
		socketBroker = conectarseABroker(IP_BROKER, PUERTO_BROKER, logger);
	}

	while(1)
	{
	esperarMensajes(socketBroker, IP_BROKER, PUERTO_BROKER, logger, TIEM_REIN_CONEXION);
	}

	puts("sali de esperar mensaje");

}

void comenzarEscuchaGameBoy(){
	int32_t socketoide = reservarSocket("5001"); //tirarle la key de la config

	escuchoSocket(socketoide); //escuchando al gameboy
	close(socketoide);
}

// Pegar una nueva linea de pokemon al final de lineas
char* agregarNuevoPokemonALineas(int posX, int posY, int cantidad, char* lineas){

	char* lineaParaAgregar = generarLineaCoordsPokemon(posX, posY, cantidad);

	char* lineaARetornar = malloc(strlen(lineas) + strlen(lineaParaAgregar) + 1);

	strcpy(lineaARetornar, lineas);
	strcat(lineaARetornar, lineaParaAgregar);

	//printf("LineaARetornar: %s", lineaARetornar);

	return lineaARetornar;
}

// Retorna una lista con la lista anterior mas los bloques nuevos pedidos
void suamarBloquesExtraALista(t_list* listaBloques, int cantidadBloquesExtra){

	t_list* bloquesExtraPedidos = obtenerPrimerosLibresDeBitmap(cantidadBloquesExtra);

	list_add_all(listaBloques, bloquesExtraPedidos);
}

void printearListaDeEnteros(t_list* lista){

	int i;
	for(i=0;i<list_size(lista); i++){
		int* item = list_get(lista, i);
		printf("El item en pos %i es %i\n", i, *item);
	}

}

void enviarMensajeAppeared(char* pokemon, int posX, int posY, int IDMensaje){
	printf("Se enviara el siguiente mensaje al broker (cola appeared):\n");
	printf("Pokemon: %s\n", pokemon);
	printf("PosX: %i\n", posX);
	printf("PosY: %i\n", posY);
	printf("ID Mensaje: %i\n", IDMensaje);
}


char* reemplazarLineaDePokemon(char* texto, int posX, int posY, int cantidad) {

	char* stringAEncontrar;
	char* stringAEscribir;
	asprintf(&stringAEncontrar, "%i-%i=", posX, posY);
    asprintf(&stringAEscribir, "%i-%i=%i", posX, posY, cantidad);

	char* aux = strstr(texto, stringAEncontrar);

	int posicionDeInicioDeLineaAModificar = aux - texto;


	//primera mitad del string
	char* primeraMitadDelString = malloc(sizeof(char)*posicionDeInicioDeLineaAModificar + 1);
	strncpy(primeraMitadDelString, texto, posicionDeInicioDeLineaAModificar);


	//segunda mitad del string

	char* segundaMitadDelString = strstr(aux, "\n");


	//los pego juntos

	char* stringARetornar = malloc(strlen(primeraMitadDelString) + strlen(stringAEscribir) + strlen(segundaMitadDelString) + 1);
	strcpy(stringARetornar, primeraMitadDelString);
	strcat(stringARetornar, stringAEscribir);
	strcat(stringARetornar, segundaMitadDelString);


	free(stringAEscribir);
	free(stringAEncontrar);
	free(primeraMitadDelString);

	return stringARetornar;
}

void mensajeNew(char* pokemon, int posX, int posY, int cantidad){

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
		printf("Ya habia bloques, leyendo...\n");

		char** bloques = leerBloques(pokemon);

		int cantidadBytesALeer = leerSizePokemon(pokemon);

		char* lineasLeidas = leerContenidoBloquesPokemon(bloques, cantidadBytesALeer);

		// Si no se encuentra la linea buscada entonces se debe agregar al final

		if (encontrarCoords(posX, posY, lineasLeidas) == -1){
			// TODO Fixear :)

			printf("La linea NO fue encontrada... pegando al final\n");

			char* lineasNuevasMasPokemon = agregarNuevoPokemonALineas(posX, posY, cantidad, lineasLeidas);

			// TODO Esta lista de bloques queda rota al leer por alguna razon D:
			t_list* listaBloques = list_create();
			listaBloques = convertirAListaDeEnterosDesdeChars(bloques);

			//printf("Printeando lista de bloques:");
			//printearListaDeEnteros(listaBloques);

			int cantidadBloquesRequeridos = cantidadDeBloquesQueOcupa(strlen(lineasNuevasMasPokemon));

			int cantidadBloquesActual = cantidadDeElementosEnArray(bloques);

			if (cantidadBloquesRequeridos == cantidadBloquesActual){
			// FUNCIONA BIEN

				// La cantidad se mantiene igual, solo escribir los bloques
				printf("No se necesitan bloques extra... solo escribir\n");

				// Generar lista con los datos a escribir en los bloques
				t_list* listaDatos = separarStringEnBloques(lineasNuevasMasPokemon, cantidadBloquesRequeridos);

				// Escribir los datos en los bloques correspondientes
				escribirLineasEnBloques(listaBloques, listaDatos);

				// Fijo el SIZE=60
				fijarSizeA(pokemon, strlen(lineasNuevasMasPokemon));


			} else if (cantidadBloquesRequeridos > cantidadBloquesActual){

				printf("Se necesitan mas bloques... pidiendo\n");

				// Cantidad de bloques extra que se deben pedir
				int cantidadBloquesExtra = cantidadBloquesRequeridos - cantidadBloquesActual;

				// Sumo los bloques extra a la listaBloques original
				suamarBloquesExtraALista(listaBloques, cantidadBloquesExtra);

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
			printf("La linea fue encontrada, se la debe modificar...");

			char* lineasConLineaReemplazada = reemplazarLineaDePokemon(lineasLeidas, posX, posY, cantidad);

			printf("Lineas con linea reemplazada: %s", lineasConLineaReemplazada);

		}


	} else {
		printf("No habia bloques... generando de 0\n");
		escribirLineaNuevaPokemon(pokemon, posX, posY, cantidad);
	}

	// Cerrar el archivo luego de usarlo
	cerrarArchivoPokemon(pokemon);

	enviarMensajeAppeared(pokemon, posX, posY, 3); // TODO Num 3 HARDCODEADO | En realidad toma el ID como parametro esta funcion
}

// Abrir un archivo de un pokemon existente
void abrirArchivoPokemon(char* pokemon){
	char* metadataBin = "/Metadata.bin";

	// Path esperado: {pathMetadata}/Files/Pikachu/Metadata.bin

	char* pathMetadataPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(metadataBin) + 2);

	strcpy(pathMetadataPokemon, pathFiles);
	strcat(pathMetadataPokemon, "/");
	strcat(pathMetadataPokemon, pokemon);
	strcat(pathMetadataPokemon, metadataBin);

	while(1){

		waitSemaforoPokemon(pokemon);

		t_config* datosMetadata = config_create(pathMetadataPokemon);

		if (datosMetadata == NULL){
			printf("No se ha podido leer los bloques del pokemon: %s\n", pokemon);
			exit(6);
		}

		char* estaAbierto = config_get_string_value(datosMetadata, "OPEN");

		if (strcmp(estaAbierto, "N") == 0){
			config_set_value(datosMetadata, "OPEN", "Y");
			config_save(datosMetadata);

			printf("El archivo fue abierto correctamente\n");
			signalSemaforoPokemon(pokemon);

			config_destroy(datosMetadata);
			break;
		}

		signalSemaforoPokemon(pokemon);
		config_destroy(datosMetadata);

		printf("No pude abrir el archivo, entrando en tiempo de espera\n");

		sleep(TIEM_REIN_OPERACION);

	}

}

void cerrarArchivoPokemon(char* pokemon){
	char* metadataBin = "/Metadata.bin";

	// Path esperado: {pathMetadata}/Files/Pikachu/Metadata.bin

	char* pathMetadataPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(metadataBin) + 2);

	strcpy(pathMetadataPokemon, pathFiles);
	strcat(pathMetadataPokemon, "/");
	strcat(pathMetadataPokemon, pokemon);
	strcat(pathMetadataPokemon, metadataBin);

	waitSemaforoPokemon(pokemon);

	t_config* datosMetadata = config_create(pathMetadataPokemon);

	if (datosMetadata == NULL){
		printf("No se ha podido leer los bloques del pokemon: %s\n", pokemon);
		exit(6);
	}

	config_set_value(datosMetadata, "OPEN", "N");
	config_save(datosMetadata);

	printf("El archivo fue cerrado correctamente\n");
	signalSemaforoPokemon(pokemon);

	config_destroy(datosMetadata);

}

// Testing semaforos pokemon
//void abrirArchivo1(){
//	char* pikachu = "Pikachu";
//
//	abrirArchivoPokemon(pikachu);
//
//	printf("Hilo 1 abrio el archivo pikachu\n");
//
//	printf("SLEEP 5 SEGUNDOS HILO 1\n");
//	sleep(5);
//
//	cerrarArchivoPokemon(pikachu);
//
//	printf("Hilo 1 cerro el archivo pikachu\n");
//}
//
//void abrirArchivo2(){
//	char* pikachu = "Pikachu";
//
//	abrirArchivoPokemon(pikachu);
//
//	printf("Hilo 2 abrio el archivo pikachu\n");
//
//	printf("SLEEP 5 SEGUNDOS HILO 2\n");
//	sleep(5);
//
//	cerrarArchivoPokemon(pikachu);
//
//	printf("Hilo 2 cerro el archivo pikachu\n");
//}
//
//void abrirArchivo3(){
//	char* pikachu = "Pikachu";
//
//	abrirArchivoPokemon(pikachu);
//
//	printf("Hilo 3 abrio el archivo pikachu\n");
//
//	printf("SLEEP 5 SEGUNDOS HILO 3\n");
//	sleep(5);
//
//	cerrarArchivoPokemon(pikachu);
//
//	printf("Hilo 3 cerro el archivo pikachu\n");
//}




int main(void) {
	t_config* config = NULL;
	int TIEM_REIN_CONEXION;
	char* PUNTO_MONTAJE;
	char* IP_BROKER;
	char* PUERTO_BROKER;

	semBitmap = malloc(sizeof(sem_t));

	// Inicializar semaforo bitmap
	sem_init(semBitmap, 0, 1);

	//Inicializar lista de semaforos pokemon
	listaSemPokemon = list_create();

	leerConfig(&TIEM_REIN_CONEXION, &TIEM_REIN_OPERACION, &PUNTO_MONTAJE, &IP_BROKER, &PUERTO_BROKER, config);

	// Testing
	//printf("Path punto montaje: %s\n", PUNTO_MONTAJE);

	// Inicializacion del logger... todavia no es necesario

	t_log* logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/GameCard/Logs/GameCard.log", "GAMECARD");

	// puntoMontaje/Blocks
	pathBloques = crearCarpetaEn(PUNTO_MONTAJE, "/Blocks");
	// puntoMontaje/Metadata
	pathMetadata = crearCarpetaEn(PUNTO_MONTAJE, "/Metadata");
	// puntoMontaje/Files
	pathFiles = crearCarpetaEn(PUNTO_MONTAJE, "/Files");

	// Testing
    // printf("%s\n", pathBloques);
    // printf("%s\n", pathMetadata);
    // printf("%s\n", pathFiles);

	char* MAGIC_NUMBER;
	t_config* metadataBin = NULL;

	// Funcion para leer metadata.bin
	leerMetadataBin(pathMetadata, &BLOCKS, &BLOCK_SIZE, &MAGIC_NUMBER, metadataBin);

	if (BLOCKS % 8 != 0){
		printf("[Error] La cantidad de bloques debe ser multiplo de 8");
		exit(5);
	}

	// Si no existe el filesystem
	if ( existeFilesystem(pathMetadata) == 0){
		printf("No existe filesystem... inicializando.\n");
		inicializarFileSystem(pathBloques, pathFiles, pathMetadata, BLOCKS);
	} else {
		printf("El filesystem ya existe. No se debe inicializar.\n");

		// Escanear pokemones existentes para crear semaforos
		crearSemaforosDePokemonesExistentes();

		printearSemaforosExistentes();
	}

	// -- Desde aca el filesystem ya está inicializado --


	// Vaciar un bloque del bitarray (hacerlo = 0)

	// Testing
	//printf("Bitarray antes: \n");
	//printearBitArray(pathMetadata, BLOCKS);

	char* pikachu = "Pikachu";
	char* jorge = "Jorge";
	char* fruta = "Fruta";
	char* bulbasaur = "Bulbasaur";

	mensajeNew(jorge, 1,15,3);
	mensajeNew(jorge, 1,14,3);
	mensajeNew(jorge, 1,20,3);
	mensajeNew(jorge, 1,21,3);
	mensajeNew(jorge, 1,23,3);
	mensajeNew(jorge, 1,5,3);
	mensajeNew(jorge, 1,3,3);
	mensajeNew(jorge, 1,7,3);
	mensajeNew(jorge, 32,5,3);

//	mensajeNew(pikachu, 1,15,3);
//	mensajeNew(pikachu, 1,14,3);
//	mensajeNew(pikachu, 1,20,3);
//	mensajeNew(pikachu, 1,21,3);
//	mensajeNew(pikachu, 1,23,3);
//	mensajeNew(pikachu, 1,5,3);
//	mensajeNew(pikachu, 1,3,3);
//	mensajeNew(pikachu, 1,7,3);
//	mensajeNew(pikachu, 32,5,3);
//
//	mensajeNew(fruta, 1,15,3);
//	mensajeNew(fruta, 1,14,3);
//	mensajeNew(fruta, 1,20,3);
//	mensajeNew(fruta, 1,21,3);
//	mensajeNew(fruta, 1,23,3);
//	mensajeNew(fruta, 1,5,3);
//	mensajeNew(fruta, 1,3,3);
//	mensajeNew(fruta, 1,7,3);
//	mensajeNew(fruta, 32,5,3);
//
//	mensajeNew(bulbasaur, 1,15,3);
//	mensajeNew(bulbasaur, 1,14,3);
//	mensajeNew(bulbasaur, 1,20,3);
//	mensajeNew(bulbasaur, 1,21,3);
//	mensajeNew(bulbasaur, 1,23,3);
//	mensajeNew(bulbasaur, 1,5,3);
//	mensajeNew(bulbasaur, 1,3,3);
//	mensajeNew(bulbasaur, 1,7,3);
//	mensajeNew(bulbasaur, 32,5,3);

	mensajeNew(jorge, 33,3,3);
	mensajeNew(jorge, 34,7,3);
	mensajeNew(jorge, 35,7,3);
	mensajeNew(jorge, 36,7,3);
	mensajeNew(jorge, 37,3,3);
	mensajeNew(jorge, 38,7,3);
	mensajeNew(jorge, 40,7,3);
	mensajeNew(jorge, 43,7,3);
	mensajeNew(jorge, 55,3,3);
	mensajeNew(jorge, 60,7,3);
	mensajeNew(jorge, 130,7,3);
	mensajeNew(jorge, 200,7,3);
	mensajeNew(jorge, 5001,3,3);
	mensajeNew(jorge, 1000,7,3);
	mensajeNew(jorge, 10000,700,300);
	mensajeNew(jorge, 100000,700,300);

//	mensajeNew(pikachu, 33,3,3);
//	mensajeNew(pikachu, 34,7,3);
//	mensajeNew(pikachu, 35,7,3);
//	mensajeNew(pikachu, 36,7,3);
//	mensajeNew(pikachu, 37,3,3);
//	mensajeNew(pikachu, 38,7,3);
//	mensajeNew(pikachu, 40,7,3);
//	mensajeNew(pikachu, 43,7,3);
//	mensajeNew(pikachu, 55,3,3);
//	mensajeNew(pikachu, 60,7,3);
//	mensajeNew(pikachu, 130,7,3);
//	mensajeNew(pikachu, 200,7,3);
//	mensajeNew(pikachu, 5001,3,3);
//	mensajeNew(pikachu, 1000,7,3);
//	mensajeNew(pikachu, 10000,700,300);
//	mensajeNew(pikachu, 100000,700,300);
//
//	mensajeNew(fruta, 33,3,3);
//	mensajeNew(fruta, 34,7,3);
//	mensajeNew(fruta, 35,7,3);
//	mensajeNew(fruta, 36,7,3);
//	mensajeNew(fruta, 37,3,3);
//	mensajeNew(fruta, 38,7,3);
//	mensajeNew(fruta, 40,7,3);
//	mensajeNew(fruta, 43,7,3);
//	mensajeNew(fruta, 55,3,3);
//	mensajeNew(fruta, 60,7,3);
//	mensajeNew(fruta, 130,7,3);
//	mensajeNew(fruta, 200,7,3);
//	mensajeNew(fruta, 5001,3,3);
//	mensajeNew(fruta, 1000,7,3);
//	mensajeNew(fruta, 10000,700,300);
//	mensajeNew(fruta, 100000,700,300);
//
//	mensajeNew(bulbasaur, 33,3,3);
//	mensajeNew(bulbasaur, 34,7,3);
//	mensajeNew(bulbasaur, 35,7,3);
//	mensajeNew(bulbasaur, 36,7,3);
//	mensajeNew(bulbasaur, 37,3,3);
//	mensajeNew(bulbasaur, 38,7,3);
//	mensajeNew(bulbasaur, 40,7,3);
//	mensajeNew(bulbasaur, 43,7,3);
//	mensajeNew(bulbasaur, 55,3,3);
//	mensajeNew(bulbasaur, 60,7,3);
//	mensajeNew(bulbasaur, 130,7,3);
//	mensajeNew(bulbasaur, 200,7,3);
//	mensajeNew(bulbasaur, 5001,3,3);
//	mensajeNew(bulbasaur, 1000,7,3);
//	mensajeNew(bulbasaur, 10000,700,300);
//	mensajeNew(bulbasaur, 100000,700,300);


	// Testing semaforos pokemon
//
//
//	crearPokemonSiNoExiste(pikachu);
//	crearPokemonSiNoExiste(bulbasaur);
//	crearPokemonSiNoExiste(jorge);
//	crearPokemonSiNoExiste(fruta);
//
//	printearSemaforosExistentes();



//	pthread_t hiloTesting1;
//	pthread_t hiloTesting2;
//	pthread_t hiloTesting3;
//
//	pthread_create(&hiloTesting1, NULL, (void*)abrirArchivo1, NULL);
//	pthread_create(&hiloTesting2, NULL, (void*)abrirArchivo2, NULL);
//	pthread_create(&hiloTesting3, NULL, (void*)abrirArchivo3, NULL);
//
//	pthread_join(hiloTesting1, NULL);
//	pthread_join(hiloTesting2, NULL);
//	pthread_join(hiloTesting3, NULL);

	// Crear 3 hilos

	// Testing semaforos
//	crearPokemonSiNoExiste(pikachu);
//	crearPokemonSiNoExiste(bulbasaur);
//	crearPokemonSiNoExiste(jorge);
//	crearPokemonSiNoExiste(fruta);
//
//	leerSemaforosLista();
//
//	waitSemaforoPokemon(bulbasaur);
//	waitSemaforoPokemon(fruta);
//
//	leerSemaforosLista();
//
//	signalSemaforoPokemon(bulbasaur);
//
//	signalSemaforoPokemon(fruta);
//
//	leerSemaforosLista();


//	char* bloque1 = "1";
//
//	char* bloqueLeido = leerContenidoDeUnBloque(pathBloques, bloque1, 8);
//
//	printf("Bloque leido: %s", bloqueLeido);

	// ****************************************************************

	// Levanto hilo para escuchar broker
//	datosHiloBroker datosBroker = {IP_BROKER, PUERTO_BROKER, TIEM_REIN_CONEXION, logger};
//
//	pthread_t hiloBroker;
//
//	pthread_create(&hiloBroker, NULL, (void*)comenzarConexionConBroker, &datosBroker);

	// ****************************************************************
	// Levanto hilo para escuchar mensajes directos de gameboy

//	pthread_t hiloGameBoy;
//
//	pthread_create(&hiloGameBoy, NULL, (void*)comenzarEscuchaGameBoy, NULL);
//
//	// CIERRO HILOS
//	//pthread_join(hiloBroker, NULL);
//	pthread_join(hiloGameBoy, NULL);

	// ****************************************************************

	/* MANDAR MENSAJE A UNA COLA
	Appeared* estructura = malloc(sizeof(Appeared));

	estructura->posPokemon.x =21;

	mandar_mensaje(estructura,APPEARED,socketBroker);
	*/

	//****************************************************************
	return EXIT_SUCCESS;
}
