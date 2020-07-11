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
int existeCarpetaPokemon(char* pathFiles, char* pokemon){

	char* pathCarpetaPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + 2);

	strcpy(pathCarpetaPokemon, pathFiles);
	strcat(pathCarpetaPokemon, "/");
	strcat(pathCarpetaPokemon, pokemon);

	printf("Path carpeta pokemon: %s\n", pathCarpetaPokemon);

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
void crearCarpetaPokemon(char* pathFiles, char* pokemon){

	char* pathCarpetaPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + 2);

	strcpy(pathCarpetaPokemon, pathFiles);
	strcat(pathCarpetaPokemon, "/");
	strcat(pathCarpetaPokemon, pokemon);

	//printf("Path carpeta pokemon: %s\n", pathCarpetaPokemon);

	mkdir(pathCarpetaPokemon, 0777);

	free(pathCarpetaPokemon);
}

// Crea el Metadata.bin default de una carpeta Pokemon ya creada
void crearMetadataPokemon(char* pathFiles, char* pokemon){
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

void crearPokemonSiNoExiste(char* pathFiles, char* pokemon){
	if (!existeCarpetaPokemon(pathFiles, pokemon)){
		printf("No existie la carpeta: %s. Creando...\n", pokemon);
		crearCarpetaPokemon(pathFiles, pokemon);
		crearMetadataPokemon(pathFiles, pokemon);
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

// Obsoleto | Se debe modificar para que se lean los bloques apendeen y luego
// realizar la misma logica pero con un char* con los datos, no de esta manera
// Se debe agregar de buscar las coordenadas dentro de un pokemon
// no dentro de bloque de prueba
int encontrarCoords(int posX, int posY){
	char* line;
	size_t len = 0;
	ssize_t read;

	FILE* fp = fopen("34.bin", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	char* coords;

	asprintf(&coords, "%i-%i" , posX, posY);

	int lineaActual = 0;
	int retorno = -1;

	//char* coordenadasLineaActual = "5-4=50";

	while ( (read = getline(&line, &len, fp) ) != -1) {
		printf("Longitud de linea: %i:\n", read);
		printf("La linea es: %s\n", line);

		// La linea leida es las coordenadas que quiero encontrar

		char* coordenadasLineaActual = separarCoord(line);

		if (strcmp( coordenadasLineaActual, coords ) == 0)
		{
			retorno = lineaActual;
		}

		lineaActual ++;
	}

	printf("La cantidad de lineas totales es: %i", lineaActual);

	fclose(fp);
	free(line);

	return retorno;
}

// Lee los bloques del metadata.bin de un pokemon existente
char** leerBloques(char* pathFiles , char* pokemon){
	char* metadataBin = "/Metadata.bin";

	// Path esperado: {pathMetadata}/Files/Pikachu/Metadata.bin

	char* pathMetadataPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(metadataBin) + 2);

	strcpy(pathMetadataPokemon, pathFiles);
	strcat(pathMetadataPokemon, "/");
	strcat(pathMetadataPokemon, pokemon);
	strcat(pathMetadataPokemon, metadataBin);

	printf("Path Metadata Pokemon: %s\n", pathMetadataPokemon);

	t_config* datosMetadata = config_create(pathMetadataPokemon);

	if (datosMetadata == NULL){
		printf("No se ha podido leer los bloques del pokemon: %s\n", pokemon);
		exit(6);
	}

	return config_get_array_value(datosMetadata, "BLOCKS");
}

// Fijar los bloques del metadata.bin del pokemon a los dados
void fijarBloquesYPesoA(char* pokemon, char* pathFiles, t_list* listaBloques, int pesoEnBytes){
	char* metadataBin = "/Metadata.bin";

	// Path esperado: {pathMetadata}/Files/Pikachu/Metadata.bin

	char* pathMetadataPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + strlen(metadataBin) + 2);

	strcpy(pathMetadataPokemon, pathFiles);
	strcat(pathMetadataPokemon, "/");
	strcat(pathMetadataPokemon, pokemon);
	strcat(pathMetadataPokemon, metadataBin);

	printf("Path Metadata Pokemon: %s\n", pathMetadataPokemon);

	t_config* datosMetadata = config_create(pathMetadataPokemon);

	char* arrayBloques = crearStringArrayBloques(listaBloques);

	config_set_value(datosMetadata, "BLOCKS", arrayBloques);

	char* pesoEnBytesAChar;

	asprintf(&pesoEnBytesAChar, "%i", pesoEnBytes);

	config_set_value(datosMetadata, "SIZE", pesoEnBytesAChar);

	config_save(datosMetadata);
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

	//printf("El size total es: %i\n", tamanioTotalBytes);

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

	//printf("String completo: %s", stringCompleto);

	return stringCompleto;

}

int hayAlgunBloque(char* pathFiles , char* pokemon){

	char** bloques = leerBloques(pathFiles , pokemon);

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

	printf("String a escribir: %s", stringAEscribir);

	return stringAEscribir;
}

// Devuelve la cantidad de bloques que necesito para un string a escribir
int cantidadDeBloquesQueOcupa(int pesoEnBytes, int BLOCK_SIZE){

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

t_list* separarStringEnBloques(char* lineaAEscribir, int cantBloques, int blockSize){

	t_list* listaStrings = list_create();

	int i;

	for (i = 0; i < cantBloques; i++){
		// Recortar la longitud del bloque del string
		char* miString = string_substring(lineaAEscribir, i * blockSize, blockSize);

		// Agrego el bloque recortado a la lista de strings
		list_add(listaStrings, miString);

		//printf("String en posicion: %i | %s\n", i , miString);
	}

	return listaStrings;

}

// Escribe la primera linea de un pokemon
void escribirLineaNuevaPokemon(char* pokemon, int posX, int posY, int cantidad, int BLOCK_SIZE, int BLOCKS, char* pathMetadata, char* pathBloques, char* pathFiles){
	char* lineaAEscribir = generarLineaCoordsPokemon(posX, posY, cantidad);

	int pesoEnBytes = strlen(lineaAEscribir);

	int cantBloques = cantidadDeBloquesQueOcupa(pesoEnBytes, BLOCK_SIZE);

	// Obtengo los bloques que necesito para guardar la info
	t_list* listaBloquesAOcupar = obtenerPrimerosLibresDeBitmap(pathMetadata, BLOCKS, cantBloques);

	// Testing para ver que se asignen bien los bloques
	// printf("Bitarray despues: \n");
	// printearBitArray(pathMetadata, BLOCKS);

	t_list* listaDatosBloques = separarStringEnBloques(lineaAEscribir, cantBloques, BLOCK_SIZE);

	escribirLineasEnBloques(listaBloquesAOcupar, listaDatosBloques, BLOCK_SIZE, pathBloques);

	fijarBloquesYPesoA(pokemon, pathFiles, listaBloquesAOcupar, pesoEnBytes);

	// Falta modificar metadata.bin para tener los bloques

}

// Escribir las lineas en listaDatosBloques en los bloques listaBloquesAOcupar
void escribirLineasEnBloques(t_list* listaBloquesAOcupar, t_list* listaDatosBloques, int BLOCK_SIZE, char* pathBloques){

	// 1) Leer lista de bloques
	// (para cada bloque) abrir numero correcto -> pegar datos correspondientes

	if (list_size(listaBloquesAOcupar) != list_size(listaDatosBloques)){
		printf("ERROR | La cantidad de bloques a escribir debe coincidir con la cantidad de datos a escribir");
	}

	int i;

	for (i = 0 ; i < list_size(listaBloquesAOcupar) ; i++){
		char* datoAEscribir = list_get(listaDatosBloques, i);
		int* bloqueAOcupar = list_get(listaBloquesAOcupar, i);
		printf("En el bloque %i se escribira el dato:%s\n", *bloqueAOcupar , datoAEscribir );

		escribirDatoEnBloque(datoAEscribir, *bloqueAOcupar, pathBloques);

		// escribirDatoEnBloque(dato, bloque, pathBloques);
	}
}

// Escribir un dato en un bloque determinado
void escribirDatoEnBloque(char* dato, int numBloque, char* pathBloques){
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

void suscribirseANew(int32_t socket){

	//Uso una estructura para guardar el numero de cola al que me quiero subscribir y mandarlo a la funcion mandar_mensaje
	Suscripcion* estructuraSuscribirse = malloc(sizeof(Suscripcion));

	estructuraSuscribirse->numeroCola = NEW;

	//mandamos el mensaje pidiendo suscribirse a la cola
	mandar_mensaje(estructuraSuscribirse, SUSCRIPCION, socket);

	sleep(1);

	estructuraSuscribirse->numeroCola = GET;
	//mandamos el mensaje pidiendo suscribirse a la cola
	mandar_mensaje(estructuraSuscribirse, SUSCRIPCION, socket);

	sleep(1);


	estructuraSuscribirse->numeroCola = CATCH;
	//mandamos el mensaje pidiendo suscribirse a la cola
	mandar_mensaje(estructuraSuscribirse, SUSCRIPCION, socket);

	free(estructuraSuscribirse);
}

int conectarseABroker(char* IP_BROKER, char* PUERTO_BROKER, t_log* logger){
	int socket = establecer_conexion(IP_BROKER, PUERTO_BROKER);//creo conexión con el Broker.

	resultado_de_conexion(socket, logger, "BROKER");

	return socket;
}

int escucharGameBoy(char* IP_GAMECARD, char* PUERTO_GAMECARD, t_log* logger){
	int socket = establecer_conexion(IP_GAMECARD, PUERTO_GAMECARD);//creo conexión con el Broker.

	resultado_de_conexion(socket, logger, "SOCKET_ESCUCHA");

	return socket;
}

void esperarMensajes(int socket){

	codigo_operacion cod_op;

	int32_t recibidos = recv(socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
	bytesRecibidos(recibidos);

	switch (cod_op)
	{
		case NEW: ;

			New* mensajeNew = malloc(sizeof(New));
			//bytesRecibidos(recv(socket, &cod_op, sizeof(codigo_operacion),MSG_WAITALL));
			recibir_mensaje(mensajeNew, cod_op, socket);

			//mensajeNew->

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

		switch (cod_op) {
		case NEW:
			mensajeNew  = malloc(sizeof(New));
			recibir_mensaje(mensajeNew, cod_op, socket_cliente);

			//ya te llegaron los datos y llamas a tus funciones

			puts("llegue al new");

			break;
		case GET:
			mensajeGet = malloc(sizeof(Get));
			recibir_mensaje(mensajeGet, cod_op, socket_cliente);

			puts("llegue al get");

			//ya te llegaron los datos y llamas a tus funciones

			break;
		case CATCH:
			mensajeCatch = malloc(sizeof(Catch));
			recibir_mensaje(mensajeCatch, cod_op, socket_cliente);

			puts("llegue al catch");

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

	//pthread_create(&threadGamecard,NULL,(void*)serve_client,&socket_cliente);
	//pthread_detach(threadGamecard);
}

void escuchoSocket(int32_t miSocket)
{
	//acepto conexiones entrantes
	struct sockaddr_in direccionConexionEntrante;
	uint32_t tamanioConexionEntrante;
	int32_t conexionEntrante = accept(miSocket, (void*) &direccionConexionEntrante, &tamanioConexionEntrante);
	//printf ("me llego una conexion: %i", conexionEntrante);
	 while(1)
	 {
		 esperar_conexiones(miSocket);
	 }
}

t_list* convertirAListaDeEnterosDesdeChars(char** listaDeChars){
	int cantidadNumeros = 0;
	t_list* miLista = list_create();

	while (listaDeChars[cantidadNumeros] != NULL){
		printf("lista de chars: %s \n", listaDeChars[cantidadNumeros]);
		int charConvertido = atoi(listaDeChars[cantidadNumeros]);
		list_add(miLista, &charConvertido);
		cantidadNumeros++;
	}

	return miLista;
}

char* leerContenidoDeUnBloque(char* pathACarpetaBloques, char* bloqueALeer, int cantBytesALeer){

	// logica para mañana: hacer esto de aca abajo. En teoria, un int es un char, asi que no deberia hacer falta convertir de int a char.
	// https://stackoverflow.com/questions/2279379/how-to-convert-integer-to-char-in-c
	char* pathDeArchivos = malloc(strlen(pathACarpetaBloques) + strlen(bloqueALeer) + 6); // 1 del /, 4 del .bin, 1 del fin de string
	strcpy(pathDeArchivos, pathACarpetaBloques);
	strcat(pathDeArchivos, "/");
	strcat(pathDeArchivos, bloqueALeer);
	strcat(pathDeArchivos, ".bin");

	//printf("path: %s\n", pathDeArchivos);

	char* datosLeidos = malloc(cantBytesALeer + 1);

	FILE* myFile = fopen(pathDeArchivos, "r");

	printf("Bytes a leer: %i\n", cantBytesALeer);

	fread(datosLeidos, cantBytesALeer + 1, 1, myFile);

	fclose(myFile);

	printf("Linea leida: %s\n", datosLeidos);

	return datosLeidos;


	// cantidad a leer esta definido asi ya que la iteracion, y por ende la lógica detras de cual es el último bloque, se debe hacer en el loop.
	// como el último bloque tiene menos cosas, este tiene que ser leido con un tamaño diferente.
	// fclose
	// return ""; //esto, obviamente, es lo que retornaría el fread.
}

int cantidadDeElementosEnArray(char** array){
	int i = 0;
	while(array[i] != NULL){
		i++;
	}
	return i;
}


char* leerContenidoBloquesPokemon(char* pathACarpetaBloques , char** bloquesALeer, int cantidadALeerEnBytes, int BLOCK_SIZE) {

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

		char* aux = leerContenidoDeUnBloque(pathACarpetaBloques, bloquesALeer[i], cantidadDeBytesALeer);

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

int main(void) {


	t_config* config = NULL;
	int TIEM_REIN_CONEXION;
	int TIEM_REIN_OPERACION;
	char* PUNTO_MONTAJE;
	char* IP_BROKER;
	char* PUERTO_BROKER;

	leerConfig(&TIEM_REIN_CONEXION, &TIEM_REIN_OPERACION, &PUNTO_MONTAJE, &IP_BROKER, &PUERTO_BROKER, config);

	// Testing
	//printf("Path punto montaje: %s\n", PUNTO_MONTAJE);

	// Inicializacion del logger... todavia no es necesario

	t_log* logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/GameCard/Logs/GameCard.log", "GAMECARD");

	// puntoMontaje/Blocks
	char* pathBloques = crearCarpetaEn(PUNTO_MONTAJE, "/Blocks");
	// puntoMontaje/Metadata
	char* pathMetadata = crearCarpetaEn(PUNTO_MONTAJE, "/Metadata");
	// puntoMontaje/Files
	char* pathFiles = crearCarpetaEn(PUNTO_MONTAJE, "/Files");

	// Testing
    // printf("%s\n", pathBloques);
    // printf("%s\n", pathMetadata);
    // printf("%s\n", pathFiles);

	int BLOCKS;
	int BLOCK_SIZE;
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
	}

	// -- Desde aca el filesystem ya está inicializado --

	char* pikachu = "Pikachu";
	crearPokemonSiNoExiste(pathFiles, pikachu);

	char* bulbasaur = "Bulbasaur";
	crearPokemonSiNoExiste(pathFiles, bulbasaur);

	// Vaciar un bloque del bitarray (hacerlo = 0)

	// Testing
	printf("Bitarray antes: \n");
	printearBitArray(pathMetadata, BLOCKS);

	if(hayAlgunBloque(pathFiles , pikachu) == 0){
		// Escribe una linea por primera vez en un archivo vacio
		escribirLineaNuevaPokemon(pikachu, 300, 4, 10, BLOCK_SIZE, BLOCKS, pathMetadata, pathBloques, pathFiles);
	} else {

//		escribirLineaNuevaPokemon(pikachu, 300, 4, 10, BLOCK_SIZE, BLOCKS, pathMetadata, pathBloques, pathFiles);
//
//		// printf("Ya hay bloques, se deben leer y apendear a memoria antes de proceder\n");
//		char** bloquesALeer = leerBloques(pathFiles, pikachu);
//
//		char* lineasLeidas = leerContenidoBloquesPokemon(pathBloques, bloquesALeer, 9, BLOCK_SIZE);
//
//		printf("Lineas Leidas:%s\n", lineasLeidas);
	}

	escribirLineaNuevaPokemon(pikachu, 300, 40, 10, BLOCK_SIZE, BLOCKS, pathMetadata, pathBloques, pathFiles);

	// printf("Ya hay bloques, se deben leer y apendear a memoria antes de proceder\n");
	char** bloquesALeer = leerBloques(pathFiles, pikachu);

	char* lineasLeidas = leerContenidoBloquesPokemon(pathBloques, bloquesALeer, 10, BLOCK_SIZE);

	printf("Lineas Leidas:%s\n", lineasLeidas);

	//****************************************************************

	// CONEXION Y ESPERAR MENSAJES DE BROKER
//	int socketBroker = -1;
//
//
//	socketBroker = conectarseABroker(IP_BROKER, PUERTO_BROKER, logger);
//
//	while (socketBroker == -1){
//		sleep(TIEM_REIN_CONEXION);
//		socketBroker = conectarseABroker(IP_BROKER, PUERTO_BROKER, logger);
//	}
//
//	suscribirseANew(socketBroker);
//
//	esperarMensajes(socketBroker);


	//****************************************************************

	/* MANDAR MENSAJE A UNA COLA
	Appeared* estructura = malloc(sizeof(Appeared));

	estructura->posPokemon.x =21;

	mandar_mensaje(estructura,APPEARED,socketBroker);
	*/

	//****************************************************************

	// CONEXION DIRECTA CON GAMEBOY

//	int32_t socketoide = reservarSocket("5001"); //tirarle la key de la config
//
//	// Iniciarlos como hilos
//	escuchoSocket(socketoide); //escuchando al broker
//
//	close(socketoide);


	//****************************************************************



//	int i;
//
//	for (i = 0; i < list_size(listaBloques); i++){
//		char* miString = list_get(listaBloques, i);
//		printf("Elemento %i de la lista: %s", i, miString);
//	}


//	cantidadDeBloquesQueOcupa(10, BLOCK_SIZE); // block_size = 64 //1
//	cantidadDeBloquesQueOcupa(64, BLOCK_SIZE); // 1
//	cantidadDeBloquesQueOcupa(128, BLOCK_SIZE);
//	cantidadDeBloquesQueOcupa(129, BLOCK_SIZE);


//	int* item0 = list_get(unaLista, 0);
//	int* item1 = list_get(unaLista, 1);
//	int* item2 = list_get(unaLista, 2);
//	int* item3 = list_get(unaLista, 3);
//
//	printf("Item 0: %i\n", *item0);
//	printf("Item 1: %i\n", *item1);
//	printf("Item 2: %i\n", *item2);
//	printf("Item 3: %i\n", *item3);

	// Testing de lista
//   int* num1 = list_get(unaLista, 0);

//	int* num2 = list_get(unaLista, 1);
//	int* num3 = list_get(unaLista, 2);
//	int* num4 = list_get(unaLista, 3);

//  printf("%i", *num1);
//	printf("%i", *num2);
//	printf("%i", *num3);
//	printf("%i", *num4);


// PRUEBA ESCRIBIR COSAS EN LISTA
//	t_list* miLista = list_create();
//	int num = 5;
//	int num2 = 7;
//
//	list_add( miLista , &num );
//	list_add(miLista, &num2);
//
//	int* item1 = list_get(miLista, 1);
//
//	printf("Item 0: %i\n", *item1);

	//Cierre del programa
//	free(pathBloques);
//	free(pathMetadata);
//	free(pathFiles);

	// Si haces estos free explota.
	//free(pikachu);
	//free(bulbasaur);

	return EXIT_SUCCESS;
}
