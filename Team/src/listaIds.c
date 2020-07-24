#include "Team.h"

typedef struct nodoListaI{
    int id_mensaje;
    struct nodoListaI* next;
}elemento_id;

elemento_id* lista_ids;

void inicializar_lista_ids(){
    lista_ids = NULL;
}

void eliminar_lista_ids(){
    elemento_id* actual = lista_ids;
    while(actual != NULL){
        printf("eliminado mensaje id %i\n", actual->id_mensaje);
	elemento_id* temp = actual;
	actual = actual->next;
	free(temp);
    }
}

int esta_vacia_lista_ids(){
    int respuesta = 0;    
    if(lista_ids == NULL){
	respuesta = 1;
    }
    return respuesta;
}

void eliminar_elemento_lista_ids(int id_a_eliminar){
    if(esta_vacia_lista_ids() == 1){printf("Se quiso eliminar elemento de lista ids vacia\n");}
    else{
        elemento_id* actual = lista_ids;
        if(actual->next == NULL){
            if(actual->id_mensaje == id_a_eliminar){
                lista_ids = NULL;
                free(actual);
            }
            else{printf("Unico elemento de lista ids no coincide con mensaje id %i\n", id_a_eliminar);}
        }
        else{
            elemento_id* anterior = actual;
            actual = actual->next;
            while(actual != NULL && actual->id_mensaje != id_a_eliminar){
                anterior = actual;
                actual = actual->next;
            }
            if(actual != NULL){
                anterior->next = actual->next;
                free(actual);
            }
            else{printf("Ningun elemento de la lista ids coincide con mensaje id %i\n", id_a_eliminar);}
        }
    }
}

void agregar_a_lista_ids(int id){
    elemento_id* nuevoId = malloc(sizeof(elemento_id));
    nuevoId->id_mensaje = id;
    nuevoId->next = lista_ids;
    lista_ids = nuevoId;
}

int esta_en_lista_ids(int idMensaje){
    int respuesta = 0;
    if(esta_vacia_lista_ids() == 0){
        elemento_id* auxiliar = lista_ids;
        while(auxiliar != NULL && auxiliar->id_mensaje != idMensaje){
            auxiliar = auxiliar->next;
        }
        if(auxiliar != NULL){respuesta = 1;}
    }
    return respuesta;
}
