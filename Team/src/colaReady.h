/*
 * colaReady.h
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_COLAREADY_H_
#define SRC_COLAREADY_H_

#include <stddef.h>

typedef struct{
    float real_ant;
    float est_actual;
}estructura_estimacion;

typedef struct nodoColaR{
    int pos_entrenador;
    struct nodoColaR* next;
}elemento_cola_ready;

typedef struct{
    elemento_cola_ready* inicio;
    elemento_cola_ready* fondo;
}cola_Ready;

void inicializar_cola_ready();
void eliminar_cola_ready();
int cola_ready_vacia();
void actualizar_cola_ready();
int pos_primero_de_ready();
void agregar_a_ready(int);
void pusheo_en_ready(elemento_cola_ready*);
void calcular_estimacion(int);
void pusheo_ordenado_en_ready(elemento_cola_ready*);
int tiene_menor_estimacion_que(int, int);
void inicializar_vector_estimaciones(int, int);
void liberar_vector_estimaciones();
void notificar_rafagas_reales(int, int);
int incrementar_rafaga_actual_en_exec(int);
void setear_nuevo_primero(int);
void setear_est_actual_ant_de(int, int);
int recalcular_estimacion(int);
int estimacion_primero_de_ready();


#endif /* SRC_COLAREADY_H_ */
