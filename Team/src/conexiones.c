#include "Team.h"
/*
char* puerto_team = "5002";
char* ip_team = "127.0.0.1";
int (*emitir_un_mensaje_catch)(mensaje_broker*);
pthread_t* pool_hilos_suscripciones;
pthread_t hilo_servidor, hilo_emisor, hilo_reconexion;
pthread_mutex_t listaIds_mutex, colaCatch_mutex, listaGet_mutex;
sem_t conexion_broker, colaCatch_llenos;
int ip, puerto, tiempo_reconexion, flag_conexiones, flag_conexion_broker;

///////////////////-CONEXIONES-////////////////////
void iniciar_conexiones(int dato_ip, int dato_puerto, int dato_tiempo_reconexion, elemento_objetivo* objetivo_global, int tamano_objetivo){
    comunicar_datos_socket(dato_ip, dato_puerto, dato_tiempo_reconexion);
    inicializar_flag_conexiones();
    inicializar_semaforos_conexiones();
    iniciar_conexion_con_gameboy();
    iniciar_conexion_con_broker(objetivo_global, tamano_objetivo);
}

void comunicar_datos_socket(int dato_ip, int dato_puerto, int dato_tiempo_reconexion){
    ip = dato_ip;
    puerto = dato_puerto;
    tiempo_reconexion = dato_tiempo_reconexion;
}

void inicializar_flag_conexiones(){
    activar_flag_conexiones();
}

void inicializar_flag_conexion_broker(){
    activar_flag_conexion_broker();
}

void activar_flag_conexiones(){
    flag_conexiones = 1;
}
void desactivar_flag_conexiones(){
    flag_conexiones = 0;
}
void activar_flag_conexion_broker(){
    flag_conexion_broker = 1;
}
void desactivar_flag_conexion_broker(){
    flag_conexion_broker = 0;
}
int flag_conexion_broker_esta_activado(){
    return flag_conexion_broker;
}
int flag_conexiones_esta_activado(){
    return flag_conexiones;
}

void inicializar_semaforos_conexiones(){
    init_mutex(&listaIds_mutex);
    init_mutex(&colaCatch_mutex);
    init_mutex(&listaGet_mutex);
    init_sem(&conexion_broker, 0);
}

void iniciar_conexion_con_gameboy(){
    activar_servidor_team();
}

void iniciar_conexion_con_broker(elemento_objetivo* objetivo_global, int tamano_objetivo){
    inicializar_comportamiento_conectado_catch();
    inicializar_lista_ids();
    inicializar_cola_mensajes_broker();
    preparar_mensajes_suscripciones();
    preparar_mensajes_get(objetivo_global, tamano_objetivo);
    activar_hilo_emisor();
}

void inicializar_comportamiento_conectado_catch(){
    asignar_comportamiento_conectado_broker();
}

//void asignar_comportamiento_conectado_broker(){
//    emitir_un_mensaje_catch = &comportamiento_catch_broker_conectado;
//}
//
//void asignar_comportamiento_default(){
//    emitir_un_mensaje_catch = &comportamiento_catch_default;
//}
//
//void activar_hilo_emisor(){
//    activar_hilo(&hilo_emisor, emision_mensajes, NULL);
//}

void cerrar_conexiones(){
    desactivar_flag_conexiones();
    cerrar_conexiones_con_gameboy();
    cerrar_conexiones_con_broker();
    destruir_semaforos_conexiones();
}

void cerrar_conexiones_con_gameboy(){
    apagar_servidor_team();
}

void cerrar_conexiones_con_broker(){
    //dessuscribirse_de_colas();
    eliminar_lista_ids();
    terminar_hilo_emisor();
    eliminar_cola_mensajes_broker();
}

void terminar_hilo_emisor(){
    join_hilo(&hilo_emisor);
}

void destruir_semaforos_conexiones(){
    sem_destroy(&conexion_broker);
    destroy_mutex(&listaIds_mutex);
    destroy_mutex(&colaCatch_mutex);
    destroy_mutex(&listaGet_mutex);
}

int se_pudo_conectar_socket(int socket){
    int respuesta = 1;
    if(socket == -1){respuesta = 0;}
    return respuesta;
}

///////////////////-SERVIDOR-////////////////////
//void activar_servidor_team(){
//    activar_hilo(&hilo_servidor, iniciar_servidor_team, NULL);
//}

void* iniciar_servidor_team(void* parametros){
    int socket_servidor_team = activar_hilo_escucha(ip_team, puerto_team);//igual_que iniciar_server en game-watch-server pero sacando la partedel while(1)
    team_server(socket_servidor_team);
    return NULL;
}

void team_server(int socket_servidor){
    while(flag_conexiones == 1){
        esperar_cliente(socket_servidor);//igual que en game watch server pero se cambia el process request por procesar_mensaje(APPEARED, socket_servidor)
    }
}

void apagar_servidor_team(){
    join_hilo(&hilo_servidor);
}

///////////////////-SUSCRIPCIONES-////////////////////
void preparar_mensajes_suscripciones(){
    preparar_suscripcion_a_cola(CAUGHT);
    preparar_suscripcion_a_cola(APPEARED);
    preparar_suscripcion_a_cola(LOCALIZED);
}

//void preparar_suscripcion_a_cola(int cola){
//    Suscripcion mensaje = armar_mensaje_suscripcion(cola);
//    agregar_mensaje_suscripcion_a_cola_mensajes_broker(mensaje);
//}

//Suscripcion armar_mensaje_suscripcion(int cola){
//    Suscripcion mensaje;
//    mensaje.numeroCola = cola;
//    return mensaje;
//}

int suscribirse_a_colas(){
    int cantidad_suscripciones, resultado_suscripciones;
    cantidad_suscripciones = cantidad_de_suscripciones_mensajes_broker();
    pool_hilos_suscripciones = malloc(cantidad_suscripciones * sizeof(pthread_t));
    resultado_suscripciones = suscribirse_segun_lista_suscripciones(pool_hilos_suscripciones, cantidad_suscripciones);
    return resultado_suscripciones;
}

int suscribirse_a_cola_broker(int pos, void* mensaje){
    activar_hilo_suscripcion(&pool_hilos_suscripciones[pos], mensaje);
    sem_wait(&conexion_broker);
    return flag_conexion_broker;
}

void activar_hilo_suscripcion(pthread_t* hilo_suscripcion, Suscripcion* mensaje){
    activar_hilo(hilo_suscripcion, suscripcion_a_cola_broker, (void*)mensaje);
}

void* suscripcion_a_cola_broker(void* mensaje){
    int socket, respuesta_broker;
    socket = establecer_conexion(ip, puerto);
    if(se_pudo_conectar_socket(socket) == 1 && emitir_mensaje_suscripcion(mensaje, socket) != -1){
        sem_post(&conexion_broker);
        recibir_mensajes_de_cola(socket);
    }
    else{
        desactivar_flag_conexion_broker();
        sem_post(&conexion_broker);
    }
    matar_conexion(socket);
    return NULL;
}

void dessuscribirse_de_colas(){
    dessuscribirse_segun_lista_suscripciones();
    free(pool_hilos_suscripciones);
}

void dessuscribirse_de(int cola){
    join_hilo(&pool_hilos_suscripciones[cola]);
}

///////////////////-RECEPCION DE MENSAJES-/////////////////////
void recibir_mensajes_de_cola(int socket){
    int codigo, respuesta_recepcion;
    respuesta_recepcion = 1;
    while(flag_conexiones == 1 && respuesta_recepcion == 1){
        codigo = recibir_codigo(socket);//ver como aplicar el recv de forma no bloqueante
        if(codigo != -1){
            if(es_codigo_valido(codigo) == 1){
                procesar_mensaje(codigo, socket);
            }
        }
        else{respuesta_recepcion = 0;}
    }
}
//VER ESTA FUNCION// | TODO
int recibir_codigo(int socket){
    int codigo;
    recv(socket, &codigo, sizeof(codigo), 0);
    return codigo;
}

int es_codigo_valido(int codigo){
    int respuesta = esta_codigo_en_lista_suscipciones_mensajes_broker(codigo);
    return respuesta;
}

void procesar_mensaje(int codigo, int socket){
    switch(codigo){
        case CAUGHT:
            Caught* mensaje = malloc(sizeof(Caught));
            recibir_mensaje(mensaje, codigo, socket);
            int idBuscado = validar_id(mensaje);
            if(idBuscado != -1){
                pthread_mutex_lock(&listaIds_mutex);
                eliminar_elemento_lista_ids(idBuscado);
                pthread_mutex_unlock(&listaIds_mutex);
                procesar_mensaje_caught(mensaje);
                emitir_ack(idBuscado, socket);
            }
            free(mensaje);
            break;
        case APPEARED:
            Appeared* mensaje = malloc(sizeof(Appeared));
            recibir_mensaje(mensaje, codigo, socket);
            procesar_mensaje_server(mensaje);
            free(mensaje);
            break;
        case LOCALIZED: 
            Localized* mensaje = malloc(sizeof(Localized));
            recibir_mensaje(mensaje, codigo, socket);
            int idBuscado = validar_id(mensaje);
            if(idBuscado != -1){
                procesar_mensaje_server(mensaje);
                emitir_ack(idBuscado, socket);
            }
            free(mensaje);
            break;
        default: printf("Error este mensaje no proviene de una cola con la que trabaje TEAM\n");
    }
}

int validar_id(void* mensaje){
    int respuesta, corrId;
    respuesta = -1;
    corrId = id_mensaje_de(mensaje);
    if(esta_en_lista_ids(corrId) == 1){
        respuesta = corrId;
    }
    return respuesta;
}

int id_mensaje_de(Localized* mensaje){
    return mensaje->corrID;
}

int id_mensaje_de(Caught* mensaje){
    return mensaje->corrID;
}

///////////////////-EMISION DE MENSAJES-/////////////////////
void* emision_mensajes(void* parametros){
    do{       
        if(iniciar_comunicaciones_con_broker() == 0){
            desactivar_flag_conexion_broker();
            activar_reconexion_a_broker();
            asignar_comportamiento_default();
        }              
    }while(emitir_mensajes_catch() == 0);
    return NULL;
}

int iniciar_comunicaciones_con_broker(){
    int respuesta = suscribirse_a_colas();
    if(respuesta == 1){
        respuesta = emitir_mensajes_get();
    }
    return respuesta;
}

int emitir_mensaje_a_broker(mensaje_broker* unMensajeBroker, int codigo){
    int socket, idMensaje;
    idMensaje = -1;
    socket = establecer_conexion(ip, puerto);
    if(se_pudo_conectar_socket(socket) == 1){
        idMensaje = emitir_mensaje((void*)unMensajeBroker->mensaje, codigo, socket);
        if(idMensaje != -1){
            pthread_mutex_lock(&listaIds_mutex);
            agregar_a_lista_ids(idMensaje);
            pthread_mutex_unlock(&listaIds_mutex);
        }
    }
    matar_conexion(socket);    
    return idMensaje;
}

int emitir_mensaje(void* mensaje, int codigo, int socket){
    mandar_mensaje(mensaje, codigo, socket);
    int idMensaje = recibir_id_mensaje(socket);
    return idMensaje;
}

void emitir_ack(int id, int socket){
    mensaje_ack mensaje = armar_mensaje_ack(id);
    mandar_mensaje(&mensaje, ACK, socket);
}

int emitir_mensaje_suscripcion(void* mensaje, int socket){
    int respuesta = emitir_mensaje(mensaje, SUSCRIPCION, socket);
    return respuesta;
}

void eliminar_pokemon_de_mensajes_get(char* pokemon){
    pthread_mutex_lock(&listaGet_mutex);
    eliminar_pokemon_de_lista_get(pokemon);
    pthread_mutex_unlock(&listaGet_mutex);    
}

int emitir_mensajes_get(){
    int respuesta;
    pthread_mutex_lock(&listaGet_mutex);
    respuesta = emitir_mensajes_segun_lista_get();
    pthread_mutex_unlock(&listaGet_mutex);
    return respuesta;
}

int emitir_un_mensaje_get(mensaje_broker* mensaje){
    int idMensaje = emitir_mensaje_a_broker(mensaje, GET);
    if(idMensaje != -1){
        registrar_id_en_pos_objetivo_global(idMensaje, mensaje->posicion);
    }
    return idMensaje;
}

void enviar_mensaje_catch(Catch mensaje, int pos_entrenador){
    pthread_mutex_lock(&colaCatch_mutex);
    agregar_mensaje_catch_a_cola_mensajes_broker(mensaje, pos_entrenador);
    pthread_mutex_unlock(&colaCatch_mutex);
    sem_post(&colaCatch_llenos);
}
//pendiente ver si se puede cambiar la espera activa | TODO
int emitir_mensajes_catch(){
    int respuesta_mensajes_catch = 1;
    mensaje_broker* unMensajeCatch;
    while(flag_conexiones == 1 && respuesta_mensajes_catch == 1){
        if(sem_trywait(&colaCatch_llenos) != -1){
            pthread_mutex_lock(&colaCatch_mutex);
            unMensajeCatch = primero_cola_catch_de_mensajes_broker();
            pthread_mutex_unlock(&colaCatch_mutex);
            if(emitir_un_mensaje_catch(unMensajeCatch) == 1){
                pthread_mutex_lock(&colaCatch_mutex);
                actualizar_cola_catch_de_mensajes_broker();
                pthread_mutex_unlock(&colaCatch_mutex);
            }
            else{respuesta_mensajes_catch = 0;}
        }
    }
    return respuesta_mensajes_catch;
}

int comportamiento_catch_broker_conectado(mensaje_broker* mensaje){
    int idMensaje = emitir_mensaje_a_broker(mensaje, CATCH);
    if(idMensaje != -1){
        registrar_id_mensaje_catch(idMensaje, mensaje->posicion);
    }
    else{
        desactivar_flag_conexion_broker();
        activar_reconexion_a_broker();
        asignar_comportamiento_default();
    }
    return 1;
}

int comportamiento_catch_default(mensaje_broker* mensaje){
    int respuesta = 1;
    if(flag_conexion_broker_esta_activado() == 0){
        Caught unMensajeCaught = armar_mensaje_caught(-1, mensaje->posicion);//el valor -1 esta reservado para mensaje de comportamiento default, en ese caso resultado es la posicion del entrenador
        procesar_mensaje_caught(&unMensajeCaught);
    }
    else{
        activar_flag_conexion_broker();
        respuesta = 0;
    }
    return respuesta;
}

Caught armar_mensaje_caught(int id, int unResultado){
    Caught mensaje;
    mensaje.corrID = id;
    mensaje.resultado = unResultado;
    return mensaje;
}

///////////////////-RECONEXION-/////////////////////
void activar_hilo_reconexion(pthread_t* hilo_reconexion){
    activar_hilo(hilo_reconexion, reconectarse_a_broker, NULL);
}

void activar_reconexion_a_broker(){
    activar_hilo_reconexion(&hilo_reconexion);
}

void* reconectarse_a_broker(void* parametros){
    int socket = establecer_conexion(ip, puerto);
    if(se_pudo_conectar_socket(socket) == 1 &&  && flag_conexiones_esta_activado() == 1){
        activar_flag_conexion_broker();
    }
    else{
        do{Sleep(5000);}while(intento_reconexion(&socket) == 0 && flag_conexiones_esta_activado() == 1);
        if(flag_conexiones_esta_activado() == 1){
            activar_flag_conexion_broker();
        }
    }
    matar_conexion(socket);
    return NULL;
}
//QUEDA POR HACER | TODO
int intento_reconexion(int* socket){
    int respuesta = 0;
    return respuesta;
}
*/



