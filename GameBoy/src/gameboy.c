#include "GameBoy.h"

int main(void) {

	/*
	int i = 1;
	printf("Probando GameBoy %i", i);
	*/

	t_log* logger;
	t_config* config;
	int socket;
	char* IP;
	char* PUERTO;
	char* mensaje_recibido;
	//NO TOCAR LOS MALLOC O EXPLOTA TODO
	char* entrada1 = malloc(sizeof(char*)); //aca se guarda la primera orden que llega al gameboy
	char* entrada2 = malloc(sizeof(char*)); //falta implementar
	int switcher = DEFAULT; //para usar el switch case.

	//Dejo cargado un logger para loguear los eventos.
	logger = cargarUnLog("Logs/Gameboy.log", "Gameboy");
	//Cargo las configuraciones del .config
	config = leerConfiguracion("../Configs/GameBoy.config");


	//leo lo que llegue por consola/terminal.
	scanf("%s %s", entrada1, entrada2);// EN DESARROLLO LAS ENTRADAS <------------------------------------------

	switcher = valor_para_switch_case(entrada1); //segun la primera palabra que ingresa por consola, decide donde va a ir el switch case

	switch(switcher)
	{
		case TEAM:
				IP = config_get_string_value(config,"IP_TEAM"); //cargo la IP del Team.
				PUERTO = config_get_string_value(config,"PUERTO_TEAM"); //cargo el puerto del Team.
				socket = establecer_conexion(IP,PUERTO);//creo conexión con Team.
				if(socket == -1)
				{
				log_warning(logger, "Conexión fallida con socket de TEAM");
				}
				else
				{
				log_info(logger, "Conexión establecida con socket de TEAM");
				}
				break;

		case BROKER://Por ahora solo sirve como test de envio de mensajes
				IP = config_get_string_value(config,"IP_BROKER"); //cargo la IP del Broker
				PUERTO = config_get_string_value(config,"PUERTO_BROKER"); //cargo el puerto del Broker
				socket = establecer_conexion(IP,PUERTO);//creo conexión con el Broker.
				if(socket == -1)
				{
				log_warning(logger, "Conexión fallida con socket de BROKER");
				}
				else
				{
				log_info(logger, "Conexión establecida con socket de BROKER");
				}
				//Enviamos mensaje de prueba
				mandar_mensaje("mensaje de prueba de mandar mensaje\n", TEST, socket);
				break;

		case GAMECARD:
				puts("Switch del gamecard, falta implementar");
				IP = config_get_string_value(config,"IP_GAMECARD"); //cargo la IP del Gamecard.
				PUERTO = config_get_string_value(config,"PUERTO_GAMECARD"); //cargo el puerto del Gamecard.
				socket = establecer_conexion(IP,PUERTO);//creo conexión con Team.
				if(socket == -1)
				{
				log_warning(logger, "Conexión fallida con socket de GAMECARD");
				}
				else
				{
				log_info(logger, "Conexión establecida con socket de GAMECARD");
				}
				break;

		case SUSCRIPTOR:
				puts("Switch del suscriptor, falta implementar");
				break;

		default:
				puts("Input no reconocida");
				socket = establecer_conexion("127.0.0.1","99999");
				break;
	}

	/*
	uint32_t size;
	//Para recibir mensajes (coming soon)
	mensaje_recibido = recibir_mensaje(socket, size);
	//se supone que recibimos un mensaje diciendo que la conexion fue exitosa o algo asi
	log_info(logger, mensaje_recibido);
	*/


	//enviarle los recursos a liberar
	matarPrograma(logger, config, socket);
	free(entrada1);
	free(entrada2);

	return EXIT_SUCCESS;
}
