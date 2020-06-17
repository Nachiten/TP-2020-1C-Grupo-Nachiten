#include "gamecard.h"

typedef struct PosicionPokemon{
	int posX;
	int posY;
	int cantidad;
}posPokemon;


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
		// IMPORTANTE: Como convertir entero a string

		char* extension = ".bin";

		// 5 = longitud del numero + 1 por la /
		char* nombreArchivo = malloc( 6 + strlen(extension) + 1 );

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
		fclose(bloque);

		free(nombreArchivo);
		free(pathBloqueActual);
	}
	
}

void leerConfig(int* TIEM_REIN_CONEXION, int* TIEM_REIN_OPERACION, char** PUNTO_MONTAJE, char** IP_BROKER, char** PUERTO_BROKER){

	t_config* config;

	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/GameCard.config");

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

t_bitarray* crearBitArray(char* pathMetadata, int cantBloques){
	char* pathBitmap = "/Bitmap.bin";

	char* pathCompleto = malloc(strlen(pathMetadata) + strlen(pathBitmap) + 1);

	// Copio path de metadata
	strcpy(pathCompleto, pathMetadata);

	// Pego el path de bitmap
	strcat(pathCompleto, pathBitmap);

	// Se crea en cantidad de bytes no bits. (por eso hago cantBloques / 8)
	return bitarray_create_with_mode(pathCompleto, cantBloques / 8, MSB_FIRST);
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

void leerUnPokemon(char* pathFiles, char* pokemon){
	// + 2 por el \0 y la /
	char* metadataBin = "/Metadata.bin";
	char* pathPokemonMetadata = malloc(sizeof(pathFiles) + sizeof(pokemon) + sizeof(metadataBin) + 2);

	strcpy(pathPokemonMetadata, pathFiles);
	strcat(pathPokemonMetadata, "/");
	// TODO se debe chequear antes de esto q la carpeta del pokemon existe | si no existe se crea
	strcat(pathPokemonMetadata, pokemon);
	// TODO idem punto anterior. Si el metadata no existe debe crearse
	strcat(pathPokemonMetadata, metadataBin);

	t_config* metadataPokemon;

	metadataPokemon = leerConfiguracion(pathPokemonMetadata);

	printf("%s\n", pathPokemonMetadata);

	config_set_value(metadataPokemon, "OPEN" , "N");
	//config_remove_key(metadataPokemon, "DIRECTORY");
	int numeroSize = config_get_int_value(metadataPokemon, "SIZE");
	// TODO : Hacer que se pueda leer y escribir los archivos metadata.bin de los pokemons

	printf("%i\n", numeroSize);

	// path: {puntoMontaje}/Files/Pikachu/Metadata.bin

}

void escribirBloquePrueba(){
	// path = /home/utnso/Escritorio/tall-grass/24.bin

	posPokemon posicionPokemon = {3,2,10};

	FILE* bloque = fopen( "/home/utnso/Escritorio/tall-grass/24.bin" , "a" );

	//fwrite();

	fclose(bloque);
}

void crearCarpetaPokemonSiNoExiste(char* pathFiles, char* pokemon){
	char* pathCarpetaPokemon = malloc(sizeof(pathFiles) + sizeof(pokemon) + 2);

	//TODO problema con estos concat
	strcpy(pathCarpetaPokemon, pathFiles);
	strcat(pathCarpetaPokemon, "/");
	strcat(pathCarpetaPokemon, pokemon);

	printf("%s\n", pathCarpetaPokemon);

	DIR* dir = opendir(pathCarpetaPokemon);
	if (dir) {
		printf("El directorio existe");
		return;
	} else if (ENOENT == errno) {
	    // El directorio no existe
		printf("El directorio no existe");
		mkdir(pathCarpetaPokemon, 0777);
	} else {
	    printf("Hubo un error inesperado al abrir una carpeta pokemon D:");
	}

}

void crearCarpetaPokemon(char* pathFiles, char* pokemon){


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

	// Datos de metadata/metadata.bin
	int BLOCKS;
	int BLOCK_SIZE;
	char* MAGIC_NUMBER;
	// Funcion para leer metadata.bin
	leerMetadataBin(pathMetadata, &BLOCKS, &BLOCK_SIZE, &MAGIC_NUMBER);

	// Creaar los bloques dentro de carpeta /Blocks
	crearBloquesEn(pathBloques, BLOCKS);

	t_bitarray* bitArrayBloques = crearBitArray(pathMetadata, BLOCKS);


	crearCarpetaPokemonSiNoExiste(pathFiles, "Bulbasaur");

	// No funciona todavia
	// leerUnPokemon(pathFiles, "Pikachu");

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
