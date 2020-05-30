#include "GameBoy.h"

int main(int cantArg, char* arg[]) {

	t_log* logger;
	t_config* config;
	uint32_t socket;
	//NO TOCAR LOS MALLOC O EXPLOTA - (viejo, ignorar)
	char* IP;
	char* PUERTO;
	char* LOG_PATH;
	char* mensaje_recibido; //no implementado todavia
	uint32_t switcher = DEFAULT; //para usar el switch case.

	//meter un 2 en el if para probar por terminal, un 0 para probar x eclipse <--------------------
	if(cantArg < 2) //esto es por si ingresan menos argumentos de los necesarios.
	{
		puts("Segmentation fault(te_la_creiste) \nHay que ingresar mas argumentos, campeón ;)");
		return EXIT_FAILURE;
	}

	//Cargo las configuraciones del .config
	config = leerConfiguracion("/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/GameBoy.config");
	LOG_PATH = config_get_string_value(config,"LOG_FILE_PATH"); //cargo el path del archivo log
	//Dejo cargado un logger para loguear los eventos.
	logger = cargarUnLog(LOG_PATH, "Gameboy");


	//NOTA: no usar arg[0] es el: "./GameBoy"
	switcher = valor_para_switch_case(arg[1]); //segun el primer parametro que se ingresa por terminal, decide donde va a ir el switch case

	switch(switcher)
	{
		case TEAM:
				//aca trabajo con el 2do argumento que ingresa por terminal: arg[2]
				if(strcmp(arg[2],"APPEARED_POKEMON") == 0)
				{					
					if(cantArg != 6)
					{
						puts("La sintáxis correcta es: ./GameBoy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]");
						return EXIT_FAILURE;
					}
					else
					{	
						IP = config_get_string_value(config,"IP_TEAM"); //cargo la IP del Team.
						PUERTO = config_get_string_value(config,"PUERTO_TEAM"); //cargo el puerto del Team.
						socket = establecer_conexion(IP,PUERTO);//creo conexión con Team.
						resultado_de_conexion(socket, logger, "TEAM");

						//Uso una estructura para guardar todos los datos del pokemon y mandarlo todo junto a la funcion mandar_mensaje
						Appeared* pokemonAppeared = malloc(sizeof(Appeared));
						pokemonAppeared->nombrePokemon = malloc(sizeof(char*));

						pokemonAppeared->nombrePokemon = arg[3];
						pokemonAppeared->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
						pokemonAppeared->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int

						//mandamos el mensaje
						mandar_mensaje(pokemonAppeared, APPEARED, socket);

						//libero la estructura que acabo de crear
						libero_estructura_appeared(pokemonAppeared);
					}
				}
				else
				{
					puts("Al módulo TEAM solo se le puede enviar el mensaje \"APPEARED_POKEMON\"");
					socket = establecer_conexion("127.0.0.1","99999");
				}
				break;

		case BROKER:
				if(strcmp(arg[2],"NEW_POKEMON") == 0) //IF DE TODAS LAS POSIBILIDADES, faltan: \"APPEARED_POKEMON\" \"CATCH_POKEMON\",	\"CUGHT_POKEMON\" o \"GET_POKEMON\"");
				{
					IP = config_get_string_value(config,"IP_BROKER"); //cargo la IP del Broker
					PUERTO = config_get_string_value(config,"PUERTO_BROKER"); //cargo el puerto del Broker
					socket = establecer_conexion(IP,PUERTO);//creo conexión con el Broker.
					resultado_de_conexion(socket, logger, "BROKER");

					if(strcmp(arg[2],"NEW_POKEMON") == 0)
					{
						if(cantArg != 7)
						{
							puts("La sintáxis correcta es: ./GameBoy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]");
							return EXIT_FAILURE;
						}
						else
						{
							//Uso una estructura para guardar todos los datos del pokemon y mandarlo junto a la funcion mandar_mensaje
							New* nuevoPokemon = malloc(sizeof(New));
							nuevoPokemon->nombrePokemon = malloc(sizeof(char*));

							nuevoPokemon->nombrePokemon = arg[3];
							nuevoPokemon->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
							nuevoPokemon->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
							nuevoPokemon->cantPokemon = cambia_a_int(arg[6]); //cambiamos el string a int

							//mandamos el mensaje
							mandar_mensaje(nuevoPokemon, NEW, socket);

							//libero la estructura que acabo de crear
							libero_estructura_New(nuevoPokemon);
						}
					}
				}
				else
				{
					puts("Al módulo BROKER solo se le pueden enviar los siguientes tipos de mensaje: \"APPEARED_POKEMON\", \"NEW_POKEMON\",	\"CATCH_POKEMON\",	\"CUGHT_POKEMON\" o \"GET_POKEMON\"");
					socket = establecer_conexion("127.0.0.1","99999");
				}
				break;

		case GAMECARD:
				puts("Switch del gamecard, falta implementar");
				IP = config_get_string_value(config,"IP_GAMECARD"); //cargo la IP del Gamecard.
				PUERTO = config_get_string_value(config,"PUERTO_GAMECARD"); //cargo el puerto del Gamecard.
				socket = establecer_conexion(IP,PUERTO);//creo conexión con Team.
				resultado_de_conexion(socket, logger, "GAMECARD");

				//Enviamos mensaje de prueba
				mandar_mensaje("1 2 3 probando...\n", TEST, socket);
				break;

		case SUSCRIPTOR://Por ahora solo sirve como test de envio de mensajes
				puts("Switch del suscriptor, falta implementar");
				socket = establecer_conexion("127.0.0.1","99999");
				break;

		default:
				puts("Input no reconocida");
				socket = establecer_conexion("127.0.0.1","99999");
				break;
	}

	/* antiguo, posiblemente obsoleto, revisar antes querer usar
	uint32_t size;
	//Para recibir mensajes (coming soon)
	mensaje_recibido = recibir_mensaje(socket, size);
	log_info(logger, mensaje_recibido);
	*/

	//enviarle los recursos a liberar
	matarPrograma(logger, config, socket);

	return EXIT_SUCCESS;
}
