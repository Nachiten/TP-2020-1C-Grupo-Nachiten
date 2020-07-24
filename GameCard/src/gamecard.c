#include "gamecard.h"

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

t_config* leerConfig(int* TIEM_REIN_CONEXION, int* TIEM_REIN_OPERACION, char** PUNTO_MONTAJE, char** IP_BROKER, char** PUERTO_BROKER){

	t_config* config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/GameCard.config");

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

	return config;
}

t_config* leerMetadataBin(char* pathMetadata, int* BLOCKS, int* BLOCK_SIZE, char** MAGIC_NUMBER){

	// Nombre del archivo metadata
	char* archivoMetadata = "/Metadata.bin";

	// Path de la carpeta {punto_montaje}/Metadata/Metadata.bin
	char* pathMetadataBin = malloc(strlen(pathMetadata) + strlen(archivoMetadata) + 1);

	// Le pego el path de metadata
	strcpy(pathMetadataBin, pathMetadata);
	// Le pego al final el path del archivo
	strcat(pathMetadataBin, archivoMetadata);

	// Leo el archivo
	t_config* metadataBin = leerConfiguracion(pathMetadataBin);

	if (metadataBin == NULL){
		printf("No se pudo leer el archivo Metadata/Metadata.bin");
		exit(2);
	}

	// Obtengo los valores (pasados por referencia)
	*BLOCKS = config_get_int_value(metadataBin, "BLOCKS");
	*BLOCK_SIZE = config_get_int_value(metadataBin, "BLOCK_SIZE");
	*MAGIC_NUMBER = config_get_string_value(metadataBin,"MAGIC_NUMBER" );

	free(pathMetadataBin);

	return metadataBin;

	/*
	printf("%i\n", *BLOCKS);
	printf("%i\n", *BLOCK_SIZE);
	printf("%s\n", *MAGIC_NUMBER);
	*/
}

// Checkear si existe un determinado pokemon dentro de la carpeta Files/
// Retorna 1 si la carpeta existe, 0 si no existe
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
// Primera linea = 0
int encontrarCoords(int posX, int posY, char* lineaABuscar){

	//char* lineaDePrueba = "123-23=10\n10-20=3\n15-20=20";

	int numLinea = 0;

	int retorno = -1;

	char** arrayLineas = string_split(lineaABuscar, "\n");

	char* lineaActual;
	while( (lineaActual = arrayLineas[numLinea]) != NULL){

		//printf("La linea es: %s\n", lineaActual);

		char* lineaConvertidaABuscar;
		char* lineaActualConvertida = separarCoord(lineaActual);

		asprintf(&lineaConvertidaABuscar, "%i-%i", posX, posY);

		if (strcmp(lineaConvertidaABuscar, lineaActualConvertida) == 0){
			retorno = numLinea;
		}

		numLinea++;
	}

	free(arrayLineas);

	return retorno;
}

// Lee los bloques del metadata.bin de un pokemon existente
char** leerBloquesPokemon(char* pokemon){
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

	char** arrayBloques = config_get_array_value(datosMetadata, "BLOCKS");

	config_destroy(datosMetadata);
	free(pathMetadataPokemon);

	return arrayBloques;
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

	int valorSize = config_get_int_value(datosMetadata, "SIZE");

	config_destroy(datosMetadata);
	free(pathMetadataPokemon);

	return valorSize;
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
	free(pathMetadataPokemon);
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
	free(pathMetadataPokemon);
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

	if (tamanioListaBloques != 0){
		// La cantidad de comas
		tamanioTotalBytes += (tamanioListaBloques - 1 );
	}

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

	char** bloques = leerBloquesPokemon(pokemon);

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

		// TODO | OJO AL PIOJO
		//free(miString);
		// Rompe todo hacer esto

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

	list_destroy(listaBloquesAOcupar);
	list_destroy(listaDatosBloques);
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
	free(nombreArchivo);
	free(pathBloque);
}



t_list* convertirAListaDeEnterosDesdeChars(char** listaDeChars){
	int cantidadNumeros = 0;
	t_list* listaEnteros = list_create();

	while (listaDeChars[cantidadNumeros] != NULL){
		//printf("lista de chars: %s \n", listaDeChars[cantidadNumeros]);

		// Al hacer malloc me aseguro que esta direccion de memoria no va a ser pisada
		int* charConvertido = malloc(sizeof(int));

		int nombre = atoi(listaDeChars[cantidadNumeros]);

		memcpy(charConvertido, &nombre, sizeof(int));

		list_add(listaEnteros, charConvertido);
		cantidadNumeros++;
	}

	return listaEnteros;
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
void sumarBloquesExtraALista(t_list* listaBloques, int cantidadBloquesExtra){

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

char* sumarALineaPokemon(char* texto, int posX, int posY, int cantidad) {
	char* stringAEncontrar;
	char* stringAEscribir;
	asprintf(&stringAEncontrar, "%i-%i=", posX, posY);

    // Un puntero al lugar donde comienza el string buscado
	char* aux = strstr(texto, stringAEncontrar);

	int posicionDeInicioDeLineaAModificar = aux - texto;

	// Segunda mitad del string
	char* segundaMitadDelString = strstr(aux, "\n");

	// Mover el puntero aux para lle
	char* aux2 = aux + strlen(stringAEncontrar);

	//printf("Aux 2: %s", aux2);

	// Separar el string restante para obtener la cantidad
	char** stringsSeparados = string_split(aux2, "\n");

	// Cantidad de la linea actual
	char* cantidadActual = stringsSeparados[0];

	// Convierto la cantidad actual a entero
	int* charConvertido = malloc(sizeof(int));
	int cantidadActualEntero = atoi(cantidadActual);
	memcpy(charConvertido, &cantidadActualEntero, sizeof(int));

	// Sumo a la cantidad que tiene que quedar
	cantidad+= cantidadActualEntero;

	asprintf(&stringAEscribir, "%i-%i=%i", posX, posY, cantidad);

	// Primera mitad del string
	char* primeraMitadDelString = malloc(posicionDeInicioDeLineaAModificar + 1);
	memcpy(primeraMitadDelString, texto, posicionDeInicioDeLineaAModificar);

	// Pego un \0 al final del string
	primeraMitadDelString[posicionDeInicioDeLineaAModificar] = '\0';

	// Pegar lo obtenido junto
	char* stringARetornar = malloc(strlen(primeraMitadDelString) + strlen(stringAEscribir) + strlen(segundaMitadDelString) + 1);
	strcpy(stringARetornar, primeraMitadDelString);
	strcat(stringARetornar, stringAEscribir);
	strcat(stringARetornar, segundaMitadDelString);

	free(stringAEscribir);
	free(stringAEncontrar);
	free(primeraMitadDelString);
	free(stringsSeparados);
	free(charConvertido);

	return stringARetornar;
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

			printf("El archivo %s fue abierto correctamente\n", pokemon);
			signalSemaforoPokemon(pokemon);

			config_destroy(datosMetadata);
			break;
		}

		signalSemaforoPokemon(pokemon);
		config_destroy(datosMetadata);

		printf("No pude abrir el archivo %s, entrando en tiempo de espera\n", pokemon);

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
	free(pathMetadataPokemon);

}

void liberarNBloques(t_list* listaBloques, int cantidad){

	// Devuelvo una nueva lista con los bloques que debo liberar
	t_list* bloquesALiberar = list_take_and_remove(listaBloques, cantidad);

	int i;
	for (i= 0; i<list_size(bloquesALiberar) ; i++){
		int* bloqueALiberar = list_get(bloquesALiberar, i);

		liberarUnBloque(*bloqueALiberar - 1);
	}
}

t_list* convertirAListaDeStringsDesdeChars(char** lineas){
	t_list* listaStrings = list_create();

	int cantStrings;

	// Valgrind dice que explota aca no capto bien :P
	while (lineas[cantStrings] != NULL){

		char* lineaActual = malloc(strlen(lineas[cantStrings]));

		memcpy(lineaActual, lineas[cantStrings], strlen(lineas[cantStrings]));

		list_add(listaStrings, lineaActual);

		cantStrings++;
	}

	return listaStrings;
}

char* restarALineaPokemon(char* texto, int posX, int posY) {
	char* stringAEncontrar;
	char* stringAEscribir;
	asprintf(&stringAEncontrar, "%i-%i=", posX, posY);

    // Un puntero al lugar donde comienza el string buscado
	char* aux = strstr(texto, stringAEncontrar);

	int posicionDeInicioDeLineaAModificar = aux - texto;

	// Segunda mitad del string
	char* segundaMitadDelString = strstr(aux, "\n");

	// Mover el puntero aux para lle
	char* aux2 = aux + strlen(stringAEncontrar);

	//printf("Aux 2: %s", aux2);

	// Separar el string restante para obtener la cantidad
	char** stringsSeparados = string_split(aux2, "\n");

	// Cantidad de la linea actual
	char* cantidadActual = stringsSeparados[0];

	// Convierto la cantidad actual a entero
	int* charConvertido = malloc(sizeof(int));
	int cantidadActualEntero = atoi(cantidadActual);
	memcpy(charConvertido, &cantidadActualEntero, sizeof(int));

	// Le resto 1
	cantidadActualEntero --;

	asprintf(&stringAEscribir, "%i-%i=%i", posX, posY, cantidadActualEntero);

	// Primera mitad del string
	char* primeraMitadDelString = malloc(posicionDeInicioDeLineaAModificar + 1);
	memcpy(primeraMitadDelString, texto, posicionDeInicioDeLineaAModificar);

	// Pego un \0 al final del string
	primeraMitadDelString[posicionDeInicioDeLineaAModificar] = '\0';

	char* stringARetornar;

	// Se debe eliminar la linea (no pegar el string a escribir)
	if (cantidadActualEntero == 0){

		// Pegar lo obtenido junto
		stringARetornar = malloc(strlen(primeraMitadDelString) + strlen(segundaMitadDelString) + 1);
		strcpy(stringARetornar, primeraMitadDelString);
		strcat(stringARetornar, segundaMitadDelString + 1);

	} else {

		// Pegar lo obtenido junto
		stringARetornar = malloc(strlen(primeraMitadDelString) + strlen(stringAEscribir) + strlen(segundaMitadDelString) + 1);
		strcpy(stringARetornar, primeraMitadDelString);
		strcat(stringARetornar, stringAEscribir);
		strcat(stringARetornar, segundaMitadDelString);

	}

	free(stringAEscribir);
	free(stringAEncontrar);
	free(primeraMitadDelString);

	return stringARetornar;
}

t_list* convertirAListaDeCoords(char* lineas){
	t_list* lista = list_create();

	// Devuelve un array donde cada string es: 3-20=50
	char** lineasSeparadas = string_split(lineas, "\n");

	/* Recorro las lineas separadas y para cada una:
	 * 1) Obtengo coordX y CoordY
	 * 2) Los paso a entero con ATOI
	 * 3) Meto las coordenadas en la lista
	 * 4) Devuelvo la lista
	 */

	// Variable iterativa para el while
	int cantLineas = 0;

	char* lineaActual;

	while ( (lineaActual = lineasSeparadas[cantLineas]) != NULL){

		// Devuelve dos elementos: 33(posX) y 3=20
		char** lineaSeparada = string_split(lineaActual, "-");

		// Obtengo la posicionX como char
		char* posX = lineaSeparada[0];

		// Devuelve dos elementos 3(posY) y 20
		char** coordMasCantidad = string_split(lineaSeparada[1], "=");

		// Obtengo la posicion Y como char
		char* posY = coordMasCantidad[0];

		// Convierto el char de posX a un entero
		int* posXConvertido = malloc(sizeof(int));
		int posXEntero = atoi(posX);
		memcpy(posXConvertido, &posXEntero, sizeof(int));

		// Convierto el char de posY a un entero
		int* posYConvertido = malloc(sizeof(int));
		int posYEntero = atoi(posY);
		memcpy(posYConvertido, &posYEntero, sizeof(int));

		list_add(lista, posXConvertido);
		list_add(lista, posYConvertido);

		cantLineas++;
	}

	if ( list_size(lista) % 2 != 0){
		printf("ERROR | Se cargo una cantidad impar de coordenads en la lista, esto no es posible");
	}

	return lista;
}

int main(void) {
	//t_config* config = NULL;
	int TIEM_REIN_CONEXION;
	char* PUNTO_MONTAJE;

	socketNew = -1;
	socketCatch = -1;
	socketGet = -1;

	semBitmap = malloc(sizeof(sem_t));

	// Inicializar semaforo bitmap
	sem_init(semBitmap, 0, 1);

	//Inicializar lista de semaforos pokemon
	listaSemPokemon = list_create();

	t_config* config = leerConfig(&TIEM_REIN_CONEXION, &TIEM_REIN_OPERACION, &PUNTO_MONTAJE, &IP_BROKER, &PUERTO_BROKER);

	// Testing
	//printf("Path punto montaje: %s\n", PUNTO_MONTAJE);

	// Inicializacion del logger... todavia no es necesario

	logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/GameCard/Logs/GameCard.log", "GAMECARD");

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

	// Funcion para leer metadata.bin
	t_config* metadataBin = leerMetadataBin(pathMetadata, &BLOCKS, &BLOCK_SIZE, &MAGIC_NUMBER);

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

	// Testing
	//printf("Bitarray antes: \n");
	//printearBitArray(pathMetadata, BLOCKS);

	char* pikachu = "Pikachu";
	char* jorge = "Jorge";
	char* fruta = "Fruta";
	char* bulbasaur = "Bulbasaur";

//	mensajeNew(pikachu, 1, 15, 1, 1);
//
//	mensajeCatch(pikachu, 1, 15, 1);
//
//	mensajeGet(pikachu, 1);
//	mensajeGet(pikachu, 1);
//	mensajeGet(pikachu, 1);
//	mensajeGet(pikachu, 1);

	mensajeNew(fruta, 10, 5, 20, 1);

	mensajeNew(bulbasaur, 1, 15, 1, 1);

	mensajeCatch(bulbasaur, 1, 15, 1);

	mensajeNew(pikachu, 1, 15, 1, 1);

	mensajeNew(fruta, 10, 5, 20, 1);

	mensajeCatch(pikachu, 1, 15, 1);

	printearBitArray();

	mensajeNew(jorge, 1, 20, 3, 1);

	mensajeGet(pikachu, 1);

	mensajeCatch(jorge, 1, 24, 1);

	mensajeNew(fruta, 23, 10, 1, 1);

	mensajeNew(jorge, 1, 30, 5, 1);

	mensajeCatch(jorge, 1, 20, 1);

	mensajeNew(fruta, 25, 15, 25, 1);

	mensajeCatch(fruta, 23, 10, 1);

	mensajeCatch(pikachu, 1, 15, 1);

	mensajeNew(jorge, 1, 40, 7, 1);

	mensajeNew(fruta, 35, 20, 40, 1);

	mensajeCatch(jorge, 1, 20, 1);

	mensajeNew(pikachu, 1, 20, 3, 1);

	mensajeCatch(jorge, 1, 20, 1);

	mensajeNew(fruta, 10, 50, 25, 1);
	mensajeNew(fruta, 10, 51, 25, 1);
	mensajeNew(fruta, 10, 52, 25, 1);
	mensajeNew(fruta, 10, 53, 25, 1);
	mensajeNew(fruta, 10, 54, 25, 1);
	mensajeNew(fruta, 10, 55, 25, 1);


	/*Expected:
	 * Bulbasaur: -> Vacio
	 * Pikachu -> 1-20=3
	 * Jorge -> 1-30=5
	 * 			1-40=7
	 * Fruta -> 10-5=40
	 * 			25-15=25
	 * 			35-20=40
	 */


	// Hilos de prueba
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
//  pthread_join(hiloBroker, NULL);
//	pthread_join(hiloGameBoy, NULL);

	// ****************************************************************

	/* MANDAR MENSAJE A UNA COLA
	Appeared* estructura = malloc(sizeof(Appeared));

	estructura->posPokemon.x =21;

	mandar_mensaje(estructura,APPEARED,socketBroker);
	*/

	//****************************************************************

	// Libero la config
	if (config != NULL) config_destroy(config);

	// Libero el config de metadatabin
	if (metadataBin != NULL) config_destroy(metadataBin);

	// Libero el logger
	if (logger != NULL) log_destroy(logger);

	return EXIT_SUCCESS;
}
