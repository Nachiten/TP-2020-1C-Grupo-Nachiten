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
void tratar_deadlock(d_entrenador*, int);

#endif /* SRC_DEADLOCK_H_ */
