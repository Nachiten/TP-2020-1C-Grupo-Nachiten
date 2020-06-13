#include "gamecard.h"

int main(void) {

	t_log* logger;
	t_config* config;
	char* ip;
	char* puerto;
	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/GameCard.config");

	logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/GameCard/Logs/GameCard.log", "GAMECARD");

	ip = config_get_string_value(config, "IP_BROKER");
	if (ip == NULL){
		log_error(logger, "No se ha podido leer la 'ip' de la config");
		return 3;
	} else {
		log_info(logger, "Se leyo la ip del broker correctamente");
	}
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	if (puerto == NULL){
		log_error(logger, "No se ha podido leer el 'puerto' de la config");
		return 3;
	} else {
		log_info(logger, "Se leyo el puerto del broker correctamente");
	}

	char* pathPuntoMontaje = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	// Como hago para crear mas de una carpeta con el mismo punto de montaje?

	char* pathBlocks = strcat(pathPuntoMontaje, "/Blocks");

	//int retornoFiles = mkdir(strcat(pathPuntoMontaje, "/Files"), 0777);
	//printf("Path punto montaje: %s\n", strcat(pathPuntoMontaje, "/Files"));
	int retornoBlocks = mkdir(pathBlocks, 0777);
	printf("Path punto montaje 2: %s\n", strcat(pathPuntoMontaje, "/Blocks"));

	//printf("RetornoFiles: %i", retornoFiles);
	printf("RetornoBlocks: %i\n", retornoBlocks);

	// Generando un archivo .bin en carpeta blocks
	char* pathCompleto = strcat(pathBlocks, "/1.bin");

	// Queriendo usar variable pathCompleto expĺota con segmentation fault :(
	FILE* bloque = fopen( "/home/utnso/Escritorio/tall-grass/Blocks/1.bin", "w+" );
	fclose(bloque);

	/*
	int i;
	char numBloque = i + 48;
	for(i = 1; i<= 8; i++){

		char* pathConBarra = strcat(pathBlocks, "/");
		char* pathConNumBloque = strcat(pathConBarra, "2");
		char* pathConExtension = strcat(pathConNumBloque, ".bin");

		FILE* bloque = fopen( pathConExtension, "w+" );

		int retornoCerrarArchivo = fclose(bloque);
		if ( retornoCerrarArchivo != 0){
			printf("Hubo un problema al cerrar el archivo");
		}

		numBloque++;
	}
	*/


	//printf("%i", retornoCrearDir);

	return EXIT_SUCCESS;
}
