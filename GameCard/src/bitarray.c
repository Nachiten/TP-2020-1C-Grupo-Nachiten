#include "bitarray.h"

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

void liberarUnBloque(int index){

	char* BITARRAY = malloc(BLOCKS / 8);

	// Se espera el semaforo antes de leer el bitarray
	sem_wait(semBitmap);

	leerBitArrayDeArchivo(pathMetadata, &BITARRAY, BLOCKS);

	t_bitarray* bitArrayBloques = crearBitArray(BITARRAY, BLOCKS);

	bitarray_clean_bit(bitArrayBloques, index);

	guardarBitArrayEnArchivo(pathMetadata, BITARRAY, BLOCKS);

	// Se hace el signal luego de guardar el bitarray en archivo
	sem_post(semBitmap);

	bitarray_destroy(bitArrayBloques);
	free(BITARRAY);

}

// Obtiene "cantidad" de bloques libres (bits = 0) del bitmap
t_list* obtenerPrimerosLibresDeBitmap(int cantidad){
	// Al meter un puntero en una lista si ese puntero cambia entonces cambia la lista. Porque guarda una referencia no una copia

	t_list * listaNums = list_create();

	char* BITARRAY_ARCHIVO = malloc(BLOCKS / 8);

	// Se espera el semaforo antes de leer el bitarray
	sem_wait(semBitmap);

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
			//printf("Ya encontre todos los bloques que queria :)");
			break;
		}
		else {
			//printf("El bloque numero: %i Ya está siendo usado..\n", numeroBloque);
		}

		numeroBloque++;

	}

	guardarBitArrayEnArchivo(pathMetadata, BITARRAY_ARCHIVO, BLOCKS);

	// Se hace el signal luego de guardar el bitarray en archivo
	sem_post(semBitmap);

	free(BITARRAY_ARCHIVO);
	bitarray_destroy(bitArray);

	return listaNums;
}

// Printea el bitarray completo, funciona
void printearBitArray(){

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

	bitarray_destroy(bitArray);
	free(BITARRAY_ARCHIVO);

}
