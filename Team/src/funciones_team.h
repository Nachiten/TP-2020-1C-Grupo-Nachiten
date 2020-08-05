/*
 * funciones_team.h
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#ifndef SRC_FUNCIONES_TEAM_H_
#define SRC_FUNCIONES_TEAM_H_

//#include "Team.h"
#include "estructurasTeam.h"
#include "semaphore.h"
#include <pthread.h>
#include "Team.h"

// Funciones agregadas por nacho
void sumarUnCicloCPU(d_entrenador*);
void sumarUnCambioContexto();

//*********************************************hilos que ya no sirven (fueron recodeados)
//void activar_hilo_recepcion(pthread_t*);
//void activar_hilo_administrador_cola_ready(pthread_t*);
//void activar_hilo_administrador_cola_caught(pthread_t*);



void init_sem(sem_t*, int);
void init_mutex(pthread_mutex_t*);
void destroy_mutex(pthread_mutex_t*);
void activar_hilo_circulo_deadlock(parametros_deadlock*, pthread_t*);
void join_hilo(pthread_t*);
void join_hilos(pthread_t*, int);
int esta_en_objetivo_actual(char*, char**, int);
int cantidad_de_veces_en_objetivo_actual(char*, char**, int);
int eliminar_de_objetivo_actual(char*, char***, int);
void cambiar_estado_a(d_entrenador*, int);
void bloquear(d_entrenador*, int);
int cumple_condicion_deadlock(d_entrenador);
int cant_objetivos(d_entrenador);
int cant_pokemones_actuales(d_entrenador, int);
void cant_objetivos_y_actuales_de(d_entrenador, int*, int*);
int esta_en_objetivo_individual(char*, char**);
int cantidad_de_veces_en_objetivo_individual(char*, char**);
int esta_en_actuales(char*, char**, int);
int cantidad_de_veces_en_actuales(char*, char**, int);
int diferencia_de_repeticiones_de_en_objetivos_y_actuales(char*, char**, char**, int);
int calcular_objetivo_propio(d_entrenador);
int esta_en_el_limite(d_entrenador);
int esta_terminado(d_entrenador);
void agregar_captura(d_entrenador*, char*);
int pokemon_se_repitio_antes_en_objetivo(char*, char**, int);
int buscar_y_reemplazar_por_en_actuales_de(char*, char*, d_entrenador*);
void actualizar_estado_entrenador(d_entrenador*, int);
int filtrar_mensaje(mensaje_server*, char**, int);
int esta_en_char(char*, char**, int);
int se_encuentra_en_char(char*, char**, int);
int esta_en_null(char*, char**);
int se_encuentra_en_null(char*, char**);
int esta_en_char_lleno(char*, char**, int);
int se_encuentra_en_char_lleno(char*, char**, int);
int son_iguales_char(char*, char*);
void llenar_con(int, int, char**, char*);
int calcular_mas_cerca_de(int, int, d_entrenador*, int);
int distancia_a(int, int, int, int);
int valor_absoluto(int);
void informar_estado_actual(d_entrenador*, int);
void se_cumplio_objetivo(d_entrenador*, int);
int cant_en_espera(d_entrenador*, int);
void llegar_por_eje(d_entrenador*, int, int);
void llegar_por_eje_con_quantum(d_entrenador*, int, int, int, int*, int);
void llegar_por_eje_con_seguimiento_de_rafaga(d_entrenador*, int, int, int);

#endif /* SRC_FUNCIONES_TEAM_H_ */
