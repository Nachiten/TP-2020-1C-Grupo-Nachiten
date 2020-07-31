#include "Team.h"

typedef struct nodoColaMb{
    mensaje_broker contenido;
    struct nodoColaMb* next;
}nodo_cola_mensajes_broker;

typedef struct{
    nodo_cola_mensajes_broker* inicio;
    nodo_cola_mensajes_broker* fondo;
}particion_cola_mensajes_broker;

typedef struct{
    nodo_cola_mensajes_broker* elementos;
    int cantidad_elementos;
}particion_lista_suscripciones_mensajes_broker;

typedef struct{
    particion_lista_suscripciones_mensajes_broker lista_suscripciones;
    nodo_cola_mensajes_broker* lista_get;
    particion_cola_mensajes_broker cola_catch;
}cola_Mensajes_Broker;

cola_Mensajes_Broker cola_mensajes_broker;

void inicializar_cola_mensajes_broker(){
    inicializar_lista_suscripciones();
    inicializar_lista_get();
    inicializar_cola_catch();
}

void inicializar_lista_suscripciones(){
    cola_mensajes_broker.lista_suscripciones.elementos = NULL;
    cola_mensajes_broker.lista_suscripciones.cantidad_elementos = 0;
}

void inicializar_lista_get(){
    cola_mensajes_broker.lista_get = NULL;
}

void inicializar_cola_catch(){
    cola_mensajes_broker.cola_catch.inicio = NULL;
    cola_mensajes_broker.cola_catch.fondo = NULL;
}

void eliminar_cola_mensajes_broker(){
    eliminar_cola_catch();
    eliminar_lista_get();
    eliminar_lista_suscripciones();
}

void eliminar_cola_catch(){
    nodo_cola_mensajes_broker* actual = cola_mensajes_broker.cola_catch.inicio;
    while(actual != NULL){
	nodo_cola_mensajes_broker* temp = actual;
	actual = actual->next;
        eliminar_nodo_cola_mensajes_broker(temp);
    }
}

void eliminar_lista_get(){
    nodo_cola_mensajes_broker* actual = cola_mensajes_broker.lista_get;
    while(actual != NULL){
	nodo_cola_mensajes_broker* temp = actual;
	actual = actual->next;
        eliminar_nodo_cola_mensajes_broker(temp);
    }
}

void eliminar_lista_suscripciones(){
    nodo_cola_mensajes_broker* actual = cola_mensajes_broker.lista_suscripciones.elementos;
    while(actual != NULL){
	nodo_cola_mensajes_broker* temp = actual;
	actual = actual->next;
        eliminar_nodo_cola_mensajes_broker(temp);
    }
}

void eliminar_elemento_lista_suscripciones(nodo_cola_mensajes_broker* elemento){
    eliminar_nodo_cola_mensajes_broker(elemento);
}

void eliminar_elemento_lista_get(nodo_cola_mensajes_broker* elemento){
    eliminar_nodo_cola_mensajes_broker(elemento);
}

void eliminar_elemento_cola_catch(nodo_cola_mensajes_broker* elemento){
    eliminar_nodo_cola_mensajes_broker(elemento);
}

void eliminar_nodo_cola_mensajes_broker(nodo_cola_mensajes_broker* nodo){
    free(nodo->contenido.mensaje);
    free(nodo);
}

void agregar_mensaje_get_a_cola_mensajes_broker(Get mensaje, int pos_en_objetivo_global){
    nodo_cola_mensajes_broker* nuevoMensaje = malloc(sizeof(nodo_cola_mensajes_broker));
    nuevoMensaje->contenido->mensaje = malloc(sizeof(Get));
    llenar_contenido_de_nodo_mensaje_broker(nuevoMensaje, &mensaje, pos_en_objetivo_global);
    nuevoMensaje->next = cola_mensajes_broker.lista_get;   
    cola_mensajes_broker.lista_get = nuevoMensaje;
}

void agregar_mensaje_catch_a_cola_mensajes_broker(Catch mensaje, int pos_entrenador){
    nodo_cola_mensajes_broker* nuevoMensaje = malloc(sizeof(nodo_cola_mensajes_broker));
    nuevoMensaje->contenido->mensaje = malloc(sizeof(Catch));
    llenar_contenido_de_nodo_mensaje_broker(nuevoMensaje, &mensaje, pos_entrenador);
    nuevoMensaje->next = NULL;
    if(esta_vacia_cola_catch_mensajes_broker() == 1){
	cola_mensajes_broker.cola_catch.inicio = nuevoMensaje;
	cola_mensajes_broker.cola_catch.fondo = nuevoMensaje;
    }
    else{
	cola_mensajes_broker.cola_catch.fondo->next = nuevoMensaje;
	cola_mensajes_broker.cola_catch.fondo = nuevoMensaje;
    }
}

void agregar_mensaje_suscripcion_a_cola_mensajes_broker(Suscripcion mensaje){
    nodo_cola_mensajes_broker* nuevoMensaje = malloc(sizeof(nodo_cola_mensajes_broker));
    nuevoMensaje->contenido->mensaje = malloc(sizeof(Suscripcion));
    llenar_contenido_de_nodo_mensaje_broker(nuevoMensaje, &mensaje, -1);
    nuevoMensaje->next = cola_mensajes_broker.lista_suscripciones.elementos;   
    cola_mensajes_broker.lista_suscripciones.elementos = nuevoMensaje;
    cola_mensajes_broker.lista_suscripciones.cantidad_elementos+=1;
}

void llenar_contenido_de_nodo_mensaje_broker(nodo_cola_mensajes_broker* nuevoMensaje, void* mensaje, int pos){
    nuevoMensaje->contenido.mensaje = mensaje;
    nuevoMensaje->contenido.posicion = pos;
}

int esta_vacia_cola_catch_mensajes_broker(){
    int respuesta = 0;    
    if(cola_mensajes_broker.cola_catch.inicio == NULL){
	respuesta = 1;
    }
    return respuesta;
}

int esta_vacia_lista_get_mensajes_broker(){
    int respuesta = 0;    
    if(cola_mensajes_broker.lista_get == NULL){
	respuesta = 1;
    }
    return respuesta;
}

void actualizar_cola_mensajes_broker(){
    actualizar_cola_catch_de_mensajes_broker();
}

void actualizar_cola_catch_de_mensajes_broker(){
    nodo_cola_mensajes_broker* temp = cola_mensajes_broker.cola_catch.inicio;
    cola_mensajes_broker.cola_catch.inicio = cola_mensajes_broker.cola_catch.inicio->next;
    eliminar_nodo_cola_mensajes_broker(temp);
}

void eliminar_pokemon_de_lista_get(char* pokemon){
    Get* unMensaje;
    int resultado_eliminacion = 0;
    nodo_cola_mensajes_broker* anterior = NULL;
    nodo_cola_mensajes_broker* actual = cola_mensajes_broker.lista_get;
    while(actual != NULL && resultado_eliminacion == 0){
	unMensaje = (Get*)actual->contenido.mensaje;
        if(son_iguales_char(pokemon, unMensaje->pokemon) == 1){
            if(anterior != NULL){
                anterior->next = actual->next;
            }
            else{cola_mensajes_broker.lista_get = cola_mensajes_broker.lista_get->next;}
            eliminar_elemento_lista_get(actual);
            resultado_eliminacion = 1;
        }
        else{
            anterior = actual;
            actual = actual->next;
        }
    }
}

int bytes_segun_codigo(int codigo){
    int bytes;
    switch(codigo){
        case GET : 
            bytes = sizeof(Get);
            break;
        case CATCH : 
            bytes = sizeof(mensaje_broker);
            break;
        default : 
            bytes = 0;
            printf("bytes de codigo invalido\n");
    }
    return bytes;
}

int cantidad_de_suscripciones_mensajes_broker(){
    return cola_mensajes_broker.lista_suscripciones.cantidad_elementos;
}

void primero_cola_mensajes_broker(){
    primero_cola_catch_de_mensajes_broker();
}

void primero_cola_catch_de_mensajes_broker(mensaje_broker* mensaje){
    mensaje = cola_mensajes_broker.cola_catch.inicio->contenido;
}

int suscribirse_segun_lista_suscripciones(){
    int i, resultado_suscripcion;
    resultado_suscripcion = 1;
    i = 0;
    nodo_cola_mensajes_broker* actual = cola_mensajes_broker.lista_suscripciones.elementos;
    while(resultado_suscripcion == 1 && i<cola_mensajes_broker.lista_suscripciones.cantidad_elementos){
        if(suscribirse_a_cola_broker(i, actual->contenido.mensaje) == 0){resultado_suscripcion = 0;}
        actual = actual->next;
        i++;
    }
    return resultado_suscripcion;
}

void dessuscribirse_segun_lista_suscripciones(){
    int num_cola;
    nodo_cola_mensajes_broker* actual = cola_mensajes_broker.lista_suscripciones.elementos;
    for(num_cola=0;num_cola<cola_mensajes_broker.lista_suscripciones.cantidad_elementos;num_cola++){
        dessuscribirse_de(num_cola);
        actual = actual->next;
    }
}

int esta_codigo_en_lista_suscipciones_mensajes_broker(int codigo){
    int resultado_busqueda = 1;  
    nodo_cola_mensajes_broker* actual = cola_mensajes_broker.lista_suscripciones.elementos;
    while(actual != NULL && cola_de_mensaje_suscripcion((Suscripcion*)actual->contenido.mensaje) != codigo){
        actual = actual->next;
    }
    if(actual == NULL){resultado_busqueda = 0;}
    return resultado_busqueda;
}

int cola_de_mensaje_suscripcion(Suscripcion* mensaje){
    return mensaje->numeroCola;
}

int emitir_mensajes_segun_lista_get(){
    int resultado_envio = 1;
    if(esta_vacia_lista_get_mensajes_broker() == 0){
        nodo_cola_mensajes_broker* actual = cola_mensajes_broker.lista_get;
        while(resultado_envio == 1 && actual != NULL){
            if(emitir_un_mensaje_get(actual->contenido) == -1){resultado_envio = 0;}
            actual = actual->next;
        }
    }
    else{resultado_envio = 0;}
    return resultado_envio;
}

