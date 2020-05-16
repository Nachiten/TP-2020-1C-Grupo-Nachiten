/*
 * deadlock.h
 *
 *  Created on: 9 may. 2020
 *      Author: utnso
 */

#ifndef SRC_DEADLOCK_H_
#define SRC_DEADLOCK_H_

#include "Team.h"

typedef struct{
	int ubicacion;
	char** no_necesito;
	char** quiero;
} estructura_deadlock;

int detectar_deadlock(d_entrenador*, int, int*, int*, int*);

#endif /* SRC_DEADLOCK_H_ */
