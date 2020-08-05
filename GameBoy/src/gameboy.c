#include "GameBoy.h"

int main(int cantArg, char* arg[]) {

	t_log* logger;
	t_config* config;
	int32_t socket = 0;
//	char* IP;
//	char* PUERTO;
	char* LOG_PATH;
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

						if(socket != -1)//si y solo si se puede conectar
						{
							//Uso una estructura para guardar todos los datos del pokemon y mandarlo a la funcion mandar_mensaje
							Appeared* pokemonAppeared = malloc(sizeof(Appeared));

							pokemonAppeared->nombrePokemon = arg[3];
							pokemonAppeared->largoNombre = strlen(arg[3]);
							pokemonAppeared->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
							pokemonAppeared->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
							pokemonAppeared->ID = 0;
							pokemonAppeared->corrID = -2;

							//mandamos el mensaje
							mandar_mensaje(pokemonAppeared, APPEARED, socket);

							//libero la estructura que acabo de crear
							free(pokemonAppeared);
						}
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

					if(socket != -1)//si y solo si se puedo conectar
					{
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

								nuevoPokemon->nombrePokemon = arg[3];
								nuevoPokemon->largoNombre = strlen(arg[3]);
								nuevoPokemon->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
								nuevoPokemon->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
								nuevoPokemon->cantPokemon = cambia_a_int(arg[6]); //cambiamos el string a int
								nuevoPokemon->ID = 0;
								nuevoPokemon->corrID = -2;

								//mandamos el mensaje
								mandar_mensaje(nuevoPokemon, NEW, socket);

								//libero la estructura que acabo de crear
								free(nuevoPokemon);
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

								pokemonAppeared->nombrePokemon = arg[3];
								pokemonAppeared->largoNombre = strlen(arg[3]);
								pokemonAppeared->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
								pokemonAppeared->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
								pokemonAppeared->ID = 0;
								pokemonAppeared->corrID = cambia_a_int(arg[6]); //cambiamos el string a int

								//mandamos el mensaje
								mandar_mensaje(pokemonAppeared, APPEARED, socket);

								//libero la estructura que acabo de crear
								free(pokemonAppeared);
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

								pokemonCatch->nombrePokemon = arg[3];
								pokemonCatch->largoNombre = strlen(arg[3]);
								pokemonCatch->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
								pokemonCatch->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
								pokemonCatch->ID = 0;
								pokemonCatch->corrID = -2;

								//mandamos el mensaje
								mandar_mensaje(pokemonCatch, CATCH, socket);

								//libero la estructura que acabo de crear
								free(pokemonCatch);
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

								pokemonCaught->corrID = cambia_a_int(arg[3]); //cambiamos el string a int

								//Ponemos el resultado de intentar atrapar
								if (strcmp(arg[4],"OK") == 0)
								{
									pokemonCaught->pudoAtrapar = 1;
								}
								else
								{
									if (strcmp(arg[4],"FAIL") == 0)
									{
										pokemonCaught->pudoAtrapar = 0;
									}
								}

								//lleno de basura para completar la estructura
								pokemonCaught->ID = 0;
								pokemonCaught->nombrePokemon = "Default";
								pokemonCaught->largoNombre = strlen(pokemonCaught->nombrePokemon);

								//mandamos el mensaje
								mandar_mensaje(pokemonCaught, CAUGHT, socket);

								//libero la estructura que acabo de crear
								free(pokemonCaught);
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

								pokemonGet->largoNombre = strlen(arg[3]);
								pokemonGet->nombrePokemon = arg[3];
								pokemonGet->ID = 0;//serializar NECESITA una ID para estandarizarlo
								pokemonGet->corrID = -2;

								//mandamos el mensaje
								mandar_mensaje(pokemonGet, GET, socket);

								//libero la estructura que acabo de crear
								free(pokemonGet);
							}
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

					if(socket != -1)//si y solo si se puedo conectar
					{
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

								nuevoPokemon->nombrePokemon = arg[3];
								nuevoPokemon->largoNombre = strlen(arg[3]);

								nuevoPokemon->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
								nuevoPokemon->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
								nuevoPokemon->cantPokemon = cambia_a_int(arg[6]); //cambiamos el string a int
								nuevoPokemon->ID = cambia_a_int(arg[7]); //cambiamos el string a int
								nuevoPokemon->corrID = -2; //TIENE que tener algo, aunque GAMECARD no se fija en corrID?

								//mandamos el mensaje
								mandar_mensaje(nuevoPokemon, NEW, socket);

								//libero la estructura que acabo de crear
								free(nuevoPokemon);
							}
						}

						if(strcmp(arg[2],"CATCH_POKEMON") == 0)
						{
							if(cantArg != 7)
							{
								puts("La sintáxis correcta es: ./GameBoy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]");
								return EXIT_FAILURE;
							}

							else
							{
								//Uso una estructura para guardar todos los datos del pokemon y mandarlo junto a la funcion mandar_mensaje
								Catch* pokemonCatch = malloc(sizeof(Catch));

								pokemonCatch->nombrePokemon = arg[3];
								pokemonCatch->largoNombre = strlen(arg[3]);
								pokemonCatch->posPokemon.x = cambia_a_int(arg[4]); //cambiamos el string a int
								pokemonCatch->posPokemon.y = cambia_a_int(arg[5]); //cambiamos el string a int
								pokemonCatch->ID = cambia_a_int(arg[6]); //cambiamos el string a int
								pokemonCatch->corrID = -2;

								//mandamos el mensaje
								mandar_mensaje(pokemonCatch, CATCH, socket);

								//libero la estructura que acabo de crear
								free(pokemonCatch);
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

								pokemonGet->largoNombre = strlen(arg[3]);
								pokemonGet->nombrePokemon = arg[3];
								pokemonGet->ID = cambia_a_int(arg[4]); //cambiamos el string a int
								pokemonGet->corrID = -2;

								//mandamos el mensaje
								mandar_mensaje(pokemonGet, GET, socket);

								//libero la estructura que acabo de crear
								free(pokemonGet);
							}
						}

					}
				}
				else
				{
					puts("Al módulo GAMECARD solo se le pueden enviar los siguientes tipos de mensaje: \"NEW_POKEMON\", \"CATCH_POKEMON\", o \"GET_POKEMON\"");
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
					if((cambia_a_int(arg[2]) > 0) && (cambia_a_int(arg[2]) <= 6))
					{
						IP = config_get_string_value(config,"IP_BROKER"); //cargo la IP del Broker
						PUERTO = config_get_string_value(config,"PUERTO_BROKER"); //cargo el puerto del Broker
						socket = establecer_conexion(IP,PUERTO);//creo conexión con el Broker.
						resultado_de_conexion(socket, logger, "BROKER");

						if(socket != -1)//si y solo si se puedo conectar
						{
							//Uso una estructura para guardar el numero de cola al que me quiero subscribir y luego desuscribir y mandarlo a la funcion mandar_mensaje
							Suscripcion* estructuraSuscribirse = malloc(sizeof(Suscripcion));
							Dessuscripcion* estructuraDessuscribirse = malloc(sizeof(Dessuscripcion));

							estructuraSuscribirse->numeroCola = cambia_a_int(arg[2]); //cambiamos el string a int
							estructuraSuscribirse->pId = getpid(); //ponemos la ID del proceso
							estructuraDessuscribirse->numeroCola = cambia_a_int(arg[2]); //cambiamos el string a int
							estructuraDessuscribirse->pId = getpid(); //ponemos la ID del proceso

							//Preparamos una estructura para recibir los mensajes de la suscripcion en un hilo
							pthread_t hilo;
							HiloGameboy estructura;
							estructura.conexion = socket;
							estructura.log = logger;
							estructura.cola = cambia_a_int(arg[2]);
							estructura.pID = getpid();

							//mandamos el mensaje pidiendo suscribirse a la cola
							mandar_mensaje(estructuraSuscribirse, SUSCRIPCION, socket);

							//logueamos la suscripcion a la cola de mensajes
							log_info(logger, "Suscripto a la cola de mensajes: %i", cambia_a_int(arg[2]));

							//hilo para recibir mensajes
							pthread_create(&hilo,NULL,(void*)hilo_recibir_mensajes,&estructura);
							pthread_detach(hilo);

							//Esperamos la cantidad de segundos que hayan pedido antes de enviar el mensaje para la dessuscripcion
							sleep(cambia_a_int(arg[3]));

							//mandamos el mensaje pidiendo dessuscribirse a la cola
							mandar_mensaje(estructuraDessuscribirse, DESSUSCRIPCION, socket);

							//libero las estructuras que acabo de crear para suscribirme y dessuscribirme
							free(estructuraSuscribirse);
							free(estructuraDessuscribirse);
						}
					}
					else
					{
						puts("Las colas de mensajes son de 1 a 6. Por favor ingrese un número de cola válido.");
						socket = establecer_conexion("127.0.0.1","99999");
					}
				}
				break;

		default:
				puts("Input no reconocida");
				socket = establecer_conexion("127.0.0.1","99999");
				break;
	}

	//enviarle los recursos a liberar
	matarPrograma(logger, config, socket);

	return EXIT_SUCCESS;
}

void hilo_recibir_mensajes(HiloGameboy* estructura)
{
	int32_t size = 1;
	int32_t tamanioRecibido = 1;
	codigo_operacion cod_op;
	int32_t socketConfirm;

	New* mensajeNew;
	Appeared* mensajeAppeared;
	Get* mensajeGet;
	Localized* mensajeLocalized;
	Catch* mensajeCatch;
	Caught* mensajeCaught;
	confirmacionMensaje* mensajeConfirm;

	while(tamanioRecibido > 0 || size > 0)
	{
		tamanioRecibido = recv(estructura->conexion, &cod_op, sizeof(codigo_operacion),MSG_WAITALL);
		bytesRecibidos(tamanioRecibido);

		tamanioRecibido = recv(estructura->conexion, &size, sizeof(int32_t),MSG_WAITALL);
		bytesRecibidos(tamanioRecibido);

		//tomo la ID del mensaje para saber si llego uno nuevo
		switch(cod_op){
			case NEW:;
				mensajeNew = malloc(size);
				mensajeConfirm = malloc(sizeof(confirmacionMensaje));
				recibir_mensaje(mensajeNew,cod_op,estructura->conexion);
				log_info(estructura->log, "Recibido un nuevo mensaje en la cola: %u",estructura->cola);
				mensajeConfirm->id_mensaje = mensajeNew->ID;
				mensajeConfirm->colaMensajes = cod_op;
				mensajeConfirm->pId = estructura->pID;
				socketConfirm = establecer_conexion(IP, PUERTO);
				mandar_mensaje(mensajeConfirm, CONFIRMACION, socketConfirm);
				cerrar_conexion(socketConfirm);
				free(mensajeNew->nombrePokemon);
				free(mensajeNew);
				free(mensajeConfirm);
				break;

			case APPEARED:
				mensajeAppeared = malloc(size);
				mensajeConfirm = malloc(sizeof(confirmacionMensaje));
				recibir_mensaje(mensajeAppeared,cod_op,estructura->conexion);
				log_info(estructura->log, "Recibido un nuevo mensaje en la cola: %u",estructura->cola);
				mensajeConfirm->id_mensaje = mensajeAppeared->ID;
				mensajeConfirm->colaMensajes = cod_op;
				mensajeConfirm->pId = estructura->pID;
				socketConfirm = establecer_conexion(IP, PUERTO);
				mandar_mensaje(mensajeConfirm, CONFIRMACION, socketConfirm);
				cerrar_conexion(socketConfirm);
				free(mensajeAppeared->nombrePokemon);
				free(mensajeAppeared);
				free(mensajeConfirm);
				break;

			case GET:
				mensajeGet = malloc(size);
				mensajeConfirm = malloc(sizeof(confirmacionMensaje));
				recibir_mensaje(mensajeGet,cod_op,estructura->conexion);
				log_info(estructura->log, "Recibido un nuevo mensaje en la cola: %u",estructura->cola);
				mensajeConfirm->id_mensaje = mensajeGet->ID;
				mensajeConfirm->colaMensajes = cod_op;
				mensajeConfirm->pId = estructura->pID;
				socketConfirm = establecer_conexion(IP, PUERTO);
				mandar_mensaje(mensajeConfirm, CONFIRMACION, socketConfirm);
				cerrar_conexion(socketConfirm);
				free(mensajeGet->nombrePokemon);
				free(mensajeGet);
				free(mensajeConfirm);
				break;

			case LOCALIZED:
				mensajeLocalized = malloc(size);
				mensajeConfirm = malloc(sizeof(confirmacionMensaje));
				recibir_mensaje(mensajeLocalized,cod_op,estructura->conexion);
				log_info(estructura->log, "Recibido un nuevo mensaje en la cola: %u",estructura->cola);
				mensajeConfirm->id_mensaje = mensajeLocalized->ID;
				mensajeConfirm->colaMensajes = cod_op;
				mensajeConfirm->pId = estructura->pID;
				socketConfirm = establecer_conexion(IP, PUERTO);
				mandar_mensaje(mensajeConfirm, CONFIRMACION, socketConfirm);
				cerrar_conexion(socketConfirm);
				free(mensajeLocalized->nombrePokemon);
				free(mensajeLocalized);
				free(mensajeConfirm);
				break;

			case CATCH:
				mensajeCatch = malloc(size);
				mensajeConfirm = malloc(sizeof(confirmacionMensaje));
				recibir_mensaje(mensajeCatch,cod_op,estructura->conexion);
				log_info(estructura->log, "Recibido un nuevo mensaje en la cola: %u",estructura->cola);
				mensajeConfirm->id_mensaje = mensajeCatch->ID;
				mensajeConfirm->colaMensajes = cod_op;
				mensajeConfirm->pId = estructura->pID;
				socketConfirm = establecer_conexion(IP, PUERTO);
				mandar_mensaje(mensajeConfirm, CONFIRMACION, socketConfirm);
				cerrar_conexion(socketConfirm);
				free(mensajeCatch->nombrePokemon);
				free(mensajeCatch);
				free(mensajeConfirm);
				break;

			case CAUGHT:
				mensajeCaught = malloc(size);
				mensajeConfirm = malloc(sizeof(confirmacionMensaje));
				recibir_mensaje(mensajeCaught,cod_op,estructura->conexion);
				log_info(estructura->log, "Recibido un nuevo mensaje en la cola: %u",estructura->cola);
				mensajeConfirm->id_mensaje = mensajeCaught->ID;
				mensajeConfirm->colaMensajes = cod_op;
				mensajeConfirm->pId = estructura->pID;
				socketConfirm = establecer_conexion(IP, PUERTO);
				mandar_mensaje(mensajeConfirm, CONFIRMACION, socketConfirm);
				cerrar_conexion(socketConfirm);
				free(mensajeCaught->nombrePokemon);
				free(mensajeCaught);
				free(mensajeConfirm);
				break;

			default:
				break;

		}
	}
}
