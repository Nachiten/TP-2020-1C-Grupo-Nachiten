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

	if(cantArg < 3) //esto es por si ingresan menos argumentos de los necesarios.
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
						pokemonAppeared->corrID = -2; //serializar NECESITA una ID CORRELATIVA para estandarizarlo

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
				//si el 2do argumento es uno de los aceptados, empieza a trabajar, sino, sale automaticamente del switch
				if(((strcmp(arg[2],"NEW_POKEMON") == 0)) || ((strcmp(arg[2],"APPEARED_POKEMON") == 0)) || ((strcmp(arg[2],"CATCH_POKEMON") == 0)) || ((strcmp(arg[2],"CAUGHT_POKEMON") == 0)) || ((strcmp(arg[2],"GET_POKEMON") == 0)))
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
							nuevoPokemon->ID = 0; //serializar NECESITA una ID para estandarizarlo

							//mandamos el mensaje
							mandar_mensaje(nuevoPokemon, NEW, socket);

							//libero la estructura que acabo de crear
							libero_estructura_New(nuevoPokemon);
						}
					}

					if(strcmp(arg[2],"APPEARED_POKEMON") == 0)
					{
						if(cantArg != 7)
						{
							puts("La sintáxis correcta es: ./GameBoy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]");
							return EXIT_FAILURE;
						}

						else
						{
							//Uso una estructura para guardar todos los datos del pokemon y mandarlo junto a la funcion mandar_mensaje
							Appeared* pokemonAppeared = malloc(sizeof(Appeared));
							pokemonAppeared->nombrePokemon = malloc(sizeof(char*));

							pokemonAppeared->nombrePokemon = arg[3];
							pokemonAppeared->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
							pokemonAppeared->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
							pokemonAppeared->corrID = cambia_a_int(arg[6]); //cambiamos el string a int

							//mandamos el mensaje
							mandar_mensaje(pokemonAppeared, APPEARED, socket);

							//libero la estructura que acabo de crear
							libero_estructura_appeared(pokemonAppeared);
						}
					}

					if(strcmp(arg[2],"CATCH_POKEMON") == 0)
					{
						if(cantArg != 6)
						{
							puts("La sintáxis correcta es: ./GameBoy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]");
							return EXIT_FAILURE;
						}

						else
						{
							//Uso una estructura para guardar todos los datos del pokemon y mandarlo junto a la funcion mandar_mensaje
							Catch* pokemonCatch = malloc(sizeof(Catch));
							pokemonCatch->nombrePokemon = malloc(sizeof(char*));

							pokemonCatch->nombrePokemon = arg[3];
							pokemonCatch->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
							pokemonCatch->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
							pokemonCatch->ID = 0; //serializar NECESITA una ID para estandarizarlo

							//mandamos el mensaje
							mandar_mensaje(pokemonCatch, CATCH, socket);

							//libero la estructura que acabo de crear
							libero_estructura_Catch(pokemonCatch);
						}
					}

					if(strcmp(arg[2],"CAUGHT_POKEMON") == 0)
					{
						if(cantArg != 5)
						{
							puts("La sintáxis correcta es: ./GameBoy BROKER CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]");
							return EXIT_FAILURE;
						}

						else
						{
							//Uso una estructura para guardar todos los datos del intento de caught y mandarlo junto a la funcion mandar_mensaje
							Caught* pokemonCaught = malloc(sizeof(Caught));
							pokemonCaught->nombrePokemon = malloc(sizeof(char*)); //wtf?

							pokemonCaught->corrID = cambia_a_int(arg[3]); //cambiamos el string a int
							pokemonCaught->pudoAtrapar = cambia_a_int(arg[4]); //cambiamos el string a int

							//mandamos el mensaje
							mandar_mensaje(pokemonCaught, CAUGHT, socket);

							//libero la estructura que acabo de crear
							libero_estructura_Caught(pokemonCaught);
						}
					}

					if(strcmp(arg[2],"GET_POKEMON") == 0)
					{
						if(cantArg != 4)
						{
							puts("La sintáxis correcta es: ./GameBoy BROKER GET_POKEMON [POKEMON]");
							return EXIT_FAILURE;
						}

						else
						{
							//Uso una estructura para guardar el nombre del pokemon y mandarlo a la funcion mandar_mensaje
							Get* pokemonGet = malloc(sizeof(Get));
							pokemonGet->nombrePokemon = malloc(sizeof(char*));

							pokemonGet->nombrePokemon = arg[3];
							pokemonGet->ID = 0;//serializar NECESITA una ID para estandarizarlo

							//mandamos el mensaje
							mandar_mensaje(pokemonGet, GET, socket);

							//libero la estructura que acabo de crear
							libero_estructura_Get(pokemonGet);
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
				//si el 2do argumento es uno de los aceptados, empieza a trabajar, sino, sale automaticamente del switch
				if(((strcmp(arg[2],"NEW_POKEMON") == 0)) || ((strcmp(arg[2],"CATCH_POKEMON") == 0)) || ((strcmp(arg[2],"GET_POKEMON") == 0)))
				{
					IP = config_get_string_value(config,"IP_GAMECARD"); //cargo la IP del Gamecard.
					PUERTO = config_get_string_value(config,"PUERTO_GAMECARD"); //cargo el puerto del Gamecard.
					socket = establecer_conexion(IP,PUERTO);//creo conexión con Team.
					resultado_de_conexion(socket, logger, "GAMECARD");

					if(strcmp(arg[2],"NEW_POKEMON") == 0)
					{
						if(cantArg != 8)
						{
							puts("La sintáxis correcta es: ./GameBoy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] [ID_MENSAJE]");
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
							nuevoPokemon->ID = cambia_a_int(arg[7]); //cambiamos el string a int

							//mandamos el mensaje
							mandar_mensaje(nuevoPokemon, NEW, socket);

							//libero la estructura que acabo de crear
							libero_estructura_New(nuevoPokemon);
						}
					}

					if(strcmp(arg[2],"CATCH_POKEMON") == 0)
					{
						if(cantArg != 8)
						{
							puts("La sintáxis correcta es: ./GameBoy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]");
							return EXIT_FAILURE;
						}

						else
						{
							//Uso una estructura para guardar todos los datos del pokemon y mandarlo junto a la funcion mandar_mensaje
							Catch* pokemonCatch = malloc(sizeof(Catch));
							pokemonCatch->nombrePokemon = malloc(sizeof(char*));

							pokemonCatch->nombrePokemon = arg[3];
							pokemonCatch->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
							pokemonCatch->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
							pokemonCatch->ID = cambia_a_int(arg[6]); //cambiamos el string a int

							//mandamos el mensaje
							mandar_mensaje(pokemonCatch, CATCH, socket);

							//libero la estructura que acabo de crear
							libero_estructura_Catch(pokemonCatch);
						}
					}

					if(strcmp(arg[2],"GET_POKEMON") == 0)
					{
						if(cantArg != 5)
						{
							puts("La sintáxis correcta es: ./GameBoy GAMECARD GET_POKEMON [POKEMON] [ID_MENSAJE]");
							return EXIT_FAILURE;
						}

						else
						{
							//Uso una estructura para guardar el nombre del pokemon y mandarlo a la funcion mandar_mensaje
							Get* pokemonGet = malloc(sizeof(Get));
							pokemonGet->nombrePokemon = malloc(sizeof(char*));

							pokemonGet->nombrePokemon = arg[3];
							pokemonGet->ID = cambia_a_int(arg[4]); //cambiamos el string a int

							puts("antes del mensaje");
							//mandamos el mensaje
							mandar_mensaje(pokemonGet, GET, socket);

							puts("despues del mensaje");

							//libero la estructura que acabo de crear
							libero_estructura_Get(pokemonGet);
						}
					}
				}
				else
				{
					puts("Al módulo GAMECARD solo se le pueden enviar los siguientes tipos de mensaje: \"NEW_POKEMON\",	\"CATCH_POKEMON\", o \"GET_POKEMON\"");
					socket = establecer_conexion("127.0.0.1","99999");
				}

				break;

		case SUSCRIPTOR://colas de mensajes de 1 a 6
				if(cantArg != 4)
				{
					puts("La sintáxis correcta es: ./GameBoy SUSCRIPTOR [COLA_DE_MENSAJES] [TIEMPO]");
					return EXIT_FAILURE;
				}
				else
				{
					IP = config_get_string_value(config,"IP_BROKER"); //cargo la IP del Broker
					PUERTO = config_get_string_value(config,"PUERTO_BROKER"); //cargo el puerto del Broker
					socket = establecer_conexion(IP,PUERTO);//creo conexión con el Broker.
					resultado_de_conexion(socket, logger, "BROKER");

					//Uso una estructura para guardar el numero de cola al que me quiero subscribir y luego desuscribir y mandarlo a la funcion mandar_mensaje
					Suscripcion* estructuraSuscribirse = malloc(sizeof(Suscripcion));
					Dessuscripcion* estructuraDessuscribirse = malloc(sizeof(Dessuscripcion));
					estructuraSuscribirse->numeroCola = cambia_a_int(arg[2]); //cambiamos el string a int
					estructuraDessuscribirse->numeroCola = cambia_a_int(arg[2]); //cambiamos el string a int

					//mandamos el mensaje pidiendo suscribirse a la cola
					mandar_mensaje(estructuraSuscribirse, SUSCRIPCION, socket);

					//logueamos la suscripcion a la cola de mensajes
					log_info(logger, "Suscripto a la cola de mensajes: %i", arg[2]);

					//Esperamos la cantidad de segundos que hayan pedido antes de enviar el mensaje para la dessuscripcion
					sleep(cambia_a_int(arg[3]));

					//mandamos el mensaje pidiendo dessuscribirse a la cola
					mandar_mensaje(estructuraDessuscribirse, DESSUSCRIPCION, socket);

					//libero las estructuras que acabo de crear
					libero_estructura_Suscripcion(estructuraSuscribirse);
					libero_estructura_Dessuscripcion(estructuraDessuscribirse);
				}
				break;

		default:
				puts("Input no reconocida");
				socket = establecer_conexion("127.0.0.1","99999");
				break;
	}

	/* era para mandar mensaje de prueba, ya no esta implementado
	//Enviamos mensaje de prueba
	mandar_mensaje("1 2 3 probando...\n", TEST, socket);
	*/


	/* antiguo, posiblemente obsoleto, revisar antes de querer usar
	uint32_t size;
	//Para recibir mensajes (coming soon)
	mensaje_recibido = recibir_mensaje(socket, size);
	log_info(logger, mensaje_recibido);
	*/

	//enviarle los recursos a liberar
	matarPrograma(logger, config, socket);

	return EXIT_SUCCESS;
}
