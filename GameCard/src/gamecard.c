#include "gamecard.h"

typedef struct PosicionPokemon{
	int posX;
	int posY;
	int cantidad;
}posPokemon;

//TODO Testing
//void escribirBloquePrueba(){
//	// path = /home/utnso/Escritorio/tall-grass/24.bin
//
//	posPokemon posicionPokemon= {3,2,10};
//
//	FILE* bloque = fopen( "/home/utnso/Escritorio/tall-grass/24.bin" , "w" );
//
//	// Solucion con fwrite (no anda)
//	//fwrite(&posicionPokemon, sizeof(posPokemon),1, bloque);
//
//	// Solucion con fprintf()
//	//fprintf(bloque, "Estoy probando meterle un texto al archivo");
//
//	fclose(bloque);
//
//}

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

		FILE* bloque = fopen( pathBloqueActual , "w+" );

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

void leerConfig(int* TIEM_REIN_CONEXION, int* TIEM_REIN_OPERACION, char** PUNTO_MONTAJE, char** IP_BROKER, char** PUERTO_BROKER){

	t_config* config;

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
}

t_bitarray* crearBitArray(char* bitarray, int cantBloques){
	// Se crea en cantidad de bytes no bits. (por eso hago cantBloques / 8)
	return bitarray_create_with_mode(bitarray, cantBloques / 8, MSB_FIRST);
	// 'bitarray' es un malloc de cantBloques / 8
}

void leerMetadataBin(char* pathMetadata, int* BLOCKS, int* BLOCK_SIZE, char** MAGIC_NUMBER){

	// Nombre del archivo metadata
	char* archivoMetadata = "/Metadata.bin";

	// Path de la carpeta {punto_montaje}/Metadata/Metadata.bin
	char* pathMetadataBin = malloc(strlen(pathMetadata) + strlen(archivoMetadata) + 1);

	// Le pego el path de metadata
	strcpy(pathMetadataBin, pathMetadata);
	// Le pego al final el path del archivo
	strcat(pathMetadataBin, archivoMetadata);

	// Leo el archivo
	t_config* metadataBin;
	metadataBin = leerConfiguracion(pathMetadataBin);

	if (metadataBin == NULL){
		printf("No se pudo leer el archivo Metadata/Metadata.bin");
		exit(2);
	}

	// Obtengo los valores (pasados por referencia)
	*BLOCKS = config_get_int_value(metadataBin, "BLOCKS");
	*BLOCK_SIZE = config_get_int_value(metadataBin, "BLOCK_SIZE");
	*MAGIC_NUMBER = config_get_string_value(metadataBin,"MAGIC_NUMBER" );

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

	free(pathCarpetaPokemon);
	return retorno;
}

// Crear la carpeta de un nuevo pokemon dentro de Files/
void crearCarpetaPokemon(char* pathFiles, char* pokemon){

	char* pathCarpetaPokemon = malloc(strlen(pathFiles) + strlen(pokemon) + 2);

	strcpy(pathCarpetaPokemon, pathFiles);
	strcat(pathCarpetaPokemon, "/");
	strcat(pathCarpetaPokemon, pokemon);

	//printf("Path carpeta pokemon: %s\n", pathCarpetaPokemon);

	mkdir(pathCarpetaPokemon, 0777);

	free(pathCarpetaPokemon);
}

void guardarBitArrayEnArchivo(char* pathMetadata, char* bitArray){
	char* pathBitmap = "/Bitmap.bin";

	char* pathCompleto = malloc(strlen(pathMetadata) + strlen(pathBitmap) + 1);

	// Copio path de metadata
	strcpy(pathCompleto, pathMetadata);

	// Pego el path de bitmap
	strcat(pathCompleto, pathBitmap);

	FILE* bitmapArchivo = fopen( pathCompleto , "a" );

	fseek( bitmapArchivo, 0, SEEK_SET );

	fwrite(bitArray, 1, 1, bitmapArchivo);

	fclose(bitmapArchivo);

	free(pathCompleto);
}

//TODO Terminar
void leerBitArrayDeArchivo( char* pathMetadata , ){
	char* pathBitmap = "/Bitmap.bin";

	char* pathCompleto = malloc(strlen(pathMetadata) + strlen(pathBitmap) + 1);

}

int main(void) {

	int TIEM_REIN_CONEXION;
	int TIEM_REIN_OPERACION;
	char* PUNTO_MONTAJE;
	char* IP_BROKER;
	char* PUERTO_BROKER;

	leerConfig(&TIEM_REIN_CONEXION, &TIEM_REIN_OPERACION, &PUNTO_MONTAJE, &IP_BROKER, &PUERTO_BROKER);

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

	printf("Path files: %s\n", pathFiles);

	// Datos de metadata/metadata.bin
	int BLOCKS;
	int BLOCK_SIZE;
	char* MAGIC_NUMBER;
	// Funcion para leer metadata.bin
	leerMetadataBin(pathMetadata, &BLOCKS, &BLOCK_SIZE, &MAGIC_NUMBER);

	// Creaar los bloques dentro de carpeta /Blocks
	crearBloquesEn(pathBloques, BLOCKS);

	if (!existeCarpetaPokemon(pathFiles, "Pikachu")){
		crearCarpetaPokemon(pathFiles, "Pikachu");
		printf("No existia la carpeta Pikachu\n");
	} else {
		printf("Existe la carpeta Pikachu\n");
	}

	if (!existeCarpetaPokemon(pathFiles, "Bulbasaur")){
		crearCarpetaPokemon(pathFiles, "Bulbasaur");
		printf("No existia la carpeta Bulbasaur\n");
	} else {
		printf("Existe la carpeta Bulbasaur\n");
	}

	char* BITARRAY = malloc(BLOCKS / 8);

	t_bitarray* bitArrayBloques = crearBitArray(BITARRAY, BLOCKS);

	bitarray_set_bit(bitArrayBloques, 1);

	guardarBitArrayEnArchivo(pathMetadata, BITARRAY);

	//t_bitarray* =

    //posPokemon unasPosPokemon = {3,2,10};
//
//	char* bloque43 = "/43.bin";
//	char* pathBloque43 = malloc(strlen(pathBloques) + strlen(bloque43) + 1);
//
//	strcpy(pathBloque43, pathBloques);
//	strcat(pathBloque43, bloque43);

	//escribirLineaEnBloque(unasPosPokemon);

	// Probando escribir .bin
	//escribirBloquePrueba();

	// No funciona todavia
	//leerUnPokemon(pathFiles, "Pikachu");


	// Tocando cosas del BitArray
	// Devuelve 200 = cantidadTotalBits
	//int cantBits = bitarray_get_max_bit(bitArrayBloques);

	/* Testing con bitarray
	// Fija el bit a 1
	bitarray_set_bit(bitArrayBloques, 4);

	// Veo cuanto vale
	int bit3 = bitarray_test_bit(bitArrayBloques, 3); // 0
	int bit4 = bitarray_test_bit(bitArrayBloques, 4); // 1

	printf("Antes del clean: \n");
	printf("Bit 3: %i\n", bit3);
	printf("Bit 4: %i\n", bit4);

	// Vuelve el bit 0
	bitarray_clean_bit(bitArrayBloques, 4);

	bit3 = bitarray_test_bit(bitArrayBloques, 3); // 0
	bit4 = bitarray_test_bit(bitArrayBloques, 4); // 1

	printf("Despues del clean: \n");
	printf("Bit 3: %i\n", bit3);
	printf("Bit 4: %i\n", bit4);
	*/

	return EXIT_SUCCESS;
}
