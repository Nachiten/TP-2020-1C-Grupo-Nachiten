#include "gamecard.h"

char* crearCarpetaEn(char* pathPuntoMontaje, char* nombreCarpeta){

	// Path de la carpeta {punto_montaje}/nombreCarpeta
	char* pathCarpeta = malloc(strlen(pathPuntoMontaje) + strlen(nombreCarpeta) + 1);
	// Hacer este malloc explota por sf | = malloc(strlen(pathPuntoMontaje + 1));

	// Copiando el punto de montaje al path de la carpeta
	strcpy(pathCarpeta, pathPuntoMontaje);

	// Agrego el path de la carpeta /nombreCarpeta
	strcat(pathCarpeta, nombreCarpeta);

	mkdir(pathCarpeta, 0777);

	return pathCarpeta;
}

void crearBloquesEn(char* pathBloques, int cantidadBloques){
	
	int bloqueActual;
	
	for(bloqueActual = 1 ; bloqueActual <= cantidadBloques ; bloqueActual++ ){

		// Array de chars para meter el int convertido a array
		char* enteroEnLetras = malloc(5 * sizeof(char));

		// Genero un array de chars que es el numero de bloque pasado a string
		sprintf(enteroEnLetras, "%i", bloqueActual);

		char* extension = ".bin";

		// 5 = longitud del numero + 1 por la /
		char* nombreArchivo = malloc( strlen(enteroEnLetras) + strlen(extension) + 2 );

		// El nombre de archivo es de la forma /{num}.bin | Ejemplo: /35.bin
		strcpy(nombreArchivo, "/");
		strcat(nombreArchivo, enteroEnLetras);
		strcat(nombreArchivo, extension);

		// Creo una copia del path de /Blocks para no modificarlo
		char* pathBloqueActual = malloc(strlen(pathBloques) + strlen(nombreArchivo) + 1);

		// Le pego el path de /Blocks
		strcpy(pathBloqueActual, pathBloques);

		// Le pego el valor hardcodeado 1.bin
		strcat(pathBloqueActual, nombreArchivo);

		FILE* bloque = fopen( pathBloqueActual , "w" );

		if (bloque == NULL){
			printf("No se pudo crear el bloque %i", bloqueActual);
			exit(3);
		}

		fclose(bloque);

		free(nombreArchivo);
		free(pathBloqueActual);
		free(enteroEnLetras);
	}
	
}

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

void escribirLineaEnBloque(posPokemon posPokemon){
// Tira error de  free(): invalid next size (normal): 0x08074b00 ***

	//printf("%s", pathBloque);

	FILE *archivo = fopen("/home/utnso/Escritorio/tall-grass/Blocks/43.bin", "a");
	if (archivo == NULL)
	{
		printf("Hubo un error abriendo el archivo!\n");
		exit(4);
	}

	fprintf(archivo, "%i-%i=%i\n", posPokemon.posX, posPokemon.posY, posPokemon.cantidad);

	fclose(archivo);

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
		crearCarpetaPokemon(pathFiles, pokemon);
		crearMetadataPokemon(pathFiles, pokemon);
		printf("No existia la carpeta: %s [Se creó]\n", pokemon);
	} else {
		printf("Existe la carpeta: %s\n", pokemon);
	}
}

t_bitarray* crearBitArray(char* bitarray, int cantBloques){
	// Se crea en cantidad de bytes no bits. (por eso hago cantBloques / 8)
	return bitarray_create_with_mode(bitarray, cantBloques / 8, MSB_FIRST);
	// 'bitarray' es un malloc de cantBloques / 8
}

void guardarBitArrayEnArchivo(char* pathMetadata, char* bitArray, int BLOCKS){
	char* pathBitmap = "/Bitmap.bin";

	char* pathCompleto = malloc(strlen(pathMetadata) + strlen(pathBitmap) + 1);

	// Copio path de metadata
	strcpy(pathCompleto, pathMetadata);

	// Pego el path de bitmap
	strcat(pathCompleto, pathBitmap);

	FILE* bitmapArchivo = fopen( pathCompleto , "w" );

	// Me muevo al principio del archivo
	fseek( bitmapArchivo, 0, SEEK_SET );

	fwrite(bitArray, BLOCKS / 8, 1, bitmapArchivo);

	fclose(bitmapArchivo);

	free(pathCompleto);
}

void leerBitArrayDeArchivo(char* pathMetadata, char** bitArray, int BLOCKS){
	char* pathBitmap = "/Bitmap.bin";

	char* pathCompleto = malloc(strlen(pathMetadata) + strlen(pathBitmap) + 1);

	// Copio path de metadata
	strcpy(pathCompleto, pathMetadata);

	// Pego el path de bitmap
	strcat(pathCompleto, pathBitmap);

	// VA UN SEMAFORO ACA !!!

	FILE* bitmapArchivo = fopen( pathCompleto , "r" );

	// Me muevo al principio del archivo
	fseek( bitmapArchivo, 0, SEEK_SET );

	fread(*bitArray, BLOCKS / 8, 1, bitmapArchivo);

	fclose(bitmapArchivo);

	free(pathCompleto);

}

void vaciarBitArray(t_bitarray* bitArray, int BLOCKS){

	int i;

	for (i = 0; i < BLOCKS; i++){
		bitarray_clean_bit(bitArray, i);
	}
}

void liberarUnBloque(char* pathMetadata, int index, int BLOCKS){

	char* BITARRAY = malloc(BLOCKS / 8);

	leerBitArrayDeArchivo(pathMetadata, &BITARRAY, BLOCKS);

	t_bitarray* bitArrayBloques = crearBitArray(BITARRAY, BLOCKS);

	bitarray_clean_bit(bitArrayBloques, index);

	guardarBitArrayEnArchivo(pathMetadata, BITARRAY, BLOCKS);

}

void escribirPokemon(char* path,int posX, int posY, int cantidad ){
	FILE* archivo = fopen("34.bin", "a");
	if (archivo == NULL)
	{
		printf("Error abriendo el archivo\n");
		exit(1);
	}

	char* stringAEscribir;

	int cantBytes = asprintf(&stringAEscribir, "%i-%i=%i\n", posX, posY, cantidad);

	printf("El numero de bytes es: %i\n", cantBytes);
	printf("El string es: %s", stringAEscribir);

	fwrite(stringAEscribir, cantBytes, 1, archivo);

	fclose(archivo);

}

char* separarCoord(char* unString){
	char* token = malloc(strlen(unString));

	char* stringModificado = strdup(unString);

	token = strsep(&stringModificado, "=");

	return token;
}

// Retorna la posicion de la linea de las coords. -1 Si no se encuentra
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

int existeFilesystem(char* pathMetadata){
	int retorno = 0;

	char* nombreBitmap = "/Bitmap.bin";

	char* pathBitmap = malloc(strlen(pathMetadata) + strlen(nombreBitmap) + 1);

	strcpy(pathBitmap, pathMetadata);
	strcat(pathBitmap, nombreBitmap);

	FILE* bitmap = fopen(pathBitmap, "r");

	// Si el archivo se encuentra
	if (bitmap != NULL){
		retorno = 1;
		fclose(bitmap);
	}

	free(pathBitmap);

	return retorno;

}

void inicializarFileSystem(char* pathBloques, char* pathFiles, char* pathMetadata, int BLOCKS){

	char* BITARRAY = malloc(BLOCKS / 8);

	// Crear metadatas con DIRECTORY=Y
	crearMetadataCarpeta(pathBloques);
	crearMetadataCarpeta(pathFiles);

	// Crear los bloques dentro de carpeta /Blocks
	crearBloquesEn(pathBloques, BLOCKS);

	// Genero el bitarray
	t_bitarray* bitArrayBloques = crearBitArray(BITARRAY, BLOCKS);

	// Piso todos los bits a 0
	vaciarBitArray(bitArrayBloques, BLOCKS);

	// Lo guardo en archivo
	guardarBitArrayEnArchivo(pathMetadata, BITARRAY, BLOCKS);
}

// Obtiene "cantidad" de bloques libres (bits = 0) del bitmap
t_list* obtenerPrimerosLibresDeBitmap(char* pathMetadata, int BLOCKS, int cantidad){
	// Al meter un puntero en una lista si ese puntero cambia entonces cambia la lista. Porque guarda una referencia no una copia

	t_list * listaNums = list_create();

	char* BITARRAY_ARCHIVO = malloc(BLOCKS / 8);

	leerBitArrayDeArchivo(pathMetadata, &BITARRAY_ARCHIVO, BLOCKS);

	t_bitarray* bitArray = crearBitArray(BITARRAY_ARCHIVO, BLOCKS);

	int i;
	int numeroBloque = 1;
	for(i = 0; i< BLOCKS; i++){

		//printf("Estoy escaneando el bit numero: %i", i);

		// Si el bit es uno y todavia necesiton sumar => sumo
		if (bitarray_test_bit(bitArray, i) == 0 && cantidad > 0){
			bitarray_set_bit(bitArray, i);

			int* numeroBloqueCopiado = malloc(sizeof(int));

			*numeroBloqueCopiado = numeroBloque;

			printf("Encontre el bit numero: %i\n", numeroBloque);
			list_add(listaNums, numeroBloqueCopiado);

			cantidad--;

		} else if (cantidad == 0){
			printf("Ya encontre todos los bloques que queria :)");
			break;
		}
		else {
			printf("El bloque numero: %i Ya está siendo usado..\n", numeroBloque);
		}

		numeroBloque++;

	}

	guardarBitArrayEnArchivo(pathMetadata, BITARRAY_ARCHIVO, BLOCKS);

//	t_list* miLista = list_create();
//
//	int num = 5;
//	int num2 = 7;
//
//	list_add(miLista, &num);
//	list_add(miLista, &num2);

	// WHAT. Todos quedan como 129 por alguna razon mistica


	return listaNums;
}

// Printea el bitarray completo, funciona
void printearBitArray(char* pathMetadata, int BLOCKS){

	char* BITARRAY_ARCHIVO = malloc(BLOCKS / 8);

	leerBitArrayDeArchivo(pathMetadata, &BITARRAY_ARCHIVO, BLOCKS);

	t_bitarray* bitArray = crearBitArray(BITARRAY_ARCHIVO, BLOCKS);

	int i;
	for ( i = 0 ; i< BLOCKS; i++){
		if(bitarray_test_bit(bitArray, i) == 1){
			printf("%i", 1);
		} else {
			printf("%i", 0);
		}
	}

	printf("\n");

	//bitarray_destroy(bitArray);
	//free(BITARRAY_ARCHIVO);

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

	/* Inicializacion del logger... todavia no es necesario

	//t_log* logger;
	//logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/GameCard/Logs/GameCard.log", "GAMECARD");

	*/

	// Crear la carpeta /Blocks
	char* pathBloques = crearCarpetaEn(PUNTO_MONTAJE, "/Blocks");
	// Crear la carpeta /Metadata [Si ya existe no hace nada]
	char* pathMetadata = crearCarpetaEn(PUNTO_MONTAJE, "/Metadata");
	// Crear la carpeta /Files
	char* pathFiles = crearCarpetaEn(PUNTO_MONTAJE, "/Files");

//	printf("%s\n", pathBloques);
//	printf("%s\n", pathMetadata);
//	printf("%s\n", pathFiles);

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
		printf("El filesystem ya existe. No creo nada.\n");
	}

	// -- Desde aca el filesystem ya está inicializado --


	char* pikachu = "Pikachu";
	crearPokemonSiNoExiste(pathFiles, pikachu);

	char* bulbasaur = "Bulbasaur";
	crearPokemonSiNoExiste(pathFiles, bulbasaur);

	liberarUnBloque(pathMetadata, 1, BLOCKS); // bloque 2
	liberarUnBloque(pathMetadata, 2, BLOCKS); // bloque 3

	// Ya funciona :D
	t_list* unaLista = obtenerPrimerosLibresDeBitmap(pathMetadata, BLOCKS, 4);

	// Vaciar un bloque del bitarray (hacerlo = 0)

	printearBitArray(pathMetadata, BLOCKS);

	int* item0 = list_get(unaLista, 0);
	int* item1 = list_get(unaLista, 1);
	int* item2 = list_get(unaLista, 2);
	int* item3 = list_get(unaLista, 3);

	printf("Item 0: %i\n", *item0);
	printf("Item 1: %i\n", *item1);
	printf("Item 2: %i\n", *item2);
	printf("Item 3: %i\n", *item3);

	// Testing de lista
//   int* num1 = list_get(unaLista, 0);

//	int* num2 = list_get(unaLista, 1);
//	int* num3 = list_get(unaLista, 2);
//	int* num4 = list_get(unaLista, 3);

//  printf("%i", *num1);
//	printf("%i", *num2);
//	printf("%i", *num3);
//	printf("%i", *num4);


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
