#include "gamecard.h"

char* crearCarpetaBlocks(char* pathPuntoMontaje){
	char* carpetaBlocks = "/Blocks";

	// Path de la carpeta {punto_montaje}/Blocks
	char* pathBloques = malloc(strlen(pathPuntoMontaje) + strlen(carpetaBlocks) + 1);
	// Hacer este malloc explota por sf | = malloc(strlen(pathPuntoMontaje + 1));

	// Copiando el punto de montaje al path de los bloques
	strcpy(pathBloques, pathPuntoMontaje);

	// Agrego el path de la carpeta /Blocks
	strcat(pathBloques, carpetaBlocks);

	int retornoBlocks = mkdir(pathBloques, 0777);

	printf("Retorno creando carpeta Blocks: %i\n", retornoBlocks);

	printf("Path bloques: %s\n", pathBloques);

	return pathBloques;
}

void crearBloquesEn(char* pathBloques, int cantidadBloques){
	int bloqueActual = 4; // Esto estaria dentro de un for

	// Array de chars para meter el int convertido a array
	char enteroEnLetras[5];

	// Genero un array de chars que es el numero de bloque pasado a string
	sprintf(enteroEnLetras, "%i", bloqueActual);
	// IMPORTANTE: Como convertir entero a string

	char* extension = ".bin";

	// 5 = longitud del numero + 1 por la /
	char* nombreArchivo = malloc( 6 + strlen(extension) + 1 );

	strcpy(nombreArchivo, "/");
	strcat(nombreArchivo, &enteroEnLetras);
	strcat(nombreArchivo, extension);

	printf("Nombre archivo: %s\n", nombreArchivo);

	// Creo una copia del path de /Blocks para no modificarlo
	char* pathBloque1 = malloc(strlen(pathBloques) + strlen(nombreArchivo) + 1);

	// Le pego el path de /Blocks
	strcpy(pathBloque1, pathBloques);

	// Le pego el valor hardcodeado 1.bin
	strcat(pathBloque1, nombreArchivo);

	printf("Path bloque 1: %s\n", pathBloque1);

	// Queriendo usar variable pathCompleto expĺota con segmentation fault :(
	FILE* bloque = fopen( pathBloque1 , "w+" );
	fclose(bloque);
}

int main(void) {

	t_log* logger;
	t_config* config;
	char* ip;
	char* puerto;
	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/GameCard.config");

	logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/GameCard/Logs/GameCard.log", "GAMECARD");

//	ip = config_get_string_value(config, "IP_BROKER");
//	if (ip == NULL){
//		log_error(logger, "No se ha podido leer la 'ip' de la config");
//		return 3;
//	} else {
//		log_info(logger, "Se leyo la ip del broker correctamente");
//	}
//	puerto = config_get_string_value(config, "PUERTO_BROKER");
//	if (puerto == NULL){
//		log_error(logger, "No se ha podido leer el 'puerto' de la config");
//		return 3;
//	} else {
//		log_info(logger, "Se leyo el puerto del broker correctamente");
//	}

	char* pathPuntoMontaje = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");

	// Crear la carpeta /Blocks
	char* pathBloques = crearCarpetaBlocks(pathPuntoMontaje);

	int cantidadBloques = 100; // Hardcodeado (//TODO debe leerse

	// Creaar los bloques dentro de carpeta /Blocks
	crearBloquesEn(pathBloques, cantidadBloques);

/*
	int i = 35;
	char enteroEnLetras[5];


		char* pathBloqueCompleto = malloc(35);

		// Le pego el path de la carpeta /Blocks
		strcpy(pathBloqueCompleto, pathBloques);

		printf("%s", pathBloqueCompleto);
		*/

		/*
		// Le pego al path una / luego del nombre de carpeta
		strcat(pathBloqueCompleto, "/");

		// Genero un array de chars que es el numero de bloque pasado a string
		sprintf(enteroEnLetras, "%i", i);
		// IMPORTANTE: Como convertir entero a string

		// Pego el numero al path del bloque
		strcat(pathBloqueCompleto, enteroEnLetras);

		// Agrego la extension
		strcat(pathBloqueCompleto, ".bin");

		printf("Path total: %s\n", pathBloqueCompleto);

		//TODO Abrir un archivo con este nombre "pathBloqueCompleto" y luego cerrarlo
		*/



	return EXIT_SUCCESS;
}
