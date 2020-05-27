/*
 * deadlock.h
 *
 *  Created on: 9 may. 2020
 *      Author: utnso
 */

#ifndef SRC_DEADLOCK_H_
#define SRC_DEADLOCK_H_

#include "Team.h"

void sacar_en_espera(deadlock_entrenador**, d_entrenador*, int);
void calcular_innecesarios_y_necesarios(d_entrenador, int*, int*);
void llenar_estructura_deadlock(d_entrenador, deadlock_entrenador, int, int);
void tratar_deadlock(deadlock_entrenador*, int, d_entrenador*);
int detectar_deadlock(deadlock_entrenador*, int, int*);
int alguno_se_encuentra_en(char**, char**);
int se_encuentra_en_null(char*, char**);
void desde_pivote(deadlock_entrenador*, int, int, int*, int*);
int esta_en_int(int, int*, int);
void recuperar_deadlock(deadlock_entrenador*, int, int*, d_entrenador*);
void borrar_de(char*, char**);
void realizar_intercambio(d_entrenador*, char*, d_entrenador*, char*);
void intercambiar(d_entrenador*, char*, d_entrenador*, char*);
void actualizar_estado(d_entrenador*, int, deadlock_entrenador*, int, int*);
void liberar_temporal_comp(deadlock_entrenador*, int);

#endif /* SRC_DEADLOCK_H_ */
