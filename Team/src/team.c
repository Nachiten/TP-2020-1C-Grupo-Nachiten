#include "Team.h"

void (*moverse_a)(d_entrenador*, int, int, int);
sem_t* sem_entrenadores;
sem_t colaMensajes_llenos, datosHilo, enExec, entrenadores_disponibles, colaReady_llenos, extraccion_mensaje_caught;// colaCaught_llenos;
pthread_mutex_t colaMensajes_mutex, objetivo_actual_mutex, colaReady_mutex;//, colaCaught_mutex;
char** objetivo_actual;
int estado_team, objetivo_team, cantidad_objetivos, quantum;

int main(int cantArgs, char* arg[])
{
	char* pathConfig;

	if (cantArgs < 2){
		printf("ERROR | No se insertó un path de archivo de configuracion, se usará el default de carpeta Configs (TeamPruebaIndividual)\n");
		pathConfig = "/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/TeamPruebaIndividual.config";
	} else if (cantArgs == 2) {
		printf("INFO | El path seleccionado de config es %s\n", arg[1]);
		pathConfig = arg[1];
	} else {
		printf("ERROR | Se insertaron parametros de mas al correr el programa");
	}

	PID = getpid(); //ignorar warning, SI SE USA
	printf("PID del TEAM: %i.\n",PID);

	//inicializamos datos para los hilos de recibir mensajes
	datosAppearedGameboy = malloc(sizeof(datosHiloColas));
	datosAppeared = malloc(sizeof(datosHiloColas));
	datosLocalized = malloc(sizeof(datosHiloColas));
	datosCaught = malloc(sizeof(datosHiloColas));
	datosAppearedGameboy->cola = APPEARED;
	datosAppeared->cola = APPEARED;
	datosLocalized->cola = LOCALIZED;
	datosCaught->cola = CAUGHT;

	cola_indices = malloc(sizeof(cola_mensajes_team)); //inicializacion de cola de mensajes

	//inicializamos semaforo para loguear eventos
	semLog = malloc(sizeof(sem_t));
	sem_init(semLog, 0, 1);

	iDFalsaTeam = 0; //ID para cuando NO se puede conectar al team
	//inicializamos semaforo de la ID trucha
	semIDFALSATEAM = malloc(sizeof(sem_t));
	sem_init(semIDFALSATEAM, 0, 1);

	// Contador de ciclos de CPU y su semaforo
	semCiclosCPU = malloc(sizeof(sem_t));
	sem_init(semCiclosCPU, 0, 1);
	ciclosCPUTotales = 0;

	// Contador de cambios de contexto totales
	semCambiosContexto = malloc(sizeof(sem_t));
	sem_init(semCambiosContexto, 0, 1);
	cambiosContextoTotales = 0;


	t_config* config;

	//Cargo las configuraciones del .config
	config = leerConfiguracion(pathConfig);
	if (config == NULL)
	{
		printf("No se pudo leer la config.\n");
	}
	else
	{
		printf("La config fue leida correctamente.\n");
	}
	// Leer path del log
	char* pathLogs = config_get_string_value(config,"LOG_FILE");
	//Dejo cargado un logger para loguear los eventos.
	logger = cargarUnLog(pathLogs, "Team");
	IP = config_get_string_value(config, "IP_BROKER");
	PUERTO = config_get_string_value(config, "PUERTO_BROKER");
	PUERTOTEAM = config_get_string_value(config, "PUERTO_TEAM");

	estado_team = 0;
	d_entrenador* entrenadores;
	pthread_t hilo_recibir_mensajes,hilo_recibir_mensajes_gameboy, hilo_cola_ready, hilo_cola_caught, hilo_mensajes_get;
	pthread_t* pool_hilos;
	mensaje_server* mensaje = malloc(sizeof(mensaje_server));
	int cant_entrenadores, algoritmo_planificacion, estimacion_inicial, flag_finalizacion, i, pos_elegido, temp_cant;
	int primer_extraccion, segunda_extraccion;

	//saca Algoritmo planificacion, quantum, valor de estimacion inicial (para sjf), retardo de ciclo de CPU y espera antes de reconectarse
	primer_extraccion = extraer_valores_config(config, &algoritmo_planificacion, &quantum, &estimacion_inicial, &retardo, &tiempo_reconexion);

	segunda_extraccion = inicializar_entrenadores_con_config(config, &entrenadores, &objetivo_actual, &cant_entrenadores, &cantidad_objetivos);

	if(primer_extraccion == 1 && segunda_extraccion == 1)
    {
        objetivo_team = cantidad_objetivos;
        pool_hilos = malloc(cant_entrenadores * sizeof(pthread_t));
        sem_entrenadores = malloc(cant_entrenadores * sizeof(sem_t));
        asignar_funcion_moverse(algoritmo_planificacion);

        inicializar_semaforos(cant_entrenadores);
        inicializar_cola_caught(cant_entrenadores);
        if(algoritmo_planificacion == SJF_S || algoritmo_planificacion == SJF_C)
        {
        	inicializar_vector_estimaciones(estimacion_inicial, cant_entrenadores);
        }

        inicializar_cola_ready();
        inicializar_cola_mensajes();
        inicializar_hilos_entrenadores(entrenadores, cant_entrenadores, pool_hilos);

        //activar_hilo_administrador_cola_caught(&hilo_cola_caught);ya no sirve
        pthread_create(&hilo_cola_caught, NULL, (void*)administrar_cola_caught, NULL); //este es el hilo que administra la cola caught
        pthread_detach(hilo_cola_caught);

        //activar_hilo_administrador_cola_ready(&hilo_cola_ready); ya no sirve
        pthread_create(&hilo_cola_ready, NULL, (void*)administrar_cola_ready, NULL);
        pthread_detach(hilo_cola_ready);

        semGETTerminados= malloc(sizeof(sem_t));
        sem_init(semGETTerminados, 0, 0);

        //activar_hilo_recepcion(&hilo_recibir_mensajes); ya no sirve
        //activamos los hilos para que TEAM pueda recibir mensajes de Broker
        pthread_create(&hilo_recibir_mensajes, NULL, (void*)recepcion_mensajes, NULL);
        pthread_detach(hilo_recibir_mensajes);
        //activamos los hilos para que TEAM pueda recibir mensajes de Gameboy
        pthread_create(&hilo_recibir_mensajes_gameboy, NULL, (void*)recepcion_Gameboy, NULL);
        pthread_detach(hilo_recibir_mensajes_gameboy);

        pthread_create(&hilo_mensajes_get, NULL, (void*)enviarMensajesGet, NULL);
        pthread_detach(hilo_mensajes_get);

		while(objetivo_team > 0)//"mientras que todavia haya pokemones que atrapar"
        {
			// Hago un wait
			sem_wait(&colaMensajes_llenos);
			// Solo si no se cumplio la condicion avanzo
            if(objetivo_team > 0)
            // version anterior: if(sem_trywait(&colaMensajes_llenos) != -1)
            {
                i=0;
                datos_primero_cola_mensajes(&mensaje);
                pthread_mutex_lock(&objetivo_actual_mutex);
                flag_finalizacion = cantidad_de_veces_en_objetivo_actual(mensaje->pokemon, objetivo_actual, cantidad_objetivos);
                pthread_mutex_unlock(&objetivo_actual_mutex);
                while(flag_finalizacion != 0 && i<mensaje->cantidad_pos && objetivo_team>0){

                	//sem_wait(&colaMensajes_llenos);
					//if(objetivo_team > 0)//todo parece no terminar si es un wait?

                    if(sem_trywait(&entrenadores_disponibles) != -1)
                    // version anterior: if(sem_trywait(&entrenadores_disponibles) != -1)
                    {
                        pos_elegido = calcular_mas_cerca_de(mensaje->posiciones[2*i], mensaje->posiciones[(2*i)+1], entrenadores, cant_entrenadores);
                        if(pos_elegido != -1)
                        {
                            printf("Entrenador %i para atrapar %s\n", pos_elegido, mensaje->pokemon);
                            sem_post(&sem_entrenadores[pos_elegido]);
                            sem_wait(&datosHilo);
                            pthread_mutex_lock(&colaReady_mutex);
                            agregar_a_ready(pos_elegido);
                            pthread_mutex_unlock(&colaReady_mutex);//
                            sem_post(&colaReady_llenos);
                        }
                        else{printf("Error pos elegido es -1 aunque hay entrenadores disponibles\n");}
                        i++;
                        flag_finalizacion-=1;
                    }
                }
                pthread_mutex_lock(&colaMensajes_mutex);
                actualizar_cola_mensajes();
                pthread_mutex_unlock(&colaMensajes_mutex);
            }
        }

        estado_team = 1;//cumplio objetivo, empiezo a solucionar deadlocks

        // Aviso para que ya no se use la cola caught
        sem_post(&colaCaught_llenos);

        join_hilos(pool_hilos, cant_entrenadores);
        join_hilo(&hilo_recibir_mensajes);
        eliminar_cola_mensajes();
        join_hilo(&hilo_cola_caught);
        eliminar_cola_caught();

        int32_t socketDessuscripcion = establecer_conexion(IP,PUERTO);//si puedo me dessuscribo
        if(socketDessuscripcion > 0)
        {
        	Dessuscripcion* estructuraDessuscripcion = malloc(sizeof(Dessuscripcion));
			estructuraDessuscripcion->pId = PID;

			estructuraDessuscripcion->numeroCola = APPEARED;
			mandar_mensaje(estructuraDessuscripcion, DESSUSCRIPCION, socketDessuscripcion);
			sleep(1);

			estructuraDessuscripcion->numeroCola = LOCALIZED;
			mandar_mensaje(estructuraDessuscripcion, DESSUSCRIPCION, socketDessuscripcion);
			sleep(1);

			estructuraDessuscripcion->numeroCola = CAUGHT;
			mandar_mensaje(estructuraDessuscripcion, DESSUSCRIPCION, socketDessuscripcion);
			sleep(1);

			close(socketDessuscripcion);
        }

        cerrar_conexion(socketAppeared);
        cerrar_conexion(socketLocalized);
        cerrar_conexion(socketCaught);
        cerrar_conexion(miSocket);

        //informar_estado_actual(entrenadores, cant_entrenadores);

        // TODO | Este while lo puso NACHO para q probar lo de deadlock
        // IMPORTANTE: CON ESTO EL PROGRAMA PASA DIRECTAMENTE A DETECCION Y RECUPERACION
//		int k = 0;
//		while( k < cant_entrenadores ){
//			entrenadores[k].estado = BLOCKED;
//			k++;
//		}

        temp_cant = cant_en_espera(entrenadores, cant_entrenadores);
        if(temp_cant > 1){
            //printf("\nInicio proceso deadlock\n");
            printf("en_espera: %i\n", temp_cant);

            log_info(logger, "DEADLOCK | Iniciando deteccion y recuperacion de deadlock");
            deteccion_y_recuperacion(entrenadores, cant_entrenadores, temp_cant, pool_hilos);
        }
        else{
        	log_info(logger, "DEADLOCK | No existe deadlock en el sistema");
        }

        estado_team = 2;//cumplio deadlocks

        // Aviso para que se cierre la funcion
        sem_post(&colaReady_llenos);

        join_hilo(&hilo_cola_ready);
        se_cumplio_objetivo(entrenadores, cant_entrenadores);

        // LOGUEAR RESULTADOS FINALES

        log_info(logger, "La cantidad de ciclos totales de CPU es: %i", ciclosCPUTotales);

        int numEntrenador = 0;

		while(numEntrenador < cant_entrenadores){
			log_info(logger, "El entrenador numero [%i] realizo [%i] ciclos de CPU", entrenadores[numEntrenador].numeroEntrenador, entrenadores[numEntrenador].cantCiclosCPU);
			numEntrenador++;
		}

		log_info(logger, "La cantidad de cambios de contexto totales es: %i", cambiosContextoTotales);

        eliminar_cola_ready();
        free(pool_hilos);
        destruir_semaforos(cant_entrenadores);
        liberarConfig();
    }
    else
    {
    	printf("Archivo .config con errores.\n");
    }

    printf("Fin Team\n");
    return 0;
}

void enviarMensajesGet()
{
	puts("Estableciendo Conexion para mandar mensajes GET...");

	// Espera a que se termine de suscribir a todas las colas
	int i = 0;
	// objetivo_actual es una lista de strings con cada pokemon que team necesita
	while (objetivo_actual[i] != NULL)
	{
		int32_t socketMandarGets = establecer_conexion(IP,PUERTO);

		if(socketMandarGets > 0)
		{
			printf("Mandando mensaje GET para Pokemon: %s\n", objetivo_actual[i]);
			enviarMensajeGet(objetivo_actual[i], socketMandarGets);
			sleep(1);
			i++;
			close(socketMandarGets);
		}
		else
		{
			sem_wait(semLog);
			log_info(logger, "Conexión a Broker fallida, Se inicia operación por default: Asumir que no existen los Pokemones objetivo.");
			sem_post(semLog);
			break;
		}

	}

	sem_post(semGETTerminados);
}

void enviarMensajeGet(char* pokemon, int32_t socketMandarGets)
{
	Get* structGet = malloc(sizeof(Get));
	structGet->largoNombre = strlen(pokemon);
	structGet->nombrePokemon = malloc(strlen(pokemon) + 1);
	structGet->nombrePokemon = pokemon;
	structGet->ID = 0;
	structGet->corrID = -2;

	mandar_mensaje(structGet, GET, socketMandarGets);

	free(structGet);
}

void printearEntrenadores(d_entrenador* entrenadores, int cant_entrenadores){
	int k = 0;
	while( k < cant_entrenadores ){
		printf("Entrenador numero: %i\n", k);
		printf("Posicion X: %i", entrenadores[k].posicion[0]);
		printf(" Posicion Y: %i\n", entrenadores[k].posicion[1]);
		printf("Estado Actual: %i\n", entrenadores[k].estado);

		printf("Pokemones actuales:\n");
		int j = 0;
		while( entrenadores[k].pokemones_actuales[j] != NULL){
			printf(" %s,", entrenadores[k].pokemones_actuales[j]);
			j++;
		}

		printf("\n");

		j = 0;
		printf("Pokemones objetivo:\n");
		while( entrenadores[k].objetivo[j] != NULL){

			printf(" %s,", entrenadores[k].objetivo[j]);
			j++;
		}
		k++;

		printf("\n");
	}
}

///////////////////-SEMAFOROS-/////////////////////
void inicializar_semaforos(int cant_entrenadores){
    inicializar_sem_entrenadores(cant_entrenadores, 0);
    init_sem(&entrenadores_disponibles, 0);
    init_sem(&colaMensajes_llenos, 0);
    init_sem(&colaReady_llenos, 0);
    init_sem(&colaCaught_llenos, 0);
    init_sem(&datosHilo, 0);
    init_sem(&enExec, 1);
    init_sem(&extraccion_mensaje_caught, 0);

    init_mutex(&objetivo_actual_mutex);
    init_mutex(&colaMensajes_mutex);
    init_mutex(&colaReady_mutex);
    init_mutex(&colaCaught_mutex);
}

void destruir_semaforos(int cantidad){
    destruir_sem_entrenadores(cantidad);
    free(sem_entrenadores);
    sem_destroy(&colaMensajes_llenos);
    sem_destroy(&colaReady_llenos);
    sem_destroy(&colaCaught_llenos);
    sem_destroy(&datosHilo);
    sem_destroy(&enExec);
    sem_destroy(&entrenadores_disponibles);
    sem_destroy(&extraccion_mensaje_caught);

    destroy_mutex(&objetivo_actual_mutex);
    destroy_mutex(&colaMensajes_mutex);
    destroy_mutex(&colaReady_mutex);
    destroy_mutex(&colaCaught_mutex);
}

void inicializar_sem_entrenadores(int cant_entrenadores, int valor){
    int i;
    for(i=0;i<cant_entrenadores;i++){
        init_sem(&sem_entrenadores[i], valor);
    }
}

void destruir_sem_entrenadores(int cant_entrenadores){
    int i;
    for(i=0;i<cant_entrenadores;i++){
        sem_destroy(&sem_entrenadores[i]);
    }
}

///////////////////-ENTRENADORES-/////////////////////
void inicializar_hilos_entrenadores(d_entrenador* entrenadores, int cant_entrenadores, pthread_t* hilos){
    int i;
    parametros_entrenador mensaje_entrenador;
    for(i=0;i<cant_entrenadores;i++){
        mensaje_entrenador.entrenador = &entrenadores[i];
        mensaje_entrenador.pos = i;
        pthread_create(&(hilos[i]), NULL, (void*)ciclo_vida_entrenador, &mensaje_entrenador);
        sem_wait(&datosHilo);
    }
}

void* ciclo_vida_entrenador(parametros_entrenador* parametros){
    d_entrenador* entrenador = parametros->entrenador;
    int posicion = parametros->pos;
    sem_post(&datosHilo);

    Appeared mensaje;
    int eliminacion, objetivo_individual;
    objetivo_individual = calcular_objetivo_propio(*entrenador);

    while(entrenador->estado != EXIT && entrenador->estado_block != EN_ESPERA)
    {
        sem_post(&entrenadores_disponibles);
        sem_wait(&sem_entrenadores[posicion]);
        pthread_mutex_lock(&colaMensajes_mutex);
        primero_en_cola_mensajes(&mensaje);
        pthread_mutex_unlock(&colaMensajes_mutex);
        sem_post(&datosHilo);
        cambiar_estado_a(entrenador, READY);

        sem_wait(&sem_entrenadores[posicion]);
        cambiar_estado_a(entrenador, EXEC);
        moverse_a(entrenador, mensaje.posPokemon.x, mensaje.posPokemon.y, posicion);
        armar_enviar_catch(mensaje.nombrePokemon, mensaje.posPokemon.x, mensaje.posPokemon.y, posicion, entrenador);
        bloquear(entrenador, ESPERA_CAUGHT);
        sem_post(&enExec);
        if(recibir_caught(posicion) == 1){
            pthread_mutex_lock(&objetivo_actual_mutex);
            eliminacion = eliminar_de_objetivo_actual(mensaje.nombrePokemon, &objetivo_actual, cantidad_objetivos);
            pthread_mutex_unlock(&objetivo_actual_mutex);
            if(eliminacion == 1){
                agregar_captura(entrenador, mensaje.nombrePokemon);
                objetivo_individual--;
                if(objetivo_individual == 0){//esta en el limite
                    //printf("esta en el limite\n");
                    if(esta_terminado(*entrenador) == 1){
                        printf("Termino entrenador %i\n", posicion);
                        cambiar_estado_a(entrenador, EXIT);
                    }
                    else{//printf("Pero no termino\n");
                        bloquear(entrenador, EN_ESPERA);
                    }
                }
                else{bloquear(entrenador, ACTIVO);}
                objetivo_team-=1;
                if (objetivo_team <= 0){
                	// Si ya termine con los objetivos activo el semaforo para avanzar
                	sem_post(&colaMensajes_llenos);
                }
                printf("Eliminado %s. La cantidad de objetivos es: %i\n", mensaje.nombrePokemon, objetivo_team);
            }
        }
        else{bloquear(entrenador, ACTIVO);}
    }
    return NULL;
}

///////////////////-RECEPCION-/////////////////////
void recepcion_Gameboy(void* argumento_de_adorno)
{
	miSocket = reservarSocket(PUERTOTEAM);

	while(1)
	{
		esperar_conexiones_Gameboy(miSocket);
	}
}

void esperar_conexiones_Gameboy(int32_t miSocket)
{
	struct sockaddr_in dir_cliente;
	socklen_t tam_direccion = sizeof(struct sockaddr_in);

	//espera una conexion
	int32_t socket_cliente = accept(miSocket, (void*) &dir_cliente, &tam_direccion);

	datosAppearedGameboy->socket = socket_cliente;

	if((socket_cliente >= 1) && (estado_team == 0))
	{
		escuchoMensajesGameboy(datosAppearedGameboy);
	}
}

void escuchoMensajesGameboy(datosHiloColas* parametros)
{
    int recibidosCodOP = 0;
    int32_t recibidosSize = 0;
	int32_t sizeAAllocar;
    codigo_operacion cod_op;

	//recibo codigo de op
	recibidosCodOP = recv(parametros->socket, &cod_op, sizeof(cod_op), MSG_WAITALL);
	bytesRecibidos(recibidosCodOP);

	//si se cayo la conexion, intento reconectar
	if(recibidosCodOP < 1)
	{
		cod_op = 0;
	}
	//recibo tamaño de lo que sigue
	recibidosSize = recv(parametros->socket, &sizeAAllocar, sizeof(int32_t), MSG_WAITALL);
	bytesRecibidos(recibidosSize);

	//si se cayo la conexion, intento reconectar
	if(recibidosSize < 1)
	{
		sizeAAllocar = 0;
	}
	printf("Tamaño de lo que sigue en el buffer: %u.\n", sizeAAllocar);

	//mando lo que consegui para que lo procesen
	procesar_mensaje(cod_op, sizeAAllocar, parametros->socket);

}

void recepcion_mensajes(void* argumento_de_adorno)
{
	semConexionBroker= malloc(sizeof(sem_t));
	sem_init(semConexionBroker, 0, 1);

	sem_wait(semGETTerminados);

	socketAppeared = intento_reconexion(APPEARED, PID, 1);//intento conectarme a Broker
	socketLocalized = intento_reconexion(LOCALIZED, PID, 1);//intento conectarme a Broker
	socketCaught = intento_reconexion(CAUGHT, PID, 1);//intento conectarme a Broker

	sleep(1);


	pthread_t hiloApp;
	pthread_t hiloLocal;
	pthread_t hiloCaug;

	datosAppeared->socket = socketAppeared;
	datosLocalized->socket = socketLocalized;
	datosCaught->socket = socketCaught;

	pthread_create(&hiloApp, NULL, (void*)escuchoMensajesBroker, datosAppeared);
	pthread_create(&hiloLocal, NULL, (void*)escuchoMensajesBroker, datosLocalized);
	pthread_create(&hiloCaug, NULL, (void*)escuchoMensajesBroker, datosCaught);

	pthread_detach(hiloApp);
	pthread_detach(hiloLocal);
	pthread_detach(hiloCaug);
}

void escuchoMensajesBroker(datosHiloColas* parametros)
{
    int recibidosCodOP = 0;
    int32_t recibidosSize = 0;
	int32_t sizeAAllocar;
    codigo_operacion cod_op;

	while(estado_team == 0)//mientras no tenga todos los pokemones que necesita
	{
		//recibo codigo de op
		recibidosCodOP = recv(parametros->socket, &cod_op, sizeof(cod_op), MSG_WAITALL);
		bytesRecibidos(recibidosCodOP);

		//si se cayo la conexion, intento reconectar
		if(recibidosCodOP < 1)
		{
			printf("Se cayo la conexion con Broker.\n");
			sem_wait(semConexionBroker);
			parametros->socket = intento_reconexion(parametros->cola, PID, 0);
			sem_post(semConexionBroker);
			if(parametros->socket > 1)
			{
				recibidosCodOP = recv(parametros->socket, &cod_op, sizeof(codigo_operacion), MSG_WAITALL);
				bytesRecibidos(recibidosCodOP);
			}
		}
		//recibo tamaño de lo que sigue
		recibidosSize = recv(parametros->socket, &sizeAAllocar, sizeof(int32_t), MSG_WAITALL);
		bytesRecibidos(recibidosSize);
		//si se cayo la conexion, intento reconectar
		if(recibidosSize < 1)
		{
			printf("Se cayo la conexion con Broker.\n");
			sem_wait(semConexionBroker);
			parametros->socket = intento_reconexion(parametros->cola, PID, 0);
			sem_post(semConexionBroker);
			if(parametros->socket > 1)
			{
				recibidosSize = recv(parametros->socket, &sizeAAllocar, sizeof(int32_t), MSG_WAITALL);
				bytesRecibidos(recibidosSize);
			}
		}
		printf("Tamaño de lo que sigue en el buffer: %u.\n", sizeAAllocar);

		//mando lo que consegui para que lo procesen
		procesar_mensaje(cod_op, sizeAAllocar, parametros->socket);
	}
}

int32_t intento_reconexion(codigo_operacion codigo, uint32_t PID, uint32_t flagPrimerIntento)
{
	int32_t elSocket = -1;
	Suscripcion* estructuraSuscripcion = malloc(sizeof(Suscripcion));
	estructuraSuscripcion->pId = PID;

	while(elSocket < 1 && estado_team !=1)
	{
		sem_wait(semLog);
		log_info(logger, "Intentando Conexión a Broker...");
		sem_post(semLog);

		if(flagPrimerIntento == 0)//la conexion a Broker se cayo, por lo que se espera lo que dice la config
		{
			sleep(tiempo_reconexion);
		}
		else//es el primer intento de conexion a Broker, no necesita esperar lo que dice la config
		{
			sleep(1);//sleep 1 para darle un respiro a Broker
		}

		elSocket = establecer_conexion(IP, PUERTO);//intento conectarme a Broker
		if(elSocket >= 1)//me suscribo a la cola
		{
			sem_wait(semLog);
			log_info(logger, "Conexión con Broker establecida, apostemos cuánto dura...");
			sem_post(semLog);
			estructuraSuscripcion->numeroCola = codigo;
			mandar_mensaje(estructuraSuscripcion, SUSCRIPCION, elSocket);
		}
		else
		{
			sem_wait(semLog);
			log_error(logger,"Intento de conexión fallido, intentando de nuevo en %u segundo/s.", tiempo_reconexion);
			sem_post(semLog);
		}
	}
	free(estructuraSuscripcion);
	return elSocket;
}

void procesar_mensaje(codigo_operacion cod_op, int32_t sizeAAllocar, int32_t socket)
{
	Appeared* recibidoAppeared;
	Localized* recibidoLocalized;
	Caught* recibidoCaught;
	confirmacionMensaje* mensajeConfirm;
	mensaje_server* mensaje_rec = malloc(sizeof(mensaje_server));
	mensaje_caught* elResultado = malloc(sizeof(mensaje_caught));
	int32_t socketAck;
	int32_t iterador = 0;
	int32_t filtro;

    switch(cod_op)
    {
        case APPEARED:
        	recibidoAppeared = malloc(sizeAAllocar);
        	recibir_mensaje(recibidoAppeared, cod_op, socket);

        	//logueamos la llegada de un mensaje nuevo
    		sem_wait(semLog);
    		log_info(logger, "LLego un mensaje Appeared, datos:\nNombre:%s\nPos X: %u\nPos Y: %u\nID: %i\nID Correlativa: %i.", recibidoAppeared->nombrePokemon, recibidoAppeared->posPokemon.x, recibidoAppeared->posPokemon.y, recibidoAppeared->ID, recibidoAppeared->corrID);
    		sem_post(semLog);

			//mandamos confirmacion para no volver a recibir este mensaje
			mensajeConfirm = malloc(sizeof(confirmacionMensaje));
			mensajeConfirm->id_mensaje = recibidoAppeared->ID;
			mensajeConfirm->colaMensajes = cod_op;
			mensajeConfirm->pId = PID;
			socketAck = establecer_conexion(IP, PUERTO);
			mandar_mensaje(mensajeConfirm, CONFIRMACION, socketAck);
			cerrar_conexion(socketAck);
			free(mensajeConfirm);
			sleep(1);

			//le asigno todos los datos a la estructura que maneja team
			mensaje_rec->pokemon = malloc(recibidoAppeared->largoNombre+1);
			mensaje_rec->pokemon = recibidoAppeared->nombrePokemon;
			mensaje_rec->cantidad_pos = 1; //1 por ser Appeared
			mensaje_rec->posiciones = malloc(mensaje_rec->cantidad_pos * sizeof(int));

			while(iterador < mensaje_rec->cantidad_pos)
			{
				mensaje_rec->posiciones[iterador] = recibidoAppeared->posPokemon.x;
				iterador++;
				mensaje_rec->posiciones[iterador] = recibidoAppeared->posPokemon.y;
				iterador++;
			}

			//ahora que lo traducimos a "idioma team" ya puede continuar el funcionamiento de team
			pthread_mutex_lock(&objetivo_actual_mutex);//reveer este mutex todo lo habra revisto? hay que comprobarlo
			filtro = filtrar_mensaje(mensaje_rec, objetivo_actual, cantidad_objetivos);//me fijo si este mensaje me sirve
			pthread_mutex_unlock(&objetivo_actual_mutex);
			if(filtro == 1)
			{
				pthread_mutex_lock(&colaMensajes_mutex);
				agregar_a_cola_mensajes(mensaje_rec);//el mensaje me sirve y lo agrego a la cola de los mensajes que si sirven
				pthread_mutex_unlock(&colaMensajes_mutex);
				sem_post(&colaMensajes_llenos);
			}
			else //el mensaje no sirve y lo vuelo a la mierda
			{
				printf("APPEARED | El pokemon %s que recibi no me sirve\n", mensaje_rec->pokemon);
			    free(mensaje_rec->pokemon);
			    free(mensaje_rec->posiciones);
			    free(mensaje_rec);
			}

			free(recibidoAppeared);
        	break;

        case LOCALIZED:
        	recibidoLocalized = malloc(sizeAAllocar);
        	recibir_mensaje(recibidoLocalized, cod_op, socket);

        	//logueamos la llegada de un mensaje nuevo
			sem_wait(semLog);
			log_info(logger, "LLego un mensaje Localized, datos:\nNombre:%s\nCantidad Posiciones: %u\nID: %i\nID Correlativa: %i.", recibidoLocalized->nombrePokemon, recibidoLocalized->cantPosciciones, recibidoLocalized->ID, recibidoLocalized->corrID);
			sem_post(semLog);

			//mandamos confirmacion para no volver a recibir este mensaje
			mensajeConfirm = malloc(sizeof(confirmacionMensaje));
			mensajeConfirm->id_mensaje = recibidoLocalized->ID;
			mensajeConfirm->colaMensajes = cod_op;
			mensajeConfirm->pId = PID;
			socketAck = establecer_conexion(IP, PUERTO);
			mandar_mensaje(mensajeConfirm, CONFIRMACION, socketAck);
			cerrar_conexion(socketAck);
			free(mensajeConfirm);
			sleep(1);

			if(recibidoLocalized->cantPosciciones != 0)//si la cantidad de posiciones es 0, ignorarlo
			{
				//le asigno todos los datos a la estructura que maneja team
				mensaje_rec->pokemon = malloc(recibidoLocalized->largoNombre+1);
				mensaje_rec->pokemon = recibidoLocalized->nombrePokemon;
				mensaje_rec->cantidad_pos = recibidoLocalized->cantPosciciones;
				mensaje_rec->posiciones = malloc(mensaje_rec->cantidad_pos * sizeof(int));

				while(iterador < ((mensaje_rec->cantidad_pos*2)))
				{
					mensaje_rec->posiciones[iterador] = recibidoLocalized->coords[iterador];
					iterador++;
				}

				//ahora que lo traducimos a "idioma team" ya puede continuar el funcionamiento de team
				pthread_mutex_lock(&objetivo_actual_mutex);//reveer este mutex todo lo habra revisto? hay que comprobarlo
				filtro = filtrar_mensaje(mensaje_rec, objetivo_actual, cantidad_objetivos);//me fijo si este mensaje me sirve
				pthread_mutex_unlock(&objetivo_actual_mutex);
				if(filtro == 1)
				{
					pthread_mutex_lock(&colaMensajes_mutex);
					agregar_a_cola_mensajes(mensaje_rec);//el mensaje me sirve y lo agrego a la cola de los mensajes que si sirven
					pthread_mutex_unlock(&colaMensajes_mutex);
					sem_post(&colaMensajes_llenos);
				}
				else //el mensaje no sirve y lo vuelo a la mierda
				{
					free(mensaje_rec->pokemon);
					free(mensaje_rec->posiciones);
					free(mensaje_rec);
				}
			}

			free(recibidoLocalized);
        	break;

        case CAUGHT:
        	recibidoCaught = malloc(sizeAAllocar);
        	recibir_mensaje(recibidoCaught, cod_op, socket);

        	//logueamos la llegada de un mensaje nuevo
			sem_wait(semLog);
			log_info(logger, "LLego un mensaje Caught, datos:\nNombre:%s\nIntento de atrapar: %i\nID: %i\nID Correlativa: %i.", recibidoCaught->nombrePokemon, recibidoCaught->pudoAtrapar, recibidoCaught->ID, recibidoCaught->corrID);
			sem_post(semLog);

			//mandamos confirmacion para no volver a recibir este mensaje
			mensajeConfirm = malloc(sizeof(confirmacionMensaje));
			mensajeConfirm->id_mensaje = recibidoCaught->ID;
			mensajeConfirm->colaMensajes = cod_op;
			mensajeConfirm->pId = PID;
			socketAck = establecer_conexion(IP, PUERTO);
			mandar_mensaje(mensajeConfirm, CONFIRMACION, socketAck);
			cerrar_conexion(socketAck);
			free(mensajeConfirm);
			sleep(1);

			//le asigno todos los datos a la estructura que maneja team
			elResultado->resultado = recibidoCaught->pudoAtrapar;
			elResultado->num_envio = recibidoCaught->corrID;

			//ahora que lo traducimos a "idioma team" ya puede continuar el funcionamiento de team
			pthread_mutex_lock(&colaCaught_mutex);
			agregar_a_cola_caught(elResultado);//toda respuesta la guarda en la cola caught o "cola de resultados"
			pthread_mutex_unlock(&colaCaught_mutex);
			sem_post(&colaCaught_llenos);

			free(recibidoCaught);
        	break;

        default:
            free(mensaje_rec);
            free(elResultado);
        	break;
    }
}

///////////////////-READY-/////////////////////
void administrar_cola_ready(void* parametros){
    int posicion;

//  sem_wait(&colaMensajes_llenos);
//	// Solo si no se cumplio la condicion avanzo
//	if(objetivo_team > 0)

    while(estado_team < 2){
    	sem_wait(&colaReady_llenos);
        if(estado_team < 2){
        // version anterior: if(sem_trywait(&colaReady_llenos) != -1){
            sem_wait(&enExec);
            pthread_mutex_lock(&colaReady_mutex);
            posicion = pos_primero_de_ready();
            pthread_mutex_unlock(&colaReady_mutex);
            sem_post(&sem_entrenadores[posicion]);
        }
    }
}

int comparar_estimaciones(int estimacion_entrenador_en_exec){
    int temp_estimacion, respuesta;
    respuesta = 1;
    pthread_mutex_lock(&colaReady_mutex);
    temp_estimacion = estimacion_primero_de_ready();
    setear_nuevo_primero(0);
    pthread_mutex_unlock(&colaReady_mutex);
    if(temp_estimacion < estimacion_entrenador_en_exec){
        respuesta = -1;
    }
    return respuesta;
}

void libero_exec_me_agrego_a_ready_y_espero(d_entrenador* entrenador, int pos){
    sem_post(&enExec);
    me_agrego_a_ready_y_espero(entrenador, pos);
}

void me_agrego_a_ready_y_espero(d_entrenador* entrenador, int pos){
    pthread_mutex_lock(&colaReady_mutex);
    cambiar_estado_a(entrenador, READY);
    agregar_a_ready(pos);
    pthread_mutex_unlock(&colaReady_mutex);
    sem_post(&colaReady_llenos);

    sem_wait(&sem_entrenadores[pos]); // TODO | Semaforo que quite
    cambiar_estado_a(entrenador, EXEC);
}

///////////////////-CAUGHT-/////////////////////
void administrar_cola_caught(void* parametros){
    int posicion;
    while(estado_team == 0)
    {
    	sem_wait(&colaCaught_llenos);
        if(estado_team == 0)
        // Version anterior: if(sem_trywait(&colaCaught_llenos) != -1)
        {
            posicion = posicion_primero_cola_caught();//todo arreglar
            if(posicion != -1)
            {
            	sem_post(&sem_entrenadores[posicion]);
				sem_wait(&extraccion_mensaje_caught);
            }
        }
    }
}

int recibir_caught(int posicion){
    int resultado;
    sem_wait(&sem_entrenadores[posicion]);
    pthread_mutex_lock(&colaCaught_mutex);
    resultado = resultado_primero_cola_caught();
    pthread_mutex_unlock(&colaCaught_mutex);
    sem_post(&extraccion_mensaje_caught);
    return resultado;
}

///////////////////-DEADLOCK-/////////////////////
void* ciclo_deadlock(parametros_deadlock* parametros){
    int tamano = parametros->tamano_respuesta;
    elemento_respuesta* respuesta = malloc(tamano * sizeof(elemento_respuesta));
    copiar_respuesta_en(parametros->respuesta, respuesta, tamano);
    sem_post(&datosHilo);

    d_entrenador* entrenadores = parametros->entrenadores;
    deadlock_entrenador* temp_entrenadores = parametros->temp_entrenadores;
    int i, ubicacion_primer_entrenador;
    ubicacion_primer_entrenador = temp_entrenadores[respuesta[0].posicion].ubicacion;
    i=1;
    while(i<tamano){
        borrar_de_entrenador_deadlock(respuesta[i-1].pokemon, &temp_entrenadores[respuesta[i-1].posicion], &temp_entrenadores[respuesta[i].posicion]);
        realizar_intercambio_entre(&entrenadores[ubicacion_primer_entrenador], respuesta[i-1].pokemon, &entrenadores[temp_entrenadores[respuesta[i].posicion].ubicacion], respuesta[i].pokemon, ubicacion_primer_entrenador);
        i++;
    }
    borrar_de_entrenador_deadlock(respuesta[i-1].pokemon, &temp_entrenadores[respuesta[i-1].posicion], &temp_entrenadores[respuesta[0].posicion]);
    free(respuesta);

    return NULL;
}

void realizar_intercambio_entre(d_entrenador* un_entrenador, char* un_pokemon, d_entrenador* otro_entrenador, char* otro_pokemon, int posicion_un_entrenador){
    cambiar_estado_a(un_entrenador, READY);
    me_agrego_a_ready_y_espero(un_entrenador, posicion_un_entrenador);
    moverse_a(un_entrenador, otro_entrenador->posicion[0], otro_entrenador->posicion[1], posicion_un_entrenador);
    intercambiar(un_entrenador, un_pokemon, otro_entrenador, otro_pokemon);
    sem_post(&enExec);
    cambiar_estado_a(un_entrenador, BLOCKED);
}

void tratar_circulos(deadlock_entrenador* entrenadores, int cant_entrenadores, elemento_respuesta* respuesta, int tamano_respuesta, parametros_deadlock* mensaje_deadlock, pthread_t* hilos){
    int num_circulo = 0;
    do{
        printf("Respuesta Circulo %i:\n", num_circulo + 1);
        mostrar_respuesta(respuesta, tamano_respuesta);
        agregar_respuesta_tamano(mensaje_deadlock, respuesta, tamano_respuesta);
        activar_hilo_circulo_deadlock(mensaje_deadlock, &(hilos[num_circulo]));
        //sem_wait(&datosHilo); todo ojo con esto
        actualizar_respuesta(respuesta, tamano_respuesta);
        num_circulo++;
        tamano_respuesta = detectar_deadlock(entrenadores, cant_entrenadores, respuesta);
    }while(tamano_respuesta > 0);
    //join_hilos(hilos, num_circulo); todo ojo con esto
    log_info(logger, "DEADLOCK | Cantidad de ciclos totales resueltos: %i", num_circulo);
}

///////////////////-MOVERSE-/////////////////////
void asignar_funcion_moverse(int algoritmo_planificacion){
    switch(algoritmo_planificacion){
        case FIFO :
            moverse_a = &moverse_fifo;
            break;
        case RR :
            moverse_a = &moverse_rr;
            break;
        case SJF_S :
            moverse_a = &moverse_sjf_sin_d;
            break;
        case SJF_C :
            moverse_a = &moverse_sjf_con_d;
            break;
        default : printf("error codigo de algoritmo planificacion");
    }
}

void loguearInicioMovimiento(d_entrenador* entrenador, int posXObjetivo, int posYObjetivo){
	log_info(logger, "MOVIMIENTO | Entrenador numero [%i] comienza a moverse a la posicion %i,%i", entrenador->numeroEntrenador, posXObjetivo, posYObjetivo);
}

void loguearFinMovimiento(d_entrenador* entrenador, int posXObjetivo, int posYObjetivo){
	log_info(logger, "MOVIMIENTO | Entrenador numero [%i] terminó de moverse a la posicion %i,%i", entrenador->numeroEntrenador, posXObjetivo, posYObjetivo);
}

void moverse_fifo(d_entrenador* entrenador, int pos_x, int pos_y, int entrenador_pos){
	loguearInicioMovimiento(entrenador, pos_x, pos_y);
	llegar_por_eje(entrenador, pos_x, 0); // EJE 0 = X, EJE 1 = Y
    llegar_por_eje(entrenador, pos_y, 1);
    loguearFinMovimiento(entrenador, pos_x, pos_y);
}

void moverse_rr(d_entrenador* entrenador, int pos_x, int pos_y, int entrenador_pos){
    int resto = quantum;
    loguearInicioMovimiento(entrenador, pos_x, pos_y);
    llegar_por_eje_con_quantum(entrenador, pos_x, 0, quantum, &resto, entrenador_pos);
    llegar_por_eje_con_quantum(entrenador, pos_y, 1, quantum, &resto, entrenador_pos);
    loguearFinMovimiento(entrenador, pos_x, pos_y);
}

void moverse_sjf_sin_d(d_entrenador* entrenador, int pos_x, int pos_y, int entrenador_pos){
    int rafagas = distancia_a(entrenador->posicion[0], entrenador->posicion[1], pos_x, pos_y);//printf("entrenador %i rafagas %i\n", entrenador_pos, rafagas);
    loguearInicioMovimiento(entrenador, pos_x, pos_y);
    moverse_fifo(entrenador, pos_x, pos_y, entrenador_pos);
    notificar_rafagas_reales(entrenador_pos, rafagas);
    loguearFinMovimiento(entrenador, pos_x, pos_y);
}

void moverse_sjf_con_d(d_entrenador* entrenador, int pos_x, int pos_y, int entrenador_pos){
	loguearInicioMovimiento(entrenador, pos_x, pos_y);
    llegar_por_eje_con_seguimiento_de_rafaga(entrenador, pos_x, 0, entrenador_pos);
    llegar_por_eje_con_seguimiento_de_rafaga(entrenador, pos_y, 1, entrenador_pos);
    loguearFinMovimiento(entrenador, pos_x, pos_y);
}
