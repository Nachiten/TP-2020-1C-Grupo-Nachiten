#include "GameBoy.h"

int main(int cantArg, char* arg[]) {

	t_log* logger;
	t_config* config;
	uint32_t socket;
	//NO TOCAR LOS MALLOC O EXPLOTA TODO
	char* IP;
	char* PUERTO;
	char* mensaje_recibido;
	uint32_t switcher = DEFAULT; //para usar el switch case.

	//meter un 2 en el if para probar por terminal, un 0 para probar x eclipse <------------------------------------------------------------------
	if(cantArg < 2) //esto es por si ingresan menos argumentos de los necesarios.
	{
		puts("Segmentation fault(te_la_creiste) \nHay que ingresar mas argumentos, campeón ;)");
		return EXIT_FAILURE;
	}

	//Dejo cargado un logger para loguear los eventos.
	logger = cargarUnLog("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/GameBoy/Logs/Gameboy.log", "Gameboy");
	//Cargo las configuraciones del .config
	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/GameBoy.config");

	//NOTA: no usar arg[0] es el: "./GameBoy"
	switcher = valor_para_switch_case(arg[1]); //segun el primer parametro que se ingresa por terminal, decide donde va a ir el switch case

	switch(switcher)
	{
		case TEAM:
				//aca trabajo con el 2do argumento que ingresa por terminal: arg[2]
				if(strcmp(arg[2],"APPEARED_POKEMON") == 0)
				{					
					if(cantArg == 6)
					{
						puts("la sintáxis correcta es: ./GameBoy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY].");
						return EXIT_FAILURE;
					}
					else
					{	
						IP = config_get_string_value(config,"IP_TEAM"); //cargo la IP del Team.
						PUERTO = config_get_string_value(config,"PUERTO_TEAM"); //cargo el puerto del Team.
						socket = establecer_conexion(IP,PUERTO);//creo conexión con Team.
						resultado_de_conexion(socket, logger, "TEAM");


						Appeared* pokemonAppeared;

						pokemonAppeared->nombrePokemon = arg[3];
						pokemonAppeared->posPokemon.x = arg[4];// pasar estos dos cositos por la funcion que los transforma a ints
						pokemonAppeared->posPokemon.y = arg[5];

						mandar_mensaje(pokemonAppeared, APPEARED, socket);
					}
				}
				else
				{
					puts("Al módulo TEAM solo se le puede enviar el mensaje \"APPEARED_POKEMON\"");
					socket = establecer_conexion("127.0.0.1","99999");
				}
				break;

		case BROKER://Por ahora solo sirve como test de envio de mensajes
				IP = config_get_string_value(config,"IP_BROKER"); //cargo la IP del Broker
				PUERTO = config_get_string_value(config,"PUERTO_BROKER"); //cargo el puerto del Broker
				socket = establecer_conexion(IP,PUERTO);//creo conexión con el Broker.
				resultado_de_conexion(socket, logger, "BROKER");

				//Enviamos mensaje de prueba
				mandar_mensaje("Mensaje de prueba\n", TEST, socket);
				break;

		case GAMECARD:
				puts("Switch del gamecard, falta implementar");
				IP = config_get_string_value(config,"IP_GAMECARD"); //cargo la IP del Gamecard.
				PUERTO = config_get_string_value(config,"PUERTO_GAMECARD"); //cargo el puerto del Gamecard.
				socket = establecer_conexion(IP,PUERTO);//creo conexión con Team.
				resultado_de_conexion(socket, logger, "GAMECARD");
				break;

		case SUSCRIPTOR:
				puts("Switch del suscriptor, falta implementar");
				socket = establecer_conexion("127.0.0.1","99999");
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

	return EXIT_SUCCESS;
}
